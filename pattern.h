#ifndef Z2MUSIC_PATTERN
#define Z2MUSIC_PATTERN

#include <string>
#include <unordered_map>
#include <vector>

#include "note.h"
#include "util.h"

namespace z2music {

class Rom;

class Pattern {
 public:
  enum class Channel { Pulse1, Pulse2, Triangle, Noise };

  Pattern();
  Pattern(const Rom& rom, Address address);
  Pattern(byte tempo, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);
  Pattern(byte v1, byte v2, std::vector<Note> pw1, std::vector<Note> pw2,
          std::vector<Note> triangle, std::vector<Note> noise);

  size_t length() const;

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

  std::vector<byte> note_data() const;
  std::vector<byte> meta_data(Address pw1_address) const;

  static std::vector<Note> parse_notes(const std::string& data,
                                       int transpose = 0);

 private:
  byte tempo_, voice1_, voice2_;
  std::unordered_map<Channel, std::vector<Note>> notes_;

  size_t length(Channel ch) const;
  bool pad_note_data(Channel ch) const;
  std::vector<byte> note_data(Channel ch) const;
  size_t note_data_length(Channel ch) const;

  void read_notes(Channel ch, const Rom& rom, Address address);

  friend class PatternTest_Constructed_Test;
};

}  // namespace z2music

#endif  // define Z2MUSIC_PATTERN
