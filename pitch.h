#ifndef Z2MUSIC_PITCH
#define Z2MUSIC_PITCH

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

#include "absl/log/log.h"
#include "rom.h"

namespace z2music {

class Pitch {
 public:
  Pitch(uint16_t timer) : timer(timer) {}

  uint16_t timer;
  std::string to_string() const;

  bool operator==(Pitch other) const { return timer == other.timer; }
  bool operator<(Pitch other) const { return timer < other.timer; }
  bool operator>(Pitch other) const { return timer > other.timer; }
  bool operator<=(Pitch other) const { return timer <= other.timer; }
  bool operator>=(Pitch other) const { return timer >= other.timer; }

  float freq() const { return kCPURate / (16.0f * (timer + 1)); }
  static Pitch from_freq(float freq) {
    int timer = static_cast<uint16_t>(std::round(kCPURate / (16 * freq) - 1));
    LOG(INFO) << "Calculated timer for frequency " << freq << " as " << timer;
    return Pitch(timer);
  }

  int midi() const {
    return kMidiA4 + std::round(12 * log(freq() / kFreqA4) / kLog2);
  }
  static Pitch from_midi(int midi) {
    float freq = pow(2.f, (midi - kMidiA4) / 12.f) * 440.f;
    LOG(INFO) << "Calculated frequency for midi note " << midi << " as "
              << freq;
    return from_freq(freq);
  }

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
