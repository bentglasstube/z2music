#ifndef Z2MUSIC_CREDITS
#define Z2MUSIC_CREDITS

#include <array>
#include <string>

#include "util.h"

namespace z2music {

class Credits {
 public:
  Credits() {}

  struct Text {
    std::string title;
    std::string name1;
    std::string name2;
  };

  Text get(byte page) const { return credits_[page]; }

  const Text operator[](byte page) const { return credits_[page]; }
  Text& operator[](byte page) { return credits_[page]; }

  static constexpr byte kPages = 9;

  const Text* begin() const { return &credits_[0]; }
  const Text* end() const { return &credits_[kPages]; }

 private:
  std::array<Text, kPages> credits_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_CREDITS
