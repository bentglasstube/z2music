#ifndef Z2MUSIC_CREDITS
#define Z2MUSIC_CREDITS

#include <array>
#include <cstddef>
#include <string>

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

  void set(size_t page, const Text& text);
  Text get(size_t page) const;
  void commit(Rom& rom) const;

 private:
  static constexpr size_t kCreditsTableAddress = 0x015259;
  static constexpr size_t kCreditsBankOffset = 0xc000;
  static constexpr size_t kCreditsPages = 9;

  std::array<Text, kCreditsPages> credits_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_CREDITS
