#ifndef Z2MUSIC_PITCH
#define Z2MUSIC_PITCH

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

#include "rom.h"

namespace z2music {

class Pitch {
 public:
  Pitch(uint16_t timer) : timer(timer) {}

  uint16_t timer;
  float freq() const { return timer_to_freq(timer); }
  int midi() const {
    return kMidiA4 + std::round(12.f * log(freq() - kFreqA4) / log(2.f));
  }
  std::string to_string() const;

  /* bool operator==(Pitch other) const { return timer == other.timer; } */
  bool operator<(Pitch other) const { return timer < other.timer; }

  static Pitch from_freq(float freq) { return Pitch(freq_to_timer(freq)); }

  static uint16_t freq_to_timer(float freq) {
    return static_cast<uint16_t>(std::round(kCPURate / (16 * freq) - 1));
  }

  static float timer_to_freq(uint16_t timer) {
    return kCPURate / (16.0f * (timer + 1));
  }

  static Pitch from_midi(int midi);

  enum class Midi {};

 private:
  static constexpr float kLog2 = std::log(2.f);
  static constexpr float kFreqA4 = 440.f;
  static constexpr int kMidiA4 = 69;
  static constexpr int kCPURate = 1789773;
  static const std::array<std::string, 12> kStepNames;
};

class PitchLUT {
 public:
  PitchLUT() {}
  PitchLUT(const Rom& rom, size_t address);

  uint8_t value_for(Pitch pitch) const;
  bool has_pitch(Pitch pitch) const;

  uint16_t add_pitch(Pitch pitch);
  void clear();

 private:
  std::map<Pitch, uint8_t> table_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_PITCH
