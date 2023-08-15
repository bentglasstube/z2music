#include "note.h"

#include "absl/log/log.h"

namespace z2music {

std::string Note::duration_string() const {
  if (ticks_ % Duration::Sixteenth == 0) {
    return std::to_string(ticks_ / Duration::Sixteenth);
  } else {
    return "t" + std::to_string(ticks_ / Duration::EighthTriplet * 2);
  }
}

std::ostream& operator<<(std::ostream& os, Note n) {
  return os << n.to_string();
}

}  // namespace z2music
