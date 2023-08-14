#include "rom.h"

#include "gtest/gtest.h"
#include "pattern.h"

namespace z2music {

TEST(RomTest, AutomaticPitchLUT) {
  Rom rom;
  Song& song = rom.song(Rom::SongTitle::TriforceFanfare);

  song.clear();
  song.add_pattern(
      {0x18,
       Pattern::parse_notes("A4.2 C5 E5 A4 C5 E5 A4 C5 E5 A4 C5 E5"),
       {},
       {},
       {}});

  song.set_sequence({0});

  rom.rebuild_pitch_lut();
  auto& lut = rom.pitch_lut();

  EXPECT_EQ(lut.size(), 5);
  EXPECT_EQ(lut[0x00], Pitch::none());
  EXPECT_EQ(lut[0x02], Pitch::none());
  EXPECT_EQ(lut[0x04], Pitch(Pitch::A4));
  EXPECT_EQ(lut[0x06], Pitch(Pitch::C5));
  EXPECT_EQ(lut[0x08], Pitch(Pitch::E5));
}

}  // namespace z2music
