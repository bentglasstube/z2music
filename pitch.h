#ifndef Z2MUSIC_PITCH
#define Z2MUSIC_PITCH

#include <array>
#include <cmath>
#include <map>
#include <set>
#include <string>

#include "util.h"

namespace z2music {

#define OCTAVE(octave)                                              \
  C##octave = (12 * octave + 12), Cs##octave = (12 * octave + 13),  \
  Db##octave = (12 * octave + 13), D##octave = (12 * octave + 14),  \
  Ds##octave = (12 * octave + 15), Eb##octave = (12 * octave + 15), \
  E##octave = (12 * octave + 16), F##octave = (12 * octave + 17),   \
  Fs##octave = (12 * octave + 18), Gb##octave = (12 * octave + 18), \
  G##octave = (12 * octave + 19), Gs##octave = (12 * octave + 20),  \
  Ab##octave = (12 * octave + 20), A##octave = (12 * octave + 21),  \
  As##octave = (12 * octave + 22), Bb##octave = (12 * octave + 22), \
  B##octave = (12 * octave + 23)

class Pitch {
 public:
  enum Midi {
    OCTAVE(0),
    OCTAVE(1),
    OCTAVE(2),
    OCTAVE(3),
    OCTAVE(4),
    OCTAVE(5),
    OCTAVE(6),
    OCTAVE(7),
    OCTAVE(8),
  };

  Pitch() : timer_(0) {}

  explicit Pitch(WordBE timer) : timer_(timer) {}
  explicit Pitch(float freq)
      : timer_(static_cast<WordBE>(std::round(kCPURate / (16 * freq) - 1))) {}
  explicit Pitch(Midi note)
      : Pitch(pow(2.f, (note - Midi::A4) / 12.f) * kFreqA4) {}

  std::string to_string() const;

  bool operator==(Pitch other) const { return timer_ == other.timer_; }
  bool operator<(Pitch other) const { return timer_ > other.timer_; }
  bool operator>(Pitch other) const { return timer_ < other.timer_; }
  bool operator<=(Pitch other) const { return timer_ >= other.timer_; }
  bool operator>=(Pitch other) const { return timer_ <= other.timer_; }

  WordBE timer() const { return timer_; }
  float freq() const { return kCPURate / (16.0f * (timer_ + 1)); }
  int midi() const {
    return Midi::A4 +
           static_cast<int>(std::round(12 * log(freq() / kFreqA4) / kLog2));
  }

  static Pitch none() { return Pitch(0); }

 private:
  static constexpr float kLog2 = std::log(2.f);
  static constexpr float kFreqA4 = 440.f;
  static constexpr int kMidiA4 = 69;
  static constexpr int kCPURate = 1789773;
  static const std::array<std::string, 12> kStepNames;

  WordBE timer_;
};

std::ostream& operator<<(std::ostream& os, Pitch p);

typedef std::set<Pitch> PitchSet;

}  // namespace z2music

#endif  // define Z2MUSIC_PITCH
