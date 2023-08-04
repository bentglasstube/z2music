#include "credits.h"

#include "absl/log/log.h"
#include "rom.h"
#include "util.h"

namespace z2music {

namespace {

char z2_decode_(byte data) {
  switch (data) {
    case 0x07:
      return '!';
    case 0xce:
      return '/';
    case 0xcf:
      return '.';
    case 0xf4:
      return ' ';
    case 0xf5:
      return ' ';
  }

  if (data >= 0xd0 && data <= 0xd9) return data - 0xa0;
  if (data >= 0xda && data <= 0xf3) return data - 0x99;

  LOG(ERROR) << "Cannot decode byte '" << data << "'";

  return 0x00;
}

byte z2_encode_(char data) {
  switch (data) {
    case ' ':
      return 0xf4;
    case '.':
      return 0xcf;
    case '/':
      return 0xce;
    case '!':
      return 0x07;
  }

  if (data >= 0x30 && data <= 0x39) return data + 0xa0;
  if (data >= 0x41 && data <= 0x5a) return data + 0x99;
  if (data >= 0x61 && data <= 0x7a) return data + 0x79;

  LOG(ERROR) << "Cannot encode character '" << data << "'";

  return 0x00;
}

std::string parse_string_(const Rom& rom, Address address) {
  const byte flag = rom.getc(address);
  if (flag != 0x22) return "";

  const byte length = rom.getc(address + 2);

  std::string s = "";
  for (byte i = 0; i < length; ++i) {
    s.append(1, z2_decode_(rom.getc(address + i + 3)));
  }

  LOG(INFO) << "Found string at " << address << " - [" << s << "]";

  return s;
}

size_t write_string_(Rom& rom, Address address, const std::string& s) {
  byte length = s.length();
  rom.putc(address, length);
  for (byte i = 0; i < length; ++i) {
    rom.putc(address + i + 1, z2_encode_(s.at(i)));
  }
  return address + length + 1;
}

}  // namespace

Credits::Credits() {}

Credits::Credits(const Rom& rom) {
  for (size_t i = 0; i < kCreditsPages; ++i) {
    const size_t addr = kCreditsTableAddress + 4 * i;

    const size_t title = rom.getw(addr) + kCreditsBankOffset;
    const size_t names = rom.getw(addr + 2) + kCreditsBankOffset;

    Text t;
    t.title = parse_string_(rom, title);
    t.name1 = parse_string_(rom, names);
    t.name2 = parse_string_(rom, names + t.name1.length() + 3);

    credits_[i] = t;
  }
}

void Credits::set(byte page, const Credits::Text& text) {
  if (page < kCreditsPages) credits_[page] = text;
}

Credits::Text Credits::get(byte page) const {
  if (page < kCreditsPages) return credits_[page];
  return {"", "", ""};
}

void Credits::commit(Rom& rom) const {
  Address table = kCreditsTableAddress;
  Address data = kCreditsTableAddress + 4 * kCreditsPages;

  for (byte i = 0; i < kCreditsPages; ++i) {
    // Add entry for title
    if (credits_[i].title.length() > 0) {
      rom.putw(table, data - kCreditsBankOffset);
      rom.write(data, {0x22, 0x47});
      data = write_string_(rom, data + 2, credits_[i].title);
      rom.putc(data++, 0xff);
    } else {
      // optimization if the title is empty, just point to the previous title
      rom.putw(table, rom.getw(table - 4));
    }

    // Add entry for name1
    rom.putw(table + 2, data - kCreditsBankOffset);
    rom.write(data, {0x22, 0x8b});
    data = write_string_(rom, data + 2, credits_[i].name1);

    // Add entry for name2 if present
    if (credits_[i].name2.length() > 0) {
      rom.write(data, {0x22, 0xcb});
      data = write_string_(rom, data + 2, credits_[i].name2);
    }

    rom.putc(data++, 0xff);
    table += 4;
  }
}

}  // namespace z2music
