#include "pattern.h"

#include <sstream>

#include "absl/log/log.h"

namespace z2music {

Pattern::Pattern() : tempo_(0x18) { clear(); }

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
  if (ch != Channel::Noise) return false;

  const size_t l = length(ch);
  return l > 0 && l < length();
}

size_t Pattern::note_data_length() const {
  return note_data_length(Channel::Pulse1) + note_data_length(Channel::Pulse2) +
         note_data_length(Channel::Triangle) + note_data_length(Channel::Noise);
}

size_t Pattern::note_data_length(Pattern::Channel ch) const {
  return notes_.at(ch).size() + (pad_note_data(ch) ? 1 : 0);
}

namespace {
Note build_note(int pitch, int octave, int duration, bool triplet,
                int transpose) {
  const int note = pitch > 0 ? pitch + 12 * octave + 11 + transpose : 0;
  const int ticks = (triplet ? 4 : 6) * 4 * duration;
  return Note(note == 0 ? Pitch::none() : Pitch(static_cast<Pitch::Midi>(note)),
              ticks);
}
}  // namespace

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
          notes.push_back(
              build_note(pitch, octave, duration, triplet, transpose));
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
    notes.push_back(build_note(pitch, octave, duration, triplet, transpose));
  }

  return notes;
}

std::string Pattern::dump_notes(Channel ch) const {
  Note::Duration prev_dur = Note::Duration::Unknown;

  std::ostringstream output;
  for (const auto note : notes(ch)) {
    auto duration = note.duration();

    if (output.tellp() > 0) output << " ";
    output << note.pitch();

    if (duration != prev_dur) {
      output << "." << note.duration_string();
      prev_dur = duration;
    }
  }
  return output.str();
}

PitchSet Pattern::pitches_used() const {
  PitchSet pitches;

  for (const auto& v : notes_) {
    for (const auto& n : v.second) {
      LOG(INFO) << "Found note " << n;
      pitches.insert(n.pitch());
    }
  }

  return pitches;
}

}  // namespace z2music
