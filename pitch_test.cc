#include "pitch.h"

#include "gtest/gtest.h"

#define EXPECT_ERROR(a, b, err) EXPECT_NEAR((a), (b), ((err) * (b)))

TEST(PitchTest, Conversions) {
  z2music::Pitch a4(0x00fd);
  EXPECT_ERROR(a4.freq(), 440.f, 0.01f);
  EXPECT_EQ(a4.midi(), z2music::Pitch::Midi::A4);

  z2music::Pitch f3(0x0280);
  EXPECT_ERROR(f3.freq(), 174.6f, 0.01f);
  EXPECT_EQ(f3.midi(), z2music::Pitch::Midi::F3);

  z2music::Pitch g5 = z2music::Pitch::from_freq(784.f);
  EXPECT_EQ(g5.timer, 0x008e);
  EXPECT_EQ(g5.midi(), z2music::Pitch::Midi::G5);

  z2music::Pitch e7 = z2music::Pitch::from_midi(z2music::Pitch::Midi::E7);
  EXPECT_ERROR(e7.freq(), 2637.f, 0.01f);
  EXPECT_EQ(e7.timer, 0x0029);
}

TEST(PitchTest, Comparisons) {
  z2music::Pitch a4 = z2music::Pitch::from_midi(z2music::Pitch::Midi::A4);
  z2music::Pitch a4f = z2music::Pitch::from_freq(440.f);
  z2music::Pitch a4t = z2music::Pitch(0x00fd);
  EXPECT_EQ(a4, a4f);
  EXPECT_EQ(a4, a4t);

  z2music::Pitch b4 = z2music::Pitch::from_midi(z2music::Pitch::Midi::B4);
  EXPECT_GT(b4, a4);

  z2music::Pitch cs3 = z2music::Pitch::from_midi(z2music::Pitch::Midi::Cs3);
  z2music::Pitch db3 = z2music::Pitch::from_midi(z2music::Pitch::Midi::Db3);
  EXPECT_EQ(cs3, db3);
}
