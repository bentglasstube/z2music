#ifndef Z2MUSIC_PITCH_LUT
#define Z2MUSIC_PITCH_LUT

#include <vector>

#include "pitch.h"
#include "util.h"

namespace z2music {

class PitchLUT {
 public:
  PitchLUT(){};

  Pitch at(byte index) const;
  Pitch operator[](byte index) const { return at(index); }
  void clear() { table_.clear(); }
  size_t size() const { return table_.size(); }

  byte index_for(const Pitch& pitch) const;
  bool has_pitch(const Pitch& pitch) const;
  byte add_pitch(const Pitch& pitch);

  static byte mask(byte b) { return b & 0b00111110; }

  const Pitch* begin() const { return &table_[0]; }
  const Pitch* end() const { return &table_[table_.size()]; }

 private:
  std::vector<Pitch> table_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_PITCH_LUT
