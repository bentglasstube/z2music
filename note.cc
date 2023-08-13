#include "note.h"

#include "absl/log/log.h"

namespace z2music {

namespace {
Note::Duration from_ticks(int ticks) {
  switch (ticks) {
    case 96 / 4:
      return Note::Duration::Sixteenth;
    case 96 * 3 / 2:
      return Note::Duration::DottedQuarter;
    case 96 / 2 * 3 / 2:
      return Note::Duration::DottedEighth;
    case 96 * 2:
      return Note::Duration::Half;
    case 96 / 2:
      return Note::Duration::Eighth;
    case 96 / 2 * 2 / 3:
      return Note::Duration::EighthTriplet;
    case 96:
      return Note::Duration::Quarter;
    case 96 * 2 / 3:
      return Note::Duration::QuarterTriplet;
    default:
      return Note::Duration::Unknown;
  }
}
}  // namespace

Note::Note(Pitch pitch, int ticks)
    : pitch_(pitch), duration_(from_ticks(ticks)) {}

int Note::length() const {
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

std::string Note::duration_string() const {
  switch (duration_) {
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
  }
  return "??";
}

}  // namespace z2music
