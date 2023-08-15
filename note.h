#ifndef Z2MUSIC_NOTE_H_
#define Z2MUSIC_NOTE_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "pitch.h"
#include "util.h"

namespace z2music {

class Note {
 public:
  static constexpr int kPPQN = 96;

  enum Duration {
    Sixteenth = kPPQN / 4,
    Eighth = kPPQN / 2,
    Quarter = kPPQN,
    Half = kPPQN * 2,
    Whole = kPPQN * 4,

    DottedEighth = kPPQN * 3 / 4,
    DottedQuarter = kPPQN * 3 / 2,

    EighthTriplet = kPPQN / 2 * 2 / 3,
    QuarterTriplet = kPPQN * 2 / 3,

    Unknown = -1,
  };

  Note(Pitch pitch, int ticks) : pitch_(pitch), ticks_(ticks) {}
  static Note rest(int ticks) { return Note{Pitch::none(), ticks}; }

  int ticks() const { return ticks_; }
  Pitch pitch() const { return pitch_; }

  std::string duration_string() const;

  std::string to_string() const {
    return pitch_.to_string() + "." + duration_string();
  }

  bool operator==(Note other) const {
    return pitch_ == other.pitch_ && ticks_ == other.ticks_;
  }

 private:
  Pitch pitch_;
  int ticks_;
};

std::ostream& operator<<(std::ostream& os, Note n);

}  // namespace z2music

#endif  // Z2MUSIC_NOTE_H_
