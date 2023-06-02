#include "pitch.h"

#include "gtest/gtest.h"

#define EXPECT_ERROR(a, b, err) EXPECT_NEAR((a), (b), ((err) * (b)))

TEST(PitchTest, Conversions) {
  z2music::Pitch a4(0x00fd);
  EXPECT_ERROR(a4.freq(), 440.f, 0.01f);
  EXPECT_EQ(a4.midi(), 69);

  z2music::Pitch f3(0x0280);
  EXPECT_ERROR(f3.freq(), 174.6f, 0.01f);
  EXPECT_EQ(f3.midi(), 53);

  z2music::Pitch g5 = z2music::Pitch::from_freq(784.f);
  EXPECT_EQ(g5.timer, 0x008e);
  EXPECT_EQ(g5.midi(), 79);

  z2music::Pitch e7 = z2music::Pitch::from_midi(100);
  EXPECT_ERROR(e7.freq(), 2637.f, 0.01f);
  EXPECT_EQ(e7.timer, 0x0029);
}
