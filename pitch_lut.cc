#include "pitch_lut.h"

#include "absl/log/log.h"

namespace z2music {

namespace {
byte offset(size_t index) {
  return index == 0 ? byte{0} : static_cast<byte>(index * 2 + 2);
}
}  // namespace

Pitch PitchLUT::at(byte index) const {
  if (index == 2) return Pitch::none();
  const size_t i = index == 0 ? 0 : index / 2 - 1;
  if (i >= table_.size()) {
    LOG(WARNING) << "No pitch at index " << index;
    return Pitch::none();
  } else {
    return table_[i];
  }
}

byte PitchLUT::index_for(const Pitch& pitch) const {
  if (pitch == Pitch::none()) return 2;
  for (size_t i = 0; i < table_.size(); ++i) {
    if (table_[i] == pitch) return offset(i);
  }
  LOG(ERROR) << "Unable to find pitch " << pitch << " in LUT";
  return 2;
}

byte PitchLUT::add_pitch(Pitch pitch) {
  for (size_t i = 0; i < table_.size(); ++i) {
    if (pitch == table_[i]) return offset(i);
  }
  table_.push_back(std::move(pitch));
  return offset(table_.size() - 1);
}

}  // namespace z2music
