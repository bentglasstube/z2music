#ifndef Z2MUSIC_DURATION_LUT_H_
#define Z2MUSIC_DURATION_LUT_H_

#include <iostream>
#include <string>
#include <vector>

#include "note.h"
#include "util.h"

namespace z2music {

class DurationLUT {
 public:
  class Row {
   public:
    Row(std::vector<byte> data) : values_(std::move(data)), error_(0.f) {}

    byte encode(int ticks);
    int decode(byte b) const;
    byte base() const { return values_[2]; }
    float ratio() const {
      return base() / static_cast<float>(Note::Duration::Eighth);
    }
    byte index_for(int ticks) const;
    size_t size() const { return values_.size(); }
    std::string to_string() const;

    void reset() { error_ = 0.f; }
    void add_value(byte value) { values_.push_back(value); }
    float error() const { return error_; }

   private:
    std::vector<byte> values_;
    float error_;
  };

  DurationLUT() {}
  byte encode(int ticks, byte offset);
  int decode(byte b, byte offset) const;
  void add_row(Row row) { rows_.push_back(std::move(row)); }
  void add_row(std::vector<byte> data) { rows_.emplace_back(std::move(data)); }
  void reset();
  bool has_error() const;
  float error() const;

  static byte shift(byte b) {
    return ((b & 0b11000000) >> 6) | ((b & 0b1) << 2);
  }
  static byte unshift(byte b) { return ((b & 0b11) << 6) | ((b & 0b100) >> 2); }

 private:
  std::vector<Row> rows_;

  static constexpr float kEpsilon = 1 / 96.f;

  Row* get_row(byte offset);
  const Row* get_row(byte offset) const;
};

std::ostream& operator<<(std::ostream& os, const DurationLUT::Row& row);

}  // namespace z2music

#endif  // Z2MUSIC_DURATION_LUT_H_
