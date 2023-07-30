#include "pitch.h"

#include <iomanip>

#include "absl/log/log.h"
#include "rom.h"

namespace z2music {

std::string Pitch::to_string() const {
  if (timer == 0) return "--";
  const int note = midi();
  const int octave = (note / 12) - 1;
  return kStepNames[note % 12] + std::to_string(octave);
}

PitchLUT::PitchLUT(const Rom& rom, size_t address) {
  LOG(INFO) << std::hex << "Reading pitch data from " << std::setw(8)
            << std::setfill('0') << address;
  for (size_t i = 0; i < 0x7a; i += 2) {
    const uint16_t timer = rom.getwr(address + i);
    table_[Pitch(timer)] = i;
    LOG(INFO) << std::hex << "Value at offset " << std::setfill('0')
              << std::setw(4) << i << ": " << std::setfill('0') << std::setw(6)
              << timer;
  }
}

uint8_t PitchLUT::index_for(Pitch pitch) const {
  auto it = table_.find(pitch);
  return it == table_.end() ? 0 : it->second;
}

Pitch PitchLUT::at(uint8_t index) const {
  for (const auto t : table_) {
    if (t.second == index) return t.first;
  }
  return Pitch::none();
}

void PitchLUT::commit(Rom& rom, size_t address) const {
  for (const auto t : table_) {
    rom.putw(address + t.second, t.first.timer);
  }
  return;
}

const std::array<std::string, 12> Pitch::kStepNames = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

}  // namespace z2music
