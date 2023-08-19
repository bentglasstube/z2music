#ifndef Z2MUSIC_FAKE_ROM_H_
#define Z2MUSIC_FAKE_ROM_H_

#include "rom.h"
#include "util.h"

namespace z2music {

class FakeRom : public Rom {
 public:
  FakeRom();

  void add_pattern(Address address, byte tempo, const std::vector<byte>& data);
  Pattern read_pattern(Address address) {
    return Rom::read_pattern(0x10000 + address);
  }
};

}  // namespace z2music

#endif  // Z2MUSIC_FAKE_ROM_H_

