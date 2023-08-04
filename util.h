#ifndef Z2MUSIC_UTIL
#define Z2MUSIC_UTIL

#include <cstdint>
#include <iostream>

namespace z2music {

template <typename T, typename>
struct StrongInt {
  constexpr StrongInt(T value = 0) : value(value) {}
  inline constexpr operator T() const { return value; }
  T operator++() { return ++value; }
  T operator++(int) { return value++; }
  T operator+=(T other) { return value += other; }
  T operator-=(T other) { return value -= other; }
  T value;
};

template <typename T, typename Name>
std::istream& operator>>(std::istream& is, StrongInt<T, Name> v);

typedef StrongInt<uint8_t, struct ByteTag> byte;
typedef StrongInt<uint16_t, struct BigEndianWordTag> WordBE;
typedef StrongInt<uint16_t, struct LittleEndianWordTag> WordLE;
typedef StrongInt<uint32_t, struct AddressTag> Address;

std::ostream& operator<<(std::ostream& os, const byte& b);
std::ostream& operator<<(std::ostream& os, const WordLE& word);
std::ostream& operator<<(std::ostream& os, const WordBE& word);
std::ostream& operator<<(std::ostream& os, const Address& a);

std::istream& operator>>(std::istream& is, byte& b);

}  // namespace z2music

template <typename T, typename Name>
struct std::hash<z2music::StrongInt<T, Name>> {
  std::size_t operator()(const z2music::StrongInt<T, Name>& n) const noexcept {
    return std::hash<T>{}(n.value);
  }
};

#endif  // define Z2MUSIC_UTIL
