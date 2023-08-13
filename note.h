#ifndef Z2MUSIC_NOTE
#define Z2MUSIC_NOTE

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "pitch.h"
#include "util.h"

namespace z2music {

class Note {
 public:
  enum class Duration {
    Sixteenth = 0x00,
    DottedQuarter = 0x01,
    DottedEighth = 0x40,
    Half = 0x41,
    Eighth = 0x80,
    EighthTriplet = 0x81,
    Quarter = 0xc0,
    QuarterTriplet = 0xc1,
    Unknown = 0xff,
  };

  Note(Pitch pitch, Duration duration) : pitch_(pitch), duration_(duration) {}
  Note(Pitch pitch, int ticks);

  static Note rest(Duration duration) { return Note{Pitch::none(), duration}; }

  Duration duration() const { return duration_; }
  Pitch pitch() const { return pitch_; }

  std::string to_string() const {
    return pitch_.to_string() + "." + duration_string();
  }

  int length() const;
  bool operator==(Note other) const {
    return pitch_ == other.pitch_ && duration_ == other.duration_;
  }

  std::string duration_string() const;

 private:
  Pitch pitch_;
  Duration duration_;
};

std::ostream& operator<<(std::ostream& os, Note n);

}  // namespace z2music

#endif  // define Z2MUSIC_NOTE
