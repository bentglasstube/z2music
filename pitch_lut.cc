#include "pitch_lut.h"

#include "absl/log/log.h"
#include "rom.h"

namespace z2music {

PitchLUT::PitchLUT(const Rom& rom, Address address) {
  LOG(INFO) << "Reading pitch data from " << address;
  for (byte i = 0; i < 0x7a; i += byte(2)) {
    const WordBE timer = rom.getwr(address + i);
    const Pitch pitch{timer};
    table_[pitch] = i;
    LOG(INFO) << "Value at offset " << i << ": " << pitch;
  }
}

byte PitchLUT::index_for(Pitch pitch) const {
  auto it = table_.find(pitch);
  return it == table_.end() ? byte(0) : it->second;
}

Pitch PitchLUT::at(byte index) const {
  for (const auto t : table_) {
    if (t.second == index) return t.first;
  }
  return Pitch::none();
}

void PitchLUT::commit(Rom& rom, Address address) const {
  for (const auto t : table_) {
    rom.putwr(address + t.second, t.first.timer);
  }
  return;
}

}  // namespace z2music
