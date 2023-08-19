#include "fake_rom.h"

#include "pitch.h"
#include "util.h"

namespace z2music {

FakeRom::FakeRom() {
  std::array<Pitch::Midi, 60> pitches = {
      Pitch::C3,  Pitch::E3,  Pitch::G3,  Pitch::Gs3, Pitch::A3,  Pitch::As3,
      Pitch::B3,  Pitch::C4,  Pitch::Cs4, Pitch::D4,  Pitch::Ds4, Pitch::E4,
      Pitch::F4,  Pitch::Fs4, Pitch::G4,  Pitch::Gs4, Pitch::A4,  Pitch::As4,
      Pitch::B4,  Pitch::C5,  Pitch::Cs5, Pitch::D5,  Pitch::Ds5, Pitch::E5,
      Pitch::F5,  Pitch::Fs5, Pitch::G5,  Pitch::A5,  Pitch::As5, Pitch::B5,
      Pitch::Cs3, Pitch::D3,  Pitch::Ds3, Pitch::F3,  Pitch::Fs3, Pitch::Gs5,
      Pitch::C6,  Pitch::Cs6, Pitch::D6,  Pitch::Ds6, Pitch::E6,  Pitch::F6,
      Pitch::Fs6, Pitch::G6,  Pitch::Gs6, Pitch::A6,  Pitch::As6, Pitch::B6,
      Pitch::C7,  Pitch::Cs7, Pitch::D7,  Pitch::Ds7, Pitch::E7,  Pitch::F7,
      Pitch::Fs7, Pitch::G7,  Pitch::Gs7, Pitch::A7,  Pitch::As7, Pitch::B7};
  for (auto const p : pitches) {
    pitch_lut().add_pitch(Pitch(p));
  }

  duration_lut().add_row({0x04, 0x0c, 0x08, 0x10, 0x18, 0x20, 0x05, 0x06});
  duration_lut().add_row({0x04, 0x0f, 0x09, 0x12, 0x1b, 0x24, 0x06, 0x06});
  duration_lut().add_row({0x05, 0x0f, 0x0a, 0x14, 0x1e, 0x28, 0x07, 0x06});
  duration_lut().add_row({0x06, 0x12, 0x0c, 0x18, 0x24, 0x30, 0x08, 0x10});
  duration_lut().add_row({0x07, 0x15, 0x0e, 0x1c, 0x2a, 0x38, 0x13, 0x12});
  duration_lut().add_row({0x07, 0x15, 0x0e, 0x1c, 0x2a, 0x38, 0x09, 0x0a});

  title_pitch_lut().add_pitch(Pitch::none());
  for (int p = Pitch::C2; p <= Pitch::Cs7; ++p) {
    title_pitch_lut().add_pitch(Pitch(static_cast<Pitch::Midi>(p)));
  }

  title_duration_lut().add_row({8, 24, 16, 32, 48, 64, 96, 128, 11, 10, 80});
}

void FakeRom::add_pattern(Address address, byte tempo,
                          const std::vector<byte>& data) {
  Address offset = address + (tempo == 0 ? 8 : 6);
  putc(address + 0x10000, tempo);
  putw(address + 0x10001, static_cast<WordLE>(offset));
  putc(address + 0x10003, 0x00);
  putc(address + 0x10004, 0x00);
  putc(address + 0x10005, 0x00);

  if (tempo == 0) {
    putc(address + 0x10004, 0x00);
    putc(address + 0x10005, 0x00);
  }

  write(0x10000 + offset, data);
}

}  // namespace z2music
