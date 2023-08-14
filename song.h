#ifndef Z2MUSIC_SONG
#define Z2MUSIC_SONG

#include <vector>

#include "pattern.h"
#include "pitch.h"
#include "util.h"

namespace z2music {

class Song {
 public:
  Song();

  void add_pattern(const Pattern& pattern);
  void set_sequence(const std::vector<byte>& seq);
  void append_sequence(byte n);

  std::vector<byte> sequence_data(byte first) const;

  size_t sequence_length() const;
  size_t pattern_count() const;
  size_t metadata_length() const;

  void clear();
  bool empty() const { return patterns_.empty(); }

  std::vector<Pattern> patterns() { return patterns_; }
  const std::vector<Pattern> patterns() const { return patterns_; }

  Pattern* at(byte i);
  const Pattern* at(byte i) const;

  const std::vector<byte> sequence() const { return sequence_; }

  PitchSet pitches_used() const;

 private:
  std::vector<Pattern> patterns_;
  std::vector<byte> sequence_;
};

}  // namespace z2music

#endif  // define Z2MUSIC_SONG
