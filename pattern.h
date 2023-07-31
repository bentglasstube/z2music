#ifndef Z2MUSIC_PATTERN
#define Z2MUSIC_PATTERN

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "note.h"

namespace z2music {

class Rom;

class Pattern {
 public:
  enum class Channel { Pulse1, Pulse2, Triangle, Noise };

  Pattern();
  Pattern(const Rom& rom, size_t address);
  Pattern(uint8_t tempo, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);
  Pattern(uint8_t v1, uint8_t v2, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);

  size_t length() const;

  void add_notes(Channel ch, std::vector<Note> notes);
  void clear();
  std::vector<Note> notes(Channel ch) const;

  // TODO figure out if the tempo values are meaningful
  void tempo(uint8_t tempo) { tempo_ = tempo; }
  uint8_t tempo() const { return tempo_; }

  bool validate() const;

  bool voiced() const { return tempo_ == 0x00; }
  uint8_t voice1() const { return voice1_; }
  uint8_t voice2() const { return voice2_; }

  void set_voicing(uint8_t v1, uint8_t v2);

  size_t metadata_length() const;

  std::vector<uint8_t> note_data() const;
  std::vector<uint8_t> meta_data(size_t pw1_address) const;

  static std::vector<Note> parse_notes(const std::string& data,
                                       int transpose = 0);

 private:
  uint8_t tempo_;
  std::unordered_map<Channel, std::vector<Note>> notes_;
  uint8_t voice1_, voice2_;

  size_t length(Channel ch) const;
  bool pad_note_data(Channel ch) const;
  std::vector<uint8_t> note_data(Channel ch) const;
  size_t note_data_length(Channel ch) const;

  void read_notes(Channel ch, const Rom& rom, size_t address);
};

}  // namespace z2music

#endif  // define Z2MUSIC_PATTERN
