#include "pitch.h"

namespace z2music {

std::string Pitch::to_string() const {
  const int note = midi();
  const int octave = (note / 12) - 1;
  return kStepNames[note % 12] + std::to_string(octave);
}

PitchLUT::PitchLUT(const Rom& rom, size_t address) {
  for (size_t i = 0; i < 0x7a; i += 2) {
    const uint16_t timer = rom.getw(address + i);
    table_[Pitch(timer)] = i;
  }
}

uint8_t PitchLUT::value_for(Pitch pitch) const {
  auto it = table_.find(pitch);
  return it == table_.end() ? 0 : it->second;
}

const std::array<std::string, 12> Pitch::kStepNames = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

}  // namespace z2music
