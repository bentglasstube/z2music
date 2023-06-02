#ifndef Z2MUSIC_NOTE
#define Z2MUSIC_NOTE

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

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
  };

  enum class Pitch {
    Rest = 0x02,
    Cs3 = 0x3e,
    E3 = 0x04,
    G3 = 0x06,
    Db3 = 0x3e,
    Gs3 = 0x08,
    A3 = 0x0a,
    As3 = 0x0c,
    B3 = 0x0e,
    Ab3 = 0x08,
    Bb3 = 0x0c,
    C4 = 0x10,
    Cs4 = 0x12,
    D4 = 0x14,
    Ds4 = 0x16,
    Db4 = 0x12,
    Eb4 = 0x16,
    E4 = 0x18,
    F4 = 0x1a,
    Fs4 = 0x1c,
    G4 = 0x1e,
    Gb4 = 0x1c,
    Gs4 = 0x20,
    A4 = 0x22,
    As4 = 0x24,
    B4 = 0x26,
    Ab4 = 0x20,
    Bb4 = 0x24,
    C5 = 0x28,
    Cs5 = 0x2a,
    D5 = 0x2c,
    Ds5 = 0x2e,
    Db5 = 0x2a,
    Eb5 = 0x2e,
    E5 = 0x30,
    F5 = 0x32,
    Fs5 = 0x34,
    G5 = 0x36,
    Gb5 = 0x34,
    A5 = 0x38,
    As5 = 0x3a,
    B5 = 0x3c,
    Bb5 = 0x3a,
  };

  Note(uint8_t value);
  Note(Duration d, Pitch p);

  static Note from_midi(int note, int ticks);

  Duration duration() const;
  Pitch pitch() const;

  void duration(Duration d);
  void pitch(Pitch p);

  size_t length() const;
  std::string pitch_string() const;

  operator uint8_t() const;

 private:
  uint8_t value_;

  static const std::unordered_map<int, uint8_t> kMidiPitchMap;
  static const std::unordered_map<int, uint8_t> kMidiDurationMap;
};

// Convenience method for writing notes
inline uint8_t operator|(Note::Pitch pitch, Note::Duration duration) {
  return ((uint8_t)pitch & 0x3e) | ((uint8_t)duration & 0xc1);
}
inline uint8_t operator|(Note::Duration duration, Note::Pitch pitch) {
  return ((uint8_t)pitch & 0x3e) | ((uint8_t)duration & 0xc1);
}

}  // namespace z2music

#endif  // define Z2MUSIC_NOTE
