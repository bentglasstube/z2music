#ifndef Z2MUSIC_PATTERN_H_
#define Z2MUSIC_PATTERN_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "note.h"
#include "pitch.h"
#include "util.h"

namespace z2music {

class Pattern {
 public:
  enum class Channel { Pulse1, Pulse2, Triangle, Noise };

  Pattern();
  Pattern(byte tempo, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);
  Pattern(byte v1, byte v2, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);

  size_t length() const { return length(Channel::Pulse1); }

  void add_notes(Channel ch, std::vector<Note> notes);
  void clear();
  std::vector<Note> notes(Channel ch) const;

  // TODO figure out if the tempo values are meaningful
  void tempo(byte tempo) { tempo_ = tempo; }
  byte tempo() const { return tempo_; }

  bool validate() const;

  bool voiced() const { return tempo_ == 0x00; }
  byte voice1() const { return voice1_; }
  byte voice2() const { return voice2_; }

  void set_voicing(byte v1, byte v2);

  size_t metadata_length() const { return voiced() ? 8 : 6; }

  std::vector<byte> meta_data(Address pw1_address) const;

  static std::vector<Note> parse_notes(const std::string& data,
                                       int transpose = 0);
  std::string dump_notes(Channel ch) const;

  bool pad_note_data(Channel ch) const;
  size_t note_data_length() const;
  size_t note_data_length(Channel ch) const;

  PitchSet pitches_used() const;

 private:
  byte tempo_, voice1_, voice2_;
  std::unordered_map<Channel, std::vector<Note>> notes_;

  size_t length(Channel ch) const;
};

}  // namespace z2music

#endif  // Z2MUSIC_PATTERN_H_
