#include "pitch.h"

#include "absl/log/log.h"
#include "rom.h"

namespace z2music {

std::string Pitch::to_string() const {
  if (timer == 0) return "--";
  const int note = midi();
  const int octave = (note / 12) - 1;
  return kStepNames[note % 12] + std::to_string(octave);
}

PitchLUT::PitchLUT(const Rom& rom, Address address) {
  LOG(INFO) << "Reading pitch data from " << address;
  for (byte i = 0; i < 0x7a; i += byte(2)) {
    const WordBE timer = rom.getwr(address + i);
    table_[Pitch(timer)] = i;
    LOG(INFO) << "Value at offset " << i << ": " << timer;
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

const std::array<std::string, 12> Pitch::kStepNames = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

}  // namespace z2music
