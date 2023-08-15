#include "duration_lut.h"

#include "absl/log/log.h"

namespace z2music {

byte DurationLUT::encode(int ticks, byte offset) {
  Row* row = get_row(offset);
  return row ? row->encode(ticks) : byte{0};
}

DurationLUT::Row* DurationLUT::get_row(byte offset) {
  size_t index = 0;
  while (true) {
    if (offset == 0) return &rows_[index];
    if (index >= rows_.size()) {
      LOG(ERROR) << "No row in duration LUT at offset " << offset;
      return nullptr;
    }
    offset -= rows_[index++].size();
  }
}

byte DurationLUT::Row::encode(int ticks) {
  float target = ticks * ratio() - error_;
  byte value = static_cast<byte>(std::round(target));
  error_ += (target - value);
  return index_for(value);
}

byte DurationLUT::Row::index_for(int value) const {
  for (size_t i = 0; i < values_.size(); ++i) {
    if (values_[i] == value) return i;
  }
  LOG(ERROR) << "Unable to find value " << value << " in Duration LUT row";
  return 0;
}

void DurationLUT::reset_error() {
  for (auto row : rows_) row.reset_error();
}

}  // namespace z2music
