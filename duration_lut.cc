#include "duration_lut.h"

#include <sstream>

#include "absl/log/log.h"

namespace z2music {

byte DurationLUT::encode(int ticks, byte offset) {
  Row* row = get_row(offset);
  return row ? row->encode(ticks) : byte{0};
}

int DurationLUT::decode(byte b, byte offset) const {
  const Row* row = get_row(offset);
  return row ? row->decode(b) : 0;
}

void DurationLUT::reset() {
  for (auto& row : rows_) row.reset();
}

bool DurationLUT::has_error() const {
  for (const auto& row : rows_) {
    if (row.error() > 0) return true;
  }
  return false;
}

float DurationLUT::error() const {
  float error = 0;
  for (const auto& row : rows_) {
    error += row.error();
  }
  return error;
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

// TODO deduplicate this from above
const DurationLUT::Row* DurationLUT::get_row(byte offset) const {
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

int DurationLUT::Row::decode(byte index) const {
  float ratio = values_[index] / base();
  return std::round(ratio * Note::Duration::Sixteenth);
}

byte DurationLUT::Row::index_for(int value) const {
  for (size_t i = 0; i < values_.size(); ++i) {
    if (values_[i] == value) return i;
  }
  LOG(ERROR) << "Unable to find value " << value << " in Duration LUT row";
  return 0;
}

std::string DurationLUT::Row::to_string() const {
  std::ostringstream out;
  for (auto v : values_) {
    out << (out.tellp() > 0 ? " " : "") << v;
  }
  return out.str();
}

std::ostream& operator<<(std::ostream& os, const DurationLUT::Row& row) {
  return os << row.to_string();
}

}  // namespace z2music
