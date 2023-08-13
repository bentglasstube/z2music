#ifndef Z2MUSIC_PITCH
#define Z2MUSIC_PITCH

#include <array>
#include <cmath>
#include <map>
#include <string>

#include "util.h"

namespace z2music {

class Pitch {
 public:
  Pitch() : timer(0) {}
  Pitch(WordBE timer) : timer(timer) {}

  WordBE timer;
  std::string to_string() const;

  bool operator==(Pitch other) const { return timer == other.timer; }
  bool operator<(Pitch other) const { return timer > other.timer; }
  bool operator>(Pitch other) const { return timer < other.timer; }
  bool operator<=(Pitch other) const { return timer >= other.timer; }
  bool operator>=(Pitch other) const { return timer <= other.timer; }

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

  float freq() const { return kCPURate / (16.0f * (timer + 1)); }
  static Pitch from_freq(float freq) {
    int timer = static_cast<WordBE>(std::round(kCPURate / (16 * freq) - 1));
    return Pitch(timer);
  }

  int midi() const {
    return Midi::A4 +
           static_cast<int>(std::round(12 * log(freq() / kFreqA4) / kLog2));
  }
  static Pitch from_midi(int midi) {
    float freq = pow(2.f, (midi - Midi::A4) / 12.f) * 440.f;
    return from_freq(freq);
  }

  static Pitch none() { return Pitch(0); }

 private:
  static constexpr float kLog2 = std::log(2.f);
  static constexpr float kFreqA4 = 440.f;
  static constexpr int kMidiA4 = 69;
  static constexpr int kCPURate = 1789773;
  static const std::array<std::string, 12> kStepNames;
};

std::ostream& operator<<(std::ostream& os, Pitch p);

}  // namespace z2music

#endif  // define Z2MUSIC_PITCH
