#ifndef Z2MUSIC_PITCH_LUT
#define Z2MUSIC_PITCH_LUT

#include "pitch.h"
#include "util.h"

namespace z2music {

class Rom;

class PitchLUT {
 public:
  PitchLUT() {}
  PitchLUT(const Rom& rom, Address address);

  byte index_for(Pitch pitch) const;
  Pitch at(byte index) const;
  bool has_pitch(Pitch pitch) const;

  WordBE add_pitch(Pitch pitch);
  void clear();

  void commit(Rom& rom, Address address) const;

 private:
  std::map<Pitch, byte> table_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_PITCH_LUT
