#include "note.h"

#include "absl/log/log.h"

namespace z2music {

std::string Note::duration_string() const {
  if (ticks_ % Duration::Sixteenth == 0) {
    return std::to_string(ticks_ / Duration::Sixteenth);
  } else {
    int length =
        std::round(ticks_ * 2 / static_cast<float>(Duration::EighthTriplet));
    return std::to_string(length) + "t";
  }
}

std::ostream& operator<<(std::ostream& os, Note n) {
  return os << n.to_string();
}

}  // namespace z2music
