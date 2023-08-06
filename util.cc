#include "util.h"

#include <iomanip>

namespace z2music {

namespace {

std::ostream& print_hex(std::ostream& os, uint32_t value, int width) {
  const auto flags = os.flags();
  char fill = os.fill('0');
  os << "0x" << std::hex << std::noshowbase << std::setw(width) << value;
  os.setf(flags, std::ios_base::basefield | std::ios_base::showbase);
  os.fill(fill);
  return os;
}

uint32_t read_hex(std::istream& is) {
  auto flags = is.flags();
  uint32_t value;
  is >> std::setbase(0) >> value;
  is.setf(flags, std::ios_base::basefield);
  return value;
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
  b.value = read_hex(is);
  return is;
}

std::istream& operator>>(std::istream& is, Address& a) {
  a.value = read_hex(is);
  return is;
}

}  // namespace z2music
