#ifndef Z2MUSIC_SFX_NOTES_H_
#define Z2MUSIC_SFX_NOTES_H_

#include <vector>

#include "pitch.h"
#include "util.h"

namespace z2music {
class SFXNotes {
 public:
  SFXNotes(Address address, std::vector<Pitch> notes)
      : address_(address), notes_(std::move(notes)) {}

  Address address() const { return address_; }
  std::vector<Pitch>::const_iterator begin() const { return notes_.begin(); }
  std::vector<Pitch>::const_iterator end() const { return notes_.end(); }
  Pitch operator[](size_t index) const { return notes_[index]; }
  size_t size() const { return notes_.size(); }

 private:
  const Address address_;
  const std::vector<Pitch> notes_;
};

}  // namespace z2music

#endif  //  Z2MUSIC_SFX_NOTES_H_

