#include "pattern.h"

#include "gtest/gtest.h"
#include "note.h"

namespace z2music {

TEST(PatternTest, Constructed) {
  Pattern pattern = Pattern(0x18,
                            {Note::Pitch::A4 | Note::Duration::Eighth,
                             Note::Pitch::C5 | Note::Duration::Quarter,
                             Note::Pitch::E5 | Note::Duration::Half},
                            {}, {}, {});
  std::vector<byte> expected = {0xa2, 0xe8, 0x71, 0x00};

  EXPECT_EQ(pattern.note_data(Pattern::Channel::Pulse1), expected);
  EXPECT_EQ(pattern.note_data(), expected);
}

}  // namespace z2music
