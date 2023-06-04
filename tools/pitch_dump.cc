#include <iostream>

#include "pitch.h"
#include "rom.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <rom>" << std::endl;
    return 1;
  }

  z2music::Rom rom(argv[1]);
  auto lut = rom.pitch_lut();

  for (size_t i = 0x00; i < 0x7a; i += 2) {
    if (i % 0x10 == 0) {
      std::cout << std::hex << std::setfill('0') << std::setw(2) << i;
    }

    std::cout << std::setfill(' ') << std::setw(6) << lut->at(i).to_string();

    if (i % 0x10 == 0x0e) std::cout << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
