#include "song.h"

#include "rom.h"

namespace z2music {

Song::Song() {}

Song::Song(const Rom& rom, size_t address, size_t entry) {
  if (entry > 7) return;

  uint8_t table[8];
  rom.read(table, address, 8);

  std::unordered_map<uint8_t, size_t> offset_map;
  std::vector<size_t> seq;
  size_t n = 0;

  for (size_t i = 0; true; ++i) {
    uint8_t offset = rom.getc(address + table[entry] + i);

    if (offset == 0) break;
    if (offset_map.find(offset) == offset_map.end()) {
      offset_map[offset] = n++;
      add_pattern(Pattern(rom, address + offset));
    }
    append_sequence(offset_map.at(offset));
  }
}

void Song::add_pattern(const Pattern& pattern) { patterns_.push_back(pattern); }

void Song::set_sequence(const std::vector<size_t>& seq) { sequence_ = seq; }

void Song::append_sequence(size_t n) { sequence_.push_back(n); }

std::vector<uint8_t> Song::sequence_data(uint8_t first) const {
  std::vector<uint8_t> b;
  b.reserve(sequence_.size() + 1);

  std::vector<uint8_t> offsets;
  offsets.reserve(sequence_.size());
  for (const auto& p : patterns_) {
    offsets.push_back(first);
    first += p.metadata_length();
  }

  for (size_t n : sequence_) {
    b.push_back(offsets[n]);
  }

  b.push_back(0);

  return b;
}

size_t Song::sequence_length() const { return sequence_.size(); }

size_t Song::pattern_count() const { return patterns_.size(); }

size_t Song::metadata_length() const {
  size_t length = sequence_length() + 1;
  for (const auto& p : patterns_) {
    length += p.metadata_length();
  }
  return length;
}

void Song::clear() {
  patterns_.clear();
  sequence_.clear();
}

Pattern* Song::at(size_t i) {
  if (i < 0 || i >= sequence_.size()) return nullptr;
  return &(patterns_.at(sequence_.at(i)));
}

const Pattern* Song::at(size_t i) const {
  if (i < 0 || i >= sequence_.size()) return nullptr;
  return &(patterns_.at(sequence_.at(i)));
}

}  // namespace z2music
