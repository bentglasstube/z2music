#include "pitch.h"

#include "absl/log/log.h"

namespace z2music {

std::string Pitch::to_string() const {
  if (timer == 0) return "--";
  const int note = midi();
  const int octave = (note / 12) - 1;
  return kStepNames[note % 12] + std::to_string(octave);
}

std::ostream& operator<<(std::ostream& os, Pitch p) {
  return os << p.to_string();
}

const std::array<std::string, 12> Pitch::kStepNames = {
    "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

}  // namespace z2music
