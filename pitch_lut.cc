#include "pitch_lut.h"

#include "absl/log/log.h"

namespace z2music {

Pitch PitchLUT::at(byte index) const {
  return index / 2 < table_.size() ? table_[index / 2] : Pitch::none();
}

byte PitchLUT::index_for(const Pitch& pitch) const {
  for (size_t i = 0; i < table_.size(); ++i) {
    if (table_[i] == pitch) return i * 2;
  }
  return 0;
}

bool PitchLUT::has_pitch(const Pitch& pitch) const {
  for (size_t i = 0; i < table_.size(); ++i) {
    if (table_[i] == pitch) return true;
  }
  return false;
}

byte PitchLUT::add_pitch(const Pitch& pitch) {
  table_.push_back(std::move(pitch));
  return table_.size() * 2 - 2;
}

}  // namespace z2music