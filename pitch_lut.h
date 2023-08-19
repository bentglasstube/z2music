#ifndef Z2MUSIC_PITCH_LUT_H_
#define Z2MUSIC_PITCH_LUT_H_

#include <vector>

#include "pitch.h"
#include "util.h"

namespace z2music {

class PitchLUT {
 public:
  PitchLUT() {}

  Pitch at(byte index) const;
  Pitch operator[](byte index) const { return at(index); }
  void clear() { table_.clear(); }
  size_t size() const { return table_.size() == 0 ? 0 : table_.size() + 1; }

  byte index_for(const Pitch& pitch) const;
  byte add_pitch(Pitch pitch);
  void add_pitches(const std::vector<Pitch>& pitches) {
    for (const auto& p : pitches) add_pitch(p);
  }

  static byte mask(byte b) { return b & 0b00111110; }

  std::vector<Pitch>::const_iterator begin() const { return table_.begin(); }
  std::vector<Pitch>::const_iterator end() const { return table_.end(); }

 private:
  std::vector<Pitch> table_;
};

}  // namespace z2music

#endif  // Z2MUSIC_PITCH_LUT_H_
