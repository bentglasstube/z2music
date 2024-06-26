#include "pattern.h"

#include <array>

#include "fake_rom.h"
#include "gtest/gtest.h"
#include "note.h"
#include "pitch.h"
#include "rom.h"

namespace z2music {

class TestWithFakeRom : public ::testing::Test {
 protected:
  FakeRom rom;

  void EXPECT_DATA_EQ(const Pattern& pattern, const std::vector<byte> metadata,
                      const std::vector<byte> expected_data) {
    EXPECT_EQ(pattern.meta_data(0x1234), metadata);
    auto encoded_data = rom.encode_pattern(pattern);
    EXPECT_EQ(encoded_data, expected_data);
  }
};

TEST_F(TestWithFakeRom, SingleChannel) {
  Pattern pattern = Pattern(0x18,
                            {{Pitch(Pitch::A4), Note::Duration::Eighth},
                             {Pitch(Pitch::C5), Note::Duration::Quarter},
                             {Pitch(Pitch::E5), Note::Duration::Half}},
                            {}, {}, {});

  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0xa2, 0xe8, 0x71, 0x00});
}

TEST_F(TestWithFakeRom, AllChannels) {
  Pattern pattern = Pattern(0x20, {{Pitch(Pitch::A4), Note::Duration::Half}},
                            {{Pitch(Pitch::C5), Note::Duration::Half}},
                            {{Pitch(Pitch::E5), Note::Duration::Half}},
                            {{Pitch(Pitch::Gs3), Note::Duration::Sixteenth}});

  EXPECT_DATA_EQ(pattern, {0x20, 0x34, 0x12, 0x03, 0x02, 0x04},
                 {0x63, 0x00, 0x69, 0x71, 0x08, 0x00});
}

TEST_F(TestWithFakeRom, Triplets) {
  Pattern pattern = Pattern(0x18,
                            {{Pitch(Pitch::C3), Note::Duration::EighthTriplet},
                             {Pitch(Pitch::C3), Note::Duration::EighthTriplet},
                             {Pitch(Pitch::C3), Note::Duration::EighthTriplet}},
                            {}, {}, {});

  // Row 0x18 has true triplets
  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0x81, 0x81, 0x81, 0x00});

  // Row 0x10 has to round
  pattern.tempo(0x10);
  EXPECT_DATA_EQ(pattern, {0x10, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0x81, 0x81, 0xc1, 0x00});

  // Row 0x28 rounds the opposite way
  pattern.tempo(0x28);
  EXPECT_DATA_EQ(pattern, {0x28, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0x81, 0x81, 0xc1, 0x00});
}

TEST_F(TestWithFakeRom, QuarterTriplets) {
  Pattern pattern =
      Pattern(0x18,
              {{Pitch(Pitch::C3), Note::Duration::QuarterTriplet},
               {Pitch(Pitch::C3), Note::Duration::QuarterTriplet},
               {Pitch(Pitch::C3), Note::Duration::QuarterTriplet}},
              {}, {}, {});

  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0xc1, 0xc1, 0xc1, 0x00});
}

