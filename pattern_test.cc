#include "pattern.h"

#include <array>
#include <sstream>

#include "absl/log/log.h"
#include "gtest/gtest.h"
#include "note.h"
#include "pitch.h"
#include "rom.h"

namespace z2music {

class TestWithFakeRom : public ::testing::Test {
 protected:
  Rom rom;

  TestWithFakeRom() {
    std::array<Pitch::Midi, 59> pitches = {
        Pitch::E3,  Pitch::G3,  Pitch::Gs3, Pitch::A3,  Pitch::As3, Pitch::B3,
        Pitch::C4,  Pitch::Cs4, Pitch::D4,  Pitch::Ds4, Pitch::E4,  Pitch::F4,
        Pitch::Fs4, Pitch::G4,  Pitch::Gs4, Pitch::A4,  Pitch::As4, Pitch::B4,
        Pitch::C5,  Pitch::Cs5, Pitch::D5,  Pitch::Ds5, Pitch::E5,  Pitch::F5,
        Pitch::Fs5, Pitch::G5,  Pitch::A5,  Pitch::As5, Pitch::B5,  Pitch::Cs3,
        Pitch::D3,  Pitch::Ds3, Pitch::F3,  Pitch::Fs3, Pitch::Gs5, Pitch::C6,
        Pitch::Cs6, Pitch::D6,  Pitch::Ds6, Pitch::E6,  Pitch::F6,  Pitch::Fs6,
        Pitch::G6,  Pitch::Gs6, Pitch::A6,  Pitch::As6, Pitch::B6,  Pitch::C7,
        Pitch::Cs7, Pitch::D7,  Pitch::Ds7, Pitch::E7,  Pitch::F7,  Pitch::Fs7,
        Pitch::G7,  Pitch::Gs7, Pitch::A7,  Pitch::As7, Pitch::B7};
    rom.pitch_lut().add_pitch(Pitch(Pitch::C3));
    rom.pitch_lut().add_pitch(Pitch::none());
    for (auto const p : pitches) {
      rom.pitch_lut().add_pitch(Pitch(p));
    }
  }

  void EXPECT_DATA_EQ(const Pattern& pattern, const std::vector<byte> metadata,
                      const std::vector<byte> expected_data) {
    EXPECT_EQ(pattern.meta_data(0x1234), metadata);

    auto encoded_data = rom.encode_pattern(pattern);
    EXPECT_EQ(encoded_data, expected_data);
  }
};

TEST_F(TestWithFakeRom, SingleChannel) {
  std::ostringstream data;
  for (const auto p : rom.pitch_lut()) {
    data << p.to_string() << " ";
  }
  LOG(INFO) << "Pitch LUT data: " << data.str();

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
                             {Pitch(Pitch::E3), Note::Duration::EighthTriplet},
                             {Pitch(Pitch::G3), Note::Duration::EighthTriplet}},
                            {}, {}, {});

  // FIXME this is what data is generated but it's incorrect.
  EXPECT_DATA_EQ(pattern, {0x18, 0x34, 0x12, 0x00, 0x00, 0x00},
                 {0x81, 0x85, 0x87, 0x00});
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

TEST(PatternTest, RoundTrip) {
  const std::string input_pw1 =
      "F4.8 r.t2 r F4 F4 F4 F4 F4 r D#4 F4.4 r.t2 r F4 F4 F4 F4 F4 r D#4 "
      "F4.4 "
      "r.t2 r F4 F4 F4 F4 F4.2 C4.1 C4 C4.2 C4.1 C4 C4.2 C4.1 C4 C4.2 C4";
  const std::string input_pw2 =
      "A3.8 r.t2 r A3 A3 A3 A3 G3 r G3 G3.4 r G3.t2 G3 G3 G#3 r G#3 G#3.4 "
      "r.t2 "
      "r G#3 G#3 G#3 G#3 G#3.2 E3.1 E3 E3.2 E3.1 E3 E3.2 E3.1 E3 E3.2 E3";
  const std::string input_triangle =
      "F4.4 F4.t2 F4 F4 F4.4 F4.t2 F4 F4 D#4.4 D#4.t2 D#4 D#4 D#4.4 D#4.t2 "
      "D#4 "
      "D#4 C#4.4 C#4.t2 C#4 C#4 C#4.4 C#4.t2 C#4 C#4 C4.4 C4 C4 D4.2 E4";
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

}  // namespace z2music
