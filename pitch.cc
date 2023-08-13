#include "pitch.h"

#include <iostream>

namespace z2music {

std::string Pitch::to_string() const {
  if (timer_ == 0) return "r";
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