TEST_F(TestWithFakeRom, Parsing) {
  Pattern pattern{
      0x18,
      z2music::Pattern::parse_notes(
          "a4.4 b4 c5 b4.2 a4 c5.8 b4.4 a4 b4.8 a4.4 g#4 a4.8 g#4", 2),
      z2music::Pattern::parse_notes("r.4 a3 g#3.2 a3.6 r.4 a3 g#3.2 a3.6 r.4 "
                                    "a3 g#3.2 a3.6 c4.4 b3 a3 g#3.2 a3",
                                    2),
      z2music::Pattern::parse_notes("a4.2 a4 a4 a4 a4 a4 a4 a4 f4 f4 f4 f4 "
                                    "f4 f4 f4 f4 g4 g4 g4 g4 g4 g4 "
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

TEST_F(TestWithFakeRom, Dumping) {
  rom.add_pattern(0x4242, 0x10,
                  {0xe6, 0xf4, 0xac, 0xea, 0xa6, 0xe6, 0xf4, 0xac, 0x2b, 0xe6,
                   0xf4, 0xac, 0xaa, 0xa6, 0xaa, 0x67, 0x65});

  Pattern pattern = rom.read_pattern(0x4242);
  EXPECT_EQ(pattern.length(), Note::Duration::Whole * 4);
  EXPECT_EQ(pattern.dump_notes(Pattern::Channel::Pulse1),
            "B4.4 F#5 D5.2 C#5.4 B4.2 B4.4 F#5 D5.2 C#5.6 B4.4 F#5 D5.2 C#5 B4 "
            "C#5 B4.8 A#4");
}

TEST_F(TestWithFakeRom, TitleParsing) {
  Pattern pattern{
      0x20,
      0x30,
      z2music::Pattern::parse_notes(
          "g5.4 d5.6 g5.1 r g5 a5 a#5 c6 a5.10 g5.2 f5 g5"),
      z2music::Pattern::parse_notes("r.8 r.10 g4.1 r g4 a4 a#4 c5 a4.8"),
      z2music::Pattern::parse_notes(
          "g3.2 r g3 d3 g3 r g3 d3 g3 r g3 d3 g3 d3 g3 d3"),
      z2music::Pattern::parse_notes("f2.4 f2 r.2 f2 r.1 f2 r.2")};

  EXPECT_DATA_EQ(
      pattern, {0x00, 0x34, 0x12, 0x1f, 0x12, 0x30, 0x20, 0x30},
      {0x83, 0x56, 0x84, 0x4c, 0x80, 0x56, 0x02, 0x56, 0x5a, 0x5c, 0x60, 0x8a,
       0x5a, 0x82, 0x56, 0x52, 0x56, 0x00, 0x85, 0x02, 0x8a, 0x02, 0x80, 0x3e,
       0x02, 0x3e, 0x42, 0x44, 0x48, 0x85, 0x42, 0x82, 0x26, 0x02, 0x26, 0x1c,
       0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x1c, 0x26, 0x1c,
       0x83, 0x0a, 0x0a, 0x82, 0x02, 0x0a, 0x80, 0x02, 0x0a, 0x82, 0x02, 0x00});
}

TEST_F(TestWithFakeRom, TitleDumping) {
  rom.add_pattern(0x1234, 0x00,
                  {
                      0x83, 0x02, 0x48, 0x82, 0x46, 0x83, 0x3E, 0x34, 0x84,
                      0x2E, 0x83, 0x30, 0x34, 0x3A, 0x38, 0x34, 0x30, 0x82,
                      0x34, 0x83, 0x30, 0x85, 0x2E, 0x82, 0x02, 0x00,
                  });

  Pattern pattern = rom.read_pattern(0x1234);
  EXPECT_EQ(pattern.length(), Note::Duration::Whole * 4);
  EXPECT_EQ(pattern.dump_notes(Pattern::Channel::Pulse1),
            "r.4 C5 B4.2 G4.4 D4 B3.6 C4.4 D4 F4 E4 D4 C4 D4.2 C4.4 B3.8 r.2");
}

TEST(PatternTest, RoundTrip) {
  const std::string input_pw1 =
      "F4.8 r.2t r F4 F4 F4 F4 F4 r D#4 F4.4 r.2t r F4 F4 F4 F4 F4 r D#4 "
      "F4.4 "
      "r.2t r F4 F4 F4 F4 F4.2 C4.1 C4 C4.2 C4.1 C4 C4.2 C4.1 C4 C4.2 C4";
  const std::string input_pw2 =
      "A3.8 r.2t r A3 A3 A3 A3 G3 r G3 G3.4 r G3.2t G3 G3 G#3 r G#3 G#3.4 "
      "r.2t "
      "r G#3 G#3 G#3 G#3 G#3.2 E3.1 E3 E3.2 E3.1 E3 E3.2 E3.1 E3 E3.2 E3";
  const std::string input_triangle =
      "F4.4 F4.2t F4 F4 F4.4 F4.2t F4 F4 D#4.4 D#4.2t D#4 D#4 D#4.4 D#4.2t "
      "D#4 "
      "D#4 C#4.4 C#4.2t C#4 C#4 C#4.4 C#4.2t C#4 C#4 C4.4 C4 C4 D4.2 E4";
  const std::string input_noise =
      "G#3.4 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3 G#3";

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

TEST(PatternTest, RoundTrip2) {
  const std::string input_pw1 = "A#4.4 G#4.2 A#4.4 G#4.6 G4.3 B4 D5.2 G5.8";
  const std::string input_pw2 = "D4.4 D4.2 D4.4 D4.6 D4.3 G4 B4.2 D5.8 F4.4";
  const std::string input_triangle =
      "F4.4 F4.2 F4.4 F4.6 G4.1 D5.2 G4.1 D4.2 G4.1 D5.2 G4.1 D4.2 G4 D5";
  const std::string input_noise =
      "G#3.4 G#3.2 G#3.4 G#3.2 G#3 G#3 G#3.1 G#3.2 G#3.1 G#3.2 G#3.1 G#3.2 "
      "G#3.1 G#3.2 G#3 G#3";

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

TEST_F(TestWithFakeRom, TownTheme06) {
  rom.add_pattern(0x1234, 0x20,
                  {0xe4, 0xa0, 0xe4, 0x21, 0x9f, 0xa7, 0xed, 0x77, 0x00});

  Pattern pattern = rom.read_pattern(0x1234);
  EXPECT_EQ(pattern.length(), Note::Duration::Whole * 2);
  EXPECT_EQ(pattern.dump_notes(Pattern::Channel::Pulse1),
            "A#4.4 G#4.2 A#4.4 G#4.6 G4.4t B4 D5 G5.8");
}

}  // namespace z2music
