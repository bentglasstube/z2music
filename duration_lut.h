#ifndef Z2MUSIC_DURATION_LUT_H_
#define Z2MUSIC_DURATION_LUT_H_

#include <vector>

#include "note.h"
#include "util.h"

namespace z2music {

class DurationLUT {
 public:
  class Row {
   public:
    Row(byte base) : values_({base}), error_(0.f) {}
    Row(std::vector<byte> data) : values_(std::move(data)), error_(0.f) {}

    byte encode(int ticks);
    byte base() const { return values_[0]; }
    float ratio() const {
      return base() / static_cast<float>(Note::Duration::Sixteenth);
    }
    byte index_for(int ticks) const;
    size_t size() const { return values_.size(); }

    void reset_error() { error_ = 0.f; }
    void add_value(byte value) { values_.push_back(value); }

   private:
    std::vector<byte> values_;
    float error_;
  };

  DurationLUT() {}
  byte encode(int ticks, byte offset);
  void add_row(Row row) { rows_.push_back(std::move(row)); }
  void add_row(std::vector<byte> data) { rows_.emplace_back(std::move(data)); }
  void reset_error();

 private:
  std::vector<Row> rows_;

  Row* get_row(byte offset);
};

}  // namespace z2music

#endif  // Z2MUSIC_DURATION_LUT_H_
