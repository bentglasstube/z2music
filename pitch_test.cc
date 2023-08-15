#include "pitch.h"

#include "absl/log/log.h"
#include "gtest/gtest.h"

#define EXPECT_ERROR(a, b, err) EXPECT_NEAR((a), (b), ((err) * (b)))

namespace z2music {

TEST(PitchTest, Conversions) {
  Pitch a4(static_cast<WordBE>(0x00fd));
  EXPECT_ERROR(a4.freq(), 440.f, 0.01f);
  EXPECT_EQ(a4.midi(), Pitch::A4);

  Pitch f3(static_cast<WordBE>(0x0280));
  EXPECT_ERROR(f3.freq(), 174.6f, 0.01f);
  EXPECT_EQ(f3.midi(), Pitch::F3);

  Pitch g5 = Pitch(784.f);
  EXPECT_EQ(g5.timer(), 0x008e);
  EXPECT_EQ(g5.midi(), Pitch::G5);

  Pitch e7 = Pitch(Pitch::E7);
  EXPECT_ERROR(e7.freq(), 2637.f, 0.01f);
  EXPECT_EQ(e7.timer(), 0x0029);
}

TEST(PitchTest, Comparisons) {
  Pitch a4 = Pitch(Pitch::A4);
  Pitch a4f = Pitch(440.f);
  Pitch a4t = Pitch(static_cast<WordBE>(0x00fd));
  EXPECT_EQ(a4, a4f);
  EXPECT_EQ(a4, a4t);

  Pitch b4 = Pitch(Pitch::B4);
  EXPECT_GT(b4, a4);
  LOG(INFO) << b4 << ": midi " << b4.midi();
  LOG(INFO) << a4 << ": midi " << a4.midi();

  Pitch cs3 = Pitch(Pitch::Cs3);
  Pitch db3 = Pitch(Pitch::Db3);
  EXPECT_EQ(cs3, db3);
}
}  // namespace z2music
