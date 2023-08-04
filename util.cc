#include "util.h"

#include <iomanip>

namespace z2music {

namespace {
std::ostream& print_hex(std::ostream& os, uint32_t value, int width) {
  const auto flags = os.flags();
  os << "0x" << std::hex << std::setfill('0') << std::setw(width) << value;
  os.setf(flags, std::ios_base::basefield);
  return os;
}
}  // namespace

std::ostream& operator<<(std::ostream& os, const byte& b) {
  return print_hex(os, b, 2);
}

std::ostream& operator<<(std::ostream& os, const WordLE& word) {
  return print_hex(os, word, 4);
}

std::ostream& operator<<(std::ostream& os, const WordBE& word) {
  return print_hex(os, word, 4);
}

std::ostream& operator<<(std::ostream& os, const Address& a) {
  return print_hex(os, a, 6);
}

std::istream& operator>>(std::istream& is, byte& b) {
  auto flags = is.flags();
  is >> std::setbase(0) >> b.value;
  is.setf(flags, std::ios_base::basefield);
  return is;
}

}  // namespace z2music
