#include "pitch_lut.h"

#include "absl/log/log.h"

namespace z2music {

Pitch PitchLUT::at(byte index) const {
  return index / 2 < table_.size() ? table_[index / 2] : Pitch::none();
}

byte PitchLUT::index_for(const Pitch& pitch) const {
  // hardcode the rest value because it has special handling
  if (pitch == Pitch::none()) return 2;
  for (size_t i = 0; i < table_.size(); ++i) {
    if (table_[i] == pitch) return i * 2;
  }
  LOG(ERROR) << "Unable to find pitch " << pitch << " in LUT";
  return 0;
}

byte PitchLUT::add_pitch(const Pitch& pitch) {
  table_.push_back(std::move(pitch));
  return table_.size() * 2 - 2;
}

}  // namespace z2music
