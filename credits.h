#ifndef Z2MUSIC_CREDITS
#define Z2MUSIC_CREDITS

#include <array>
#include <string>

#include "util.h"

namespace z2music {

class Rom;

class Credits {
 public:
  Credits();
  Credits(const Rom& rom);

  struct Text {
    std::string title;
    std::string name1;
    std::string name2;
  };

  void set(byte page, const Text& text);
  Text get(byte page) const;
  void commit(Rom& rom) const;

 private:
  static constexpr Address kCreditsTableAddress = 0x015259;
  static constexpr Address kCreditsBankOffset = 0xc000;
  static constexpr byte kCreditsPages = 9;

  std::array<Text, kCreditsPages> credits_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_CREDITS
