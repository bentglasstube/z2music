#include "note.h"

#include "absl/log/log.h"

namespace z2music {

namespace {

template <typename E>
constexpr uint8_t to_byte(E e) noexcept {
  return static_cast<std::uint8_t>(e);
}

}  // namespace

Note::Note(Duration d, Pitch p) : value_(to_byte(d) | to_byte(p)) {}

Note Note::from_midi(int note, int ticks) {
  if (kMidiPitchMap.count(note) == 0) {
    LOG(FATAL) << "Note " << note << " is not usable.";
  }

  if (kMidiDurationMap.count(ticks) == 0) {
    LOG(FATAL) << "Duration " << ticks << " is not usable";
  }

  return kMidiPitchMap.at(note) | kMidiDurationMap.at(ticks);
}

void Note::duration(Note::Duration d) {
  value_ = to_byte(d) | to_byte(pitch());
}

void Note::pitch(Note::Pitch p) { value_ = to_byte(duration()) | to_byte(p); }

size_t Note::length() const {
  // MIDI generally uses 96 ppqn, so we'll use the same
  switch (duration()) {
    case Duration::Sixteenth:
      return 96 / 4;
    case Duration::DottedQuarter:
      return 96 * 3 / 2;
    case Duration::DottedEighth:
      return 96 / 2 * 3 / 2;
    case Duration::Half:
      return 96 * 2;
    case Duration::Eighth:
      return 96 / 2;
    case Duration::EighthTriplet:
      return 96 / 2 * 2 / 3;
    case Duration::Quarter:
      return 96;
    case Duration::QuarterTriplet:
      return 96 * 2 / 3;

    default:
      return 0;
  }
}

std::string Note::pitch_string() const {
  switch (pitch()) {
    case Pitch::Rest:
      return "r";
    case Pitch::C3:
      return "C3";
    case Pitch::Cs3:
      return "C#3";
    case Pitch::E3:
      return "E3";
    case Pitch::G3:
      return "G3";
    case Pitch::Gs3:
      return "G#3";
    case Pitch::A3:
      return "A3";
    case Pitch::As3:
      return "A#3";
    case Pitch::B3:
      return "B3";
    case Pitch::C4:
      return "C4";
    case Pitch::Cs4:
      return "C#4";
    case Pitch::D4:
      return "D4";
    case Pitch::Ds4:
      return "D#4";
    case Pitch::E4:
      return "E4";
    case Pitch::F4:
      return "F4";
    case Pitch::Fs4:
      return "F#4";
    case Pitch::G4:
      return "G4";
    case Pitch::Gs4:
      return "G#4";
    case Pitch::A4:
      return "A4";
    case Pitch::As4:
      return "A#4";
    case Pitch::B4:
      return "B4";
    case Pitch::C5:
      return "C5";
    case Pitch::Cs5:
      return "C#5";
    case Pitch::D5:
      return "D5";
    case Pitch::Ds5:
      return "D#5";
    case Pitch::E5:
      return "E5";
    case Pitch::F5:
      return "F5";
    case Pitch::Fs5:
      return "F#5";
    case Pitch::G5:
      return "G5";
    case Pitch::A5:
      return "A5";
    case Pitch::As5:
      return "A#5";
    case Pitch::B5:
      return "B5";

    default:
      return "?" + std::to_string(static_cast<int>(pitch()));
  }
}

std::string Note::duration_string() const {
  switch (duration()) {
    case Duration::Sixteenth:
      return "1";
    case Duration::DottedQuarter:
      return "6";
    case Duration::DottedEighth:
      return "3";
    case Duration::Half:
      return "8";
    case Duration::Eighth:
      return "2";
    case Duration::EighthTriplet:
      return "t2";
    case Duration::Quarter:
      return "4";
    case Duration::QuarterTriplet:
      return "t4";
    default:
      return "?";
  }
}

Note::operator uint8_t() const { return value_; }

const std::unordered_map<int, uint8_t> Note::kMidiPitchMap = {
    {0, 0x02},  // special pitch value for rest
    {49, 0x3e}, {52, 0x04}, {55, 0x06}, {56, 0x08}, {57, 0x0a}, {58, 0x0c},
    {59, 0x0e}, {60, 0x10}, {61, 0x12}, {62, 0x14}, {63, 0x16}, {64, 0x18},
    {65, 0x1a}, {66, 0x1c}, {67, 0x1e}, {68, 0x20}, {69, 0x22}, {70, 0x24},
    {71, 0x26}, {72, 0x28}, {73, 0x2a}, {74, 0x2c}, {75, 0x2e}, {76, 0x30},
    {77, 0x32}, {78, 0x34}, {79, 0x36}, {81, 0x38}, {82, 0x3a}, {83, 0x3c},
};

const std::unordered_map<int, uint8_t> Note::kMidiDurationMap = {
    {6, 0x00},  {36, 0x01}, {18, 0x40}, {48, 0x41},
    {12, 0x80}, {8, 0x81},  {24, 0xc0}, {16, 0xc1},
};

}  // namespace z2music
