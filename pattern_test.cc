#include "pattern.h"

#include "gtest/gtest.h"
#include "note.h"

namespace z2music {

namespace {
void EXPECT_DATA_EQ(const Pattern& pattern, const std::vector<byte> metadata,
                    const std::vector<byte> data) {
  EXPECT_EQ(pattern.meta_data(0x1234), metadata);
  EXPECT_EQ(pattern.note_data(), data);
}
}  // namespace

TEST(PatternTest, SingleChannel) {
  Pattern pattern = Pattern(0x18,
                            {Note::Pitch::A4 | Note::Duration::Eighth,
                             Note::Pitch::C5 | Note::Duration::Quarter,
                             Note::Pitch::E5 | Note::Duration::Half},
                            {}, {}, {});

  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0xa2, 0xe8, 0x71, 0x00});
}

TEST(PatternTest, AllChannels) {
  Pattern pattern = Pattern(0x20, {Note::Pitch::A4 | Note::Duration::Half},
                            {Note::Pitch::C5 | Note::Duration::Half},
                            {Note::Pitch::E5 | Note::Duration::Half},
                            {Note::Pitch::Gs3 | Note::Duration::Sixteenth});

  EXPECT_DATA_EQ(pattern, {0x20, 0x34, 0x12, 0x03, 0x02, 0x04},
                 {0x63, 0x00, 0x69, 0x71, 0x08, 0x00});
}

TEST(PatternTest, Triplets) {
  Pattern pattern = Pattern(0x18,
                            {Note::Pitch::C3 | Note::Duration::EighthTriplet,
                             Note::Pitch::E3 | Note::Duration::EighthTriplet,
                             Note::Pitch::G3 | Note::Duration::EighthTriplet},
                            {}, {}, {});

  // FIXME this is what data is generated but it's incorrect.
  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0x81, 0x85, 0x87, 0x00});
}

TEST(PatternTest, Parsing) {
  Pattern pattern{
      0x18,
      z2music::Pattern::parse_notes(
          "a4.4 b4 c5 b4.2 a4 c5.8 b4.4 a4 b4.8 a4.4 g#4 a4.8 g#4", 2),
      z2music::Pattern::parse_notes("r.4 a3 g#3.2 a3.6 r.4 a3 g#3.2 a3.6 r.4 "
                                    "a3 g#3.2 a3.6 c4.4 b3 a3 g#3.2 a3",
                                    2),
      z2music::Pattern::parse_notes(
          "a4.2 a4 a4 a4 a4 a4 a4 a4 f4 f4 f4 f4 f4 f4 f4 f4 g4 g4 g4 g4 g4 g4 "
          "g4 g4 e4 e4 e4 e4 e4 e4 e4 e4",
          2),
      z2music::Pattern::parse_notes(
          "x.4 x.2 x.6 x.4 x x.2 x.6 x.4 x x.2 x.6 x.4 x x.2 x.4 x.2 x.4"),
  };

  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x1f, 0x0e, 0x3f},
                 {
                     0xe6, 0xea, 0xec, 0xaa, 0xa6, 0x6d, 0xea, 0xe6, 0x6b, 0xe6,
                     0xe4, 0x67, 0x65, 0x00, 0xc2, 0xce, 0x8c, 0x0f, 0xc2, 0xce,
                     0x8c, 0x0f, 0xc2, 0xce, 0x8c, 0x0f, 0xd4, 0xd2, 0xce, 0x8c,
                     0x8e, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0x9e,
                     0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0x9e, 0xa2, 0xa2, 0xa2,
                     0xa2, 0xa2, 0xa2, 0xa2, 0xa2, 0x9c, 0x9c, 0x9c, 0x9c, 0x9c,
                     0x9c, 0x9c, 0x9c, 0xc8, 0x88, 0x09, 0xc8, 0xc8, 0x88, 0x09,
                     0xc8, 0xc8, 0x88, 0x09, 0xc8, 0xc8, 0x88, 0xc8, 0x88, 0xc8,
                 });
}

TEST(PatternTest, RoundTrip) {
  const std::string input_pw1 =
      "r.4 A4 B4.2 C5.4 D5.6 A4.4 B4.2 C5.4 E5.6 A4.4 B4.2 C5.6 F5.4 E5 D5 "
      "E5.2 C5";
  const std::string input_pw2 =
      "A3.4 G#3 A3 B3 C4 r B3 C4 D4 r C4 B3 C4 B3 A3 G#3";
  const std::string input_triangle =
      "A4.2 A4 A4 A4 A4 A4 A4 A4 F4 F4 F4 F4 F4 F4 F4 F4 G4 G4 G4 G4 G4 G4 G4 "
      "G4 E4 E4 E4 E4 E4 E4 E4 E4";
  const std::string input_noise =
      "G#3.2 G#3 G#3 G#3 G#3.4 G#3.2 G#3 G#3.4 G#3 G#3.2 G#3 G#3.4 G#3.2 G#3 "
      "G#3 G#3 G#3.4 G#3.2 G#3 G#3.4 G#3 G#3.2 G#3 G#3 G#3";

  Pattern pattern{
      0x18,
      z2music::Pattern::parse_notes(input_pw1),
      z2music::Pattern::parse_notes(input_pw2),
      z2music::Pattern::parse_notes(input_triangle),
      z2music::Pattern::parse_notes(input_noise),
  };

  EXPECT_EQ(input_pw1, pattern.dump_notes(z2music::Pattern::Channel::Pulse1));
  EXPECT_EQ(input_pw2, pattern.dump_notes(z2music::Pattern::Channel::Pulse2));
  EXPECT_EQ(input_triangle,
            pattern.dump_notes(z2music::Pattern::Channel::Triangle));
  EXPECT_EQ(input_noise, pattern.dump_notes(z2music::Pattern::Channel::Noise));
}

}  // namespace z2music
