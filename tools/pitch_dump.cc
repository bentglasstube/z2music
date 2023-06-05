#include <iostream>
#include <sstream>

#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "pitch.h"
#include "rom.h"

int main(int argc, char** argv) {
  std::ostringstream usage;
  usage << "Dumps the pitch LUT from the given rom." << std::endl;
  usage << "Example usage:" << std::endl;
  usage << argv[0] << " <rom>";
  absl::SetProgramUsageMessage(usage.str());

  auto args = absl::ParseCommandLine(argc, argv);
  z2music::Rom rom(args[1]);
  auto lut = rom.pitch_lut();

  std::cout << std::hex << "  ";
  for (size_t i = 0; i < 16; i += 2) {
    std::cout << "    " << std::setfill('0') << std::setw(2) << i;
  }
  std::cout << std::endl;
  for (size_t i = 0x00; i < 0x7a; i += 2) {
    if (i % 0x10 == 0) {
      std::cout << std::setfill('0') << std::setw(2) << i;
    }

    std::cout << std::setfill(' ') << std::setw(6) << lut->at(i).to_string();

    if (i % 0x10 == 0x0e) std::cout << std::endl;
  }

  std::cout << std::endl;

  return 0;
}
