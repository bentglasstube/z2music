#ifndef Z2MUSIC_NOTE
#define Z2MUSIC_NOTE

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

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

  enum class Pitch {
    Rest = 0x02,
    C3 = 0x00,
    Cs3 = 0x3e,
    Db3 = 0x3e,
    E3 = 0x04,
    G3 = 0x06,
    Gs3 = 0x08,
    Ab3 = 0x08,
    A3 = 0x0a,
    As3 = 0x0c,
    Bb3 = 0x0c,
    B3 = 0x0e,
    C4 = 0x10,
    Cs4 = 0x12,
    Db4 = 0x12,
    D4 = 0x14,
    Ds4 = 0x16,
    Eb4 = 0x16,
    E4 = 0x18,
    F4 = 0x1a,
    Fs4 = 0x1c,
    Gb4 = 0x1c,
    G4 = 0x1e,
    Gs4 = 0x20,
    Ab4 = 0x20,
    A4 = 0x22,
    As4 = 0x24,
    Bb4 = 0x24,
    B4 = 0x26,
    C5 = 0x28,
    Cs5 = 0x2a,
    Db5 = 0x2a,
    D5 = 0x2c,
    Ds5 = 0x2e,
    Eb5 = 0x2e,
    E5 = 0x30,
    F5 = 0x32,
    Fs5 = 0x34,
    Gb5 = 0x34,
    G5 = 0x36,
    A5 = 0x38,
    As5 = 0x3a,
    Bb5 = 0x3a,
    B5 = 0x3c,
  };

  Note(byte value) : value_(value) {}

  Note(Duration d, Pitch p);

  static Note from_midi(int note, int ticks);

  Duration duration() const { return static_cast<Duration>(value_ & 0xc1); }
  Pitch pitch() const { return static_cast<Pitch>(value_ & 0x3e); }

  void duration(Duration d);
  void pitch(Pitch p);

  int length() const;
  std::string pitch_string() const;
  std::string duration_string() const;

  operator byte() const;
  bool operator==(int other) const { return value_ == other; }

 private:
  byte value_;

  static const std::unordered_map<int, byte> kMidiPitchMap;
  static const std::unordered_map<int, byte> kMidiDurationMap;
};

// Convenience method for writing notes
inline byte operator|(Note::Pitch pitch, Note::Duration duration) {
  return (static_cast<uint8_t>(pitch) & 0x3e) |
         (static_cast<uint8_t>(duration) & 0xc1);
}
inline uint8_t operator|(Note::Duration duration, Note::Pitch pitch) {
  return pitch | duration;
}

}  // namespace z2music

#endif  // define Z2MUSIC_NOTE
