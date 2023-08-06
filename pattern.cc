#include "pattern.h"

#include <sstream>

#include "absl/log/log.h"
#include "rom.h"

namespace z2music {

Pattern::Pattern() : tempo_(0x18) { clear(); }

Pattern::Pattern(const Rom& rom, Address address) {
  clear();

  byte header[6];
  rom.read(header, address, 6);

  tempo_ = header[0];

  if (tempo_ == 0x00) {
    voice1_ = rom.getc(address + 6);
    voice2_ = rom.getc(address + 7);
  }

  Address note_base = (header[2] << 8) + header[1] + 0x10000;

  read_notes(Channel::Pulse1, rom, note_base);

  if (header[3] > 0) read_notes(Channel::Triangle, rom, note_base + header[3]);
  if (header[4] > 0) read_notes(Channel::Pulse2, rom, note_base + header[4]);
  if (header[5] > 0) read_notes(Channel::Noise, rom, note_base + header[5]);
}

Pattern::Pattern(byte tempo, std::vector<Note> pw1, std::vector<Note> pw2,
                 std::vector<Note> triangle, std::vector<Note> noise)
    : tempo_(tempo) {
  clear();
  add_notes(Channel::Pulse1, pw1);
  add_notes(Channel::Pulse2, pw2);
  add_notes(Channel::Triangle, triangle);
  add_notes(Channel::Noise, noise);
}

Pattern::Pattern(byte v1, byte v2, std::vector<Note> pw1, std::vector<Note> pw2,
                 std::vector<Note> triangle, std::vector<Note> noise)
    : tempo_(0x00), voice1_(v1), voice2_(v2) {
  clear();
  add_notes(Channel::Pulse1, pw1);
  add_notes(Channel::Pulse2, pw2);
  add_notes(Channel::Triangle, triangle);
  add_notes(Channel::Noise, noise);
}

size_t Pattern::length() const { return length(Channel::Pulse1); }

void Pattern::add_notes(Pattern::Channel ch, std::vector<Note> notes) {
  for (auto n : notes) {
    notes_[ch].push_back(n);
  }
}

void Pattern::clear() {
  notes_[Channel::Pulse1].clear();
  notes_[Channel::Pulse2].clear();
  notes_[Channel::Triangle].clear();
  notes_[Channel::Noise].clear();
}

std::vector<Note> Pattern::notes(Pattern::Channel ch) const {
  return notes_.at(ch);
}

bool Pattern::validate() const {
  // TODO validate pattern

  // check that pw1 length is <= 16 quarter notes
  // check that other lengths are equal or valid partial lengths
  // TODO does the game handle unusual lengths?  maybe this isn't really
  // needed

  return true;
}

void Pattern::set_voicing(byte v1, byte v2) {
  tempo_ = 0x00;
  voice1_ = v1;
  voice2_ = v2;
}

std::vector<byte> Pattern::note_data() const {
  std::vector<byte> b;

  const std::array<Channel, 4> channels = {
      Channel::Pulse1,
      Channel::Pulse2,
      Channel::Triangle,
      Channel::Noise,
  };

  for (auto ch : channels) {
    const std::vector<byte> c = note_data(ch);
    b.insert(b.end(), c.begin(), c.end());
  }

  return b;
}

std::vector<byte> Pattern::meta_data(Address pw1_address) const {
  // FIXME calculate which channels need extra bytes :(
  const size_t pw1 = note_data_length(Channel::Pulse1);
  const size_t pw2 = note_data_length(Channel::Pulse2);
  const size_t tri = note_data_length(Channel::Triangle);
  const size_t noi = note_data_length(Channel::Noise);

  std::vector<byte> b;
  b.reserve(metadata_length());

  b.push_back(tempo_);
  b.push_back(pw1_address % 256);
  b.push_back(pw1_address >> 8);
  b.push_back(tri == 0 ? 0 : pw1 + pw2);
  b.push_back(pw2 == 0 ? 0 : pw1);
  b.push_back(noi == 0 ? 0 : pw1 + pw2 + tri);

  if (voiced()) {
    b.push_back(voice1_);
    b.push_back(voice2_);
  }

  return b;
}

size_t Pattern::length(Pattern::Channel ch) const {
  size_t length = 0;
  for (auto n : notes_.at(ch)) {
    length += n.length();
  }
  return length;
}

bool Pattern::pad_note_data(Pattern::Channel ch) const {
  if (ch == Channel::Pulse1) return true;

  const size_t l = length(ch);
  return l > 0 && l < length();
}

std::vector<byte> Pattern::note_data(Pattern::Channel ch) const {
  std::vector<byte> b;
  b.reserve(notes_.at(ch).size() + 1);

  for (auto n : notes_.at(ch)) {
    b.push_back(n);
  }
  if (pad_note_data(ch)) b.push_back(0);

  return b;
}

size_t Pattern::note_data_length(Pattern::Channel ch) const {
  return notes_.at(ch).size() + (pad_note_data(ch) ? 1 : 0);
}

void Pattern::read_notes(Pattern::Channel ch, const Rom& rom, Address address) {
  const size_t max_length = ch == Channel::Pulse1 ? 64 * 96 : length();
  size_t length = 0;

  while (length < max_length) {
    Note n = Note(rom.getc(address++));
    // Note data can terminate early on 00 byte
    if (n == 0x00) break;

    length += n.length();
    add_notes(ch, {n});

    // FIXME This is all wrong, the duration comes from a LUT which has a number
    // of ticks for each duration value.  Triplets are encoded using two
    // different indices in the LUT because the tick length they divide is even
    // (and thus not divisble by three).  This whole thing needs to be reworked
    // to use the duration LUT.
    //
    // The QuarterTriplet duration has special meaning when preceeded by
    // two EighthTriplets, which differs based on a tempo flag.
    if (n.duration() == Note::Duration::QuarterTriplet) {
      const size_t i = notes_[ch].size() - 3;
      if (notes_[ch][i + 0].duration() == Note::Duration::EighthTriplet &&
          notes_[ch][i + 1].duration() == Note::Duration::EighthTriplet) {
        if (tempo_ & 0x08) {
          // If flag 0x08 is set, just count 0xc1 as a third EighthTriplet
          notes_[ch][i + 2].duration(Note::Duration::EighthTriplet);
        } else {
          // If flag 0x08 is not set, rewrite the whole sequence
          notes_[ch][i + 0].duration(Note::Duration::DottedEighth);
          notes_[ch][i + 1].duration(Note::Duration::DottedEighth);
          notes_[ch][i + 2].duration(Note::Duration::Eighth);
        }
      }
    }
  }
}

std::vector<Note> Pattern::parse_notes(const std::string& data, int transpose) {
  std::vector<Note> notes;

  int duration = 0;
  int pitch = 0;
  int octave = 0;
  bool triplet = false;

  size_t i = 0;

  while (i < data.length()) {
    const char c = data[i++];

    switch (c) {
      case 'C':
      case 'c':
        pitch = 1;
        break;

      case 'D':
      case 'd':
        pitch = 3;
        break;

      case 'E':
      case 'e':
        pitch = 5;
        break;

      case 'F':
      case 'f':
        pitch = 6;
        break;

      case 'G':
      case 'g':
        pitch = 8;
        break;

      case 'A':
      case 'a':
        pitch = 10;
        break;

      case 'B':
        pitch = 12;
        break;

      case 'b':
        // flat if pitch is already set, otherwise b note
        pitch = pitch == 0 ? 12 : pitch - 1;
        break;

      case '#':
      case 's':
        ++pitch;
        break;

      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        if (octave == 0) {
          octave = c - '0';
        } else {
          duration = c - '0';
        }
        break;

      case '.':
        triplet = false;
        break;

      case 't':
        triplet = true;
        break;

      case 'x':
        // snare drum (G#3)
        pitch = 9;
        octave = 3;
        break;

      case 'r':
      case '-':
        // rest
        pitch = -1;
        octave = -1;
        break;

      case ' ':
        if (pitch && octave && duration) {
          const int note = pitch > 0 ? pitch + 12 * octave + 11 + transpose : 0;
          const int ticks = (triplet ? 4 : 6) * duration;
          notes.push_back(z2music::Note::from_midi(note, ticks));

          // Keep duration for later notes, but reset pitch and octave
          pitch = 0;
          octave = 0;
        }
        break;

      default:
        LOG(WARNING) << "Unknown char '" << c << "' when parsing notes";
        break;
    }
  }

  // Add final note
  if (pitch && octave && duration) {
    const int note = pitch > 0 ? pitch + 12 * octave + 11 + transpose : 0;
    const int ticks = (triplet ? 4 : 6) * duration;
    notes.push_back(z2music::Note::from_midi(note, ticks));
  }

  return notes;
}

std::string Pattern::dump_notes(Channel ch) const {
  Note::Duration prev_dur = Note::Duration::Unknown;

  std::ostringstream output;
  for (const auto note : notes(ch)) {
    auto duration = note.duration();

    if (output.tellp() > 0) output << " ";
    output << note.pitch_string();

    if (duration != prev_dur) {
      output << "." << note.duration_string();
      prev_dur = duration;
    }
  }
  return output.str();
}

}  // namespace z2music
