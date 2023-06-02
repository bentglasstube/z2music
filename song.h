#ifndef Z2MUSIC_SONG
#define Z2MUSIC_SONG

#include <cstddef>
#include <cstdint>
#include <vector>

#include "pattern.h"

namespace z2music {

class Rom;

class Song {
 public:
  Song();
  Song(const Rom& rom, size_t address, size_t entry);

  void add_pattern(const Pattern& pattern);
  void set_sequence(const std::vector<size_t>& seq);
  void append_sequence(size_t n);

  std::vector<uint8_t> sequence_data(uint8_t first) const;

  size_t sequence_length() const;
  size_t pattern_count() const;
  size_t metadata_length() const;

  void clear();

  std::vector<Pattern> patterns();

  Pattern* at(size_t i);
  const Pattern* at(size_t i) const;

 private:
  std::vector<Pattern> patterns_;
  std::vector<size_t> sequence_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_SONG
