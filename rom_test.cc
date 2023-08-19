#include "rom.h"

#include "fake_rom.h"
#include "gtest/gtest.h"
#include "pattern.h"

namespace z2music {

TEST(RomTest, AutomaticPitchLUT) {
  FakeRom rom;
  Song& song = rom.song(Rom::SongTitle::TriforceFanfare);

  song.clear();
  song.add_pattern(
      {0x18,
       Pattern::parse_notes("A5.2 C6 E6 A5 C6 E6 A5 C6 E6 A5 C6 E6"),
       {},
       {},
       {}});

  song.set_sequence({0});

  rom.write(0x12345, {0x38, 0x48, 0x08, 0x06, 0x08, 0x06});  // A5 G#5 G#3 G3
  rom.read_sfx_notes(0x12345, 6);

  rom.rebuild_pitch_lut();
  auto& lut = rom.pitch_lut();

  EXPECT_EQ(lut.size(), 7);
  // Notes from song
  EXPECT_EQ(lut[0x00], Pitch(Pitch::A5));
  EXPECT_EQ(lut[0x02], Pitch::none());
  EXPECT_EQ(lut[0x04], Pitch(Pitch::C6));
  EXPECT_EQ(lut[0x06], Pitch(Pitch::E6));

  // Notes from SFX
  EXPECT_EQ(lut[0x08], Pitch(Pitch::Gs5));
  EXPECT_EQ(lut[0x0a], Pitch(Pitch::Gs3));
  EXPECT_EQ(lut[0x0c], Pitch(Pitch::G3));

  rom.commit_sfx_notes();

  auto data = rom.read(0x12345, 6);
  const std::vector<byte> expected = {0x00, 0x08, 0x0a, 0x0c, 0x0a, 0x0c};
  EXPECT_EQ(data, expected);
}

}  // namespace z2music
