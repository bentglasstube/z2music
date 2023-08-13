#include "rom.h"

#include <fstream>
#include <iomanip>

#include "absl/log/log.h"

namespace z2music {

Rom::Rom(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (file.is_open()) {
    file.read(reinterpret_cast<char*>(&header_[0]), kHeaderSize);
    file.read(reinterpret_cast<char*>(&data_[0]), kRomSize);

    title_screen_table = get_song_table_address(kTitleScreenLoader);
    overworld_song_table = get_song_table_address(kOverworldLoader);
    town_song_table = get_song_table_address(kTownLoader);
    palace_song_table = get_song_table_address(kPalaceLoader);
    great_palace_song_table = get_song_table_address(kGreatPalaceLoader);

    songs_[SongTitle::TitleIntro] = read_song(title_screen_table, 0);
    songs_[SongTitle::TitleThemeStart] = read_song(title_screen_table, 1);
    songs_[SongTitle::TitleThemeBuildup] = read_song(title_screen_table, 2);
    songs_[SongTitle::TitleThemeMain] = read_song(title_screen_table, 3);
    songs_[SongTitle::TitleThemeBreakdown] = read_song(title_screen_table, 4);

    songs_[SongTitle::OverworldIntro] = read_song(overworld_song_table, 0);
    songs_[SongTitle::OverworldTheme] = read_song(overworld_song_table, 1);
    songs_[SongTitle::BattleTheme] = read_song(overworld_song_table, 2);
    songs_[SongTitle::CaveItemFanfare] = read_song(overworld_song_table, 4);

    songs_[SongTitle::TownIntro] = read_song(town_song_table, 0);
    songs_[SongTitle::TownTheme] = read_song(town_song_table, 1);
    songs_[SongTitle::HouseTheme] = read_song(town_song_table, 2);
    songs_[SongTitle::TownItemFanfare] = read_song(town_song_table, 4);

    songs_[SongTitle::PalaceIntro] = read_song(palace_song_table, 0);
    songs_[SongTitle::PalaceTheme] = read_song(palace_song_table, 1);
    songs_[SongTitle::BossTheme] = read_song(palace_song_table, 3);
    songs_[SongTitle::PalaceItemFanfare] = read_song(palace_song_table, 4);
    songs_[SongTitle::CrystalFanfare] = read_song(palace_song_table, 6);

    songs_[SongTitle::GreatPalaceIntro] = read_song(great_palace_song_table, 0);
    songs_[SongTitle::GreatPalaceTheme] = read_song(great_palace_song_table, 1);
    songs_[SongTitle::ZeldaTheme] = read_song(great_palace_song_table, 2);
    songs_[SongTitle::CreditsTheme] = read_song(great_palace_song_table, 3);
    songs_[SongTitle::GreatPalaceItemFanfare] =
        read_song(great_palace_song_table, 4);
    songs_[SongTitle::TriforceFanfare] = read_song(great_palace_song_table, 5);
    songs_[SongTitle::FinalBossTheme] = read_song(great_palace_song_table, 6);

    credits_ = read_credits(kCreditsTableAddress);
    pitch_lut_ = read_pitch_lut(kPitchLUTAddress);
  }
}

byte Rom::getc(Address address) const {
  if (address > kRomSize) return byte(0xff);
  return byte(data_[address]);
}

WordLE Rom::getw(Address address) const {
  return getc(address) + (getc(address + 1) << 8);
}

WordBE Rom::getwr(Address address) const {
  return (getc(address) << 8) + getc(address + 1);
}

void Rom::read(byte* buffer, Address address, size_t length) const {
  // Could use std::copy or std::memcpy but this handles out of range
  // addresses
  for (size_t i = 0; i < length; ++i) {
    buffer[i] = getc(address + i);
  }
}

void Rom::putc(Address address, byte data) {
  if (address > kRomSize) return;
  data_[address] = data;
}

void Rom::putw(Address address, WordLE data) {
  putc(address, data & 0xff);
  putc(address + 1, data >> 8);
}

void Rom::putwr(Address address, WordBE data) {
  putc(address, data >> 8);
  putc(address + 1, data & 0xff);
}

void Rom::write(Address address, std::vector<byte> data) {
  for (size_t i = 0; i < data.size(); ++i) {
    putc(address + i, data[i]);
  }
}

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
}  // namespace

std::string Rom::read_string(Address address) const {
  const byte flag = getc(address);
  if (flag != 0x22) return "";

  const byte length = getc(address + 2);

  std::string s = "";
  for (byte i = 0; i < length; ++i) {
    s.append(1, z2_decode_(getc(address + i + 3)));
  }

  LOG(INFO) << "Found string at " << address << " - [" << s << "]";

  return s;
}

Address Rom::write_string(Address address, const std::string& s) {
  byte length = s.length();
  putc(address, length);
  for (byte i = 0; i < length; ++i) {
    putc(address + i + 1, z2_encode_(s.at(i)));
  }
  return address + length + 1;
}

bool Rom::commit() {
  commit(title_screen_table,
         {SongTitle::TitleIntro, SongTitle::TitleThemeStart,
          SongTitle::TitleThemeBuildup, SongTitle::TitleThemeMain,
          SongTitle::TitleThemeBreakdown});

  commit(overworld_song_table,
         {SongTitle::OverworldIntro, SongTitle::OverworldTheme,
          SongTitle::BattleTheme, SongTitle::CaveItemFanfare});

  commit(town_song_table, {SongTitle::TownIntro, SongTitle::TownTheme,
                           SongTitle::HouseTheme, SongTitle::TownItemFanfare});

  commit(palace_song_table,
         {SongTitle::PalaceIntro, SongTitle::PalaceTheme, SongTitle::BossTheme,
          SongTitle::PalaceItemFanfare, SongTitle::CrystalFanfare});

  commit(great_palace_song_table,
         {SongTitle::GreatPalaceIntro, SongTitle::GreatPalaceTheme,
          SongTitle::ZeldaTheme, SongTitle::CreditsTheme,
          SongTitle::GreatPalaceItemFanfare, SongTitle::TriforceFanfare,
          SongTitle::FinalBossTheme});

  commit_credits(kCreditsTableAddress);
  commit_pitch_lut(kPitchLUTAddress);

  return true;
}

void Rom::save(const std::string& filename) {
  if (commit()) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
      file.write(reinterpret_cast<char*>(&header_[0]), kHeaderSize);
      file.write(reinterpret_cast<char*>(&data_[0]), kRomSize);
    }
  }
}

void Rom::move_song_table(Address loader_address, Address base_address) {
  if (loader_address == kTitleScreenLoader) {
    title_screen_table = base_address + 0x010000;
  } else if (loader_address == kOverworldLoader) {
    overworld_song_table = base_address + 0x010000;
  } else if (loader_address == kTownLoader) {
    town_song_table = base_address + 0x010000;
  } else if (loader_address == kPalaceLoader) {
    palace_song_table = base_address + 0x010000;
  } else if (loader_address == kGreatPalaceLoader) {
    great_palace_song_table = base_address + 0x010000;
  } else {
    LOG(ERROR) << "Unsure what loader is at " << loader_address
               << ", need manual update";
  }

  const WordLE old_base = getw(loader_address + 1);

  // Rewind a bit because there is a load before the main section
  loader_address -= 11;

  while (true) {
    const byte op = getc(loader_address);
    if (op == 0xb9) {
      const WordLE addr = getw(loader_address + 1);
      const WordLE new_addr = base_address + addr - old_base;
      LOG(INFO) << "Found LDA, replacing " << addr << " with " << new_addr;
      putw(loader_address + 1, new_addr);
      loader_address += 3;
    } else if (op == 0x4c) {
      LOG(INFO) << "Found JMP, done moving table";
      break;
    } else if (loader_address >= 0x19c74) {
      LOG(INFO) << "Got to music reset code, done moving table";
      break;
    } else {
      ++loader_address;
    }
  }
}

namespace {
std::string data_dump(const std::vector<z2music::byte>& data) {
  std::ostringstream output;
  output << std::hex << std::noshowbase << std::setfill('0');
  size_t i = 0;
  for (auto d : data) {
    if (++i % 2 == 1) output << ' ';
    output << std::setw(2) << static_cast<int>(d.value);
  }
  return output.str();
}
}  // namespace

void Rom::commit(Address address, std::vector<Rom::SongTitle> songs) {
  std::array<byte, 8> table;

  // TODO make these changeable.
  // This will require rearchitecting things so that there is a Score object
  // which is a list of 8 (possibly duplicate) songs.  For now, it's just
  // hardcode which songs are where in each table.
  if (address == title_screen_table) {
    table = {0, 1, 2, 3, 4, 5, 5, 5};
  } else if (address == overworld_song_table || address == town_song_table) {
    table = {0, 1, 2, 2, 3, 4, 4, 4};
  } else if (address == palace_song_table) {
    table = {0, 1, 1, 2, 3, 5, 4, 5};
  } else if (address == great_palace_song_table) {
    table = {0, 1, 2, 3, 4, 5, 6, 7};
  } else {
    return;
  }

  /**************
   * SONG TABLE *
   **************/

  byte offset = 8;
  std::vector<byte> offsets;
  offsets.reserve(8);

  // Calculate song offset table
  for (auto s : songs) {
    offsets.push_back(offset);
    LOG(INFO) << "Offset for next song: " << offset;
    offset += songs_.at(s).sequence_length() + 1;
  }

  // One extra offset for the "empty" song at the end
  // We could save a whole byte by pointing this at the end of some other
  // sequence but it's kind of nice to see the double 00 to mean the end of
  // the sequence data.
  offsets.push_back(offset);

  // Write song table to ROM
  for (size_t i = 0; i < 8; ++i) {
    putc(address + i, offsets[table[i]]);
  }

  /******************
   * SEQUENCE TABLE *
   ******************/

  const byte first_pattern = offset + 1;
  byte seq_offset = 8;
  byte pat_offset = first_pattern;

  for (auto s : songs) {
    const auto& song = songs_.at(s);

    LOG(INFO) << "Writing seq at " << seq_offset << " with pat at "
              << pat_offset;
    const std::vector<byte> seq = song.sequence_data(pat_offset);
    LOG(INFO) << "Sequence data: " << data_dump(seq);
    write(address + seq_offset, seq);

    for (size_t i = 0; i < song.pattern_count(); ++i) {
      pat_offset += song.at(i)->metadata_length();
    }

    seq_offset += seq.size();
  }

  // Write an empty sequence for the empty song
  putc(address + seq_offset, 0);

  /*******************************
   * PATTERN TABLE AND NOTE DATA *
   *******************************/

  Address note_address = pat_offset + address;
  pat_offset = first_pattern;

  for (auto s : songs) {
    for (auto p : songs_.at(s).patterns()) {
      const std::vector<byte> note_data = encode_pattern(p);
      const std::vector<byte> meta_data = p.meta_data(note_address);

      const z2music::Address meta_address = address + pat_offset;

      LOG(INFO) << "Metadata:  " << meta_address << " " << data_dump(meta_data);
      LOG(INFO) << "Note data: " << note_address << " " << data_dump(note_data);

      write(meta_address, meta_data);
      write(note_address, note_data);

      pat_offset += meta_data.size();
      note_address += note_data.size();
    }
  }
}

Rom::SongTitle Rom::title_by_name(const std::string& name) {
  if (name == "TitleIntro") return Rom::SongTitle::TitleIntro;
  if (name == "TitleThemeStart") return Rom::SongTitle::TitleThemeStart;
  if (name == "TitleThemeBuildup") return Rom::SongTitle::TitleThemeBuildup;
  if (name == "TitleThemeMain") return Rom::SongTitle::TitleThemeMain;
  if (name == "TitleThemeBreakdown") return Rom::SongTitle::TitleThemeBreakdown;

  if (name == "OverworldIntro") return Rom::SongTitle::OverworldIntro;
  if (name == "OverworldTheme") return Rom::SongTitle::OverworldTheme;
  if (name == "BattleTheme") return Rom::SongTitle::BattleTheme;
  if (name == "CaveItemFanfare") return Rom::SongTitle::CaveItemFanfare;

  if (name == "TownIntro") return Rom::SongTitle::TownIntro;
  if (name == "TownTheme") return Rom::SongTitle::TownTheme;
  if (name == "HouseTheme") return Rom::SongTitle::HouseTheme;
  if (name == "TownItemFanfare") return Rom::SongTitle::TownItemFanfare;

  if (name == "PalaceIntro") return Rom::SongTitle::PalaceIntro;
  if (name == "PalaceTheme") return Rom::SongTitle::PalaceTheme;
  if (name == "BossTheme") return Rom::SongTitle::BossTheme;
  if (name == "PalaceItemFanfare") return Rom::SongTitle::PalaceItemFanfare;
  if (name == "CrystalFanfare") return Rom::SongTitle::CrystalFanfare;

  if (name == "GreatPalaceIntro") return Rom::SongTitle::GreatPalaceIntro;
  if (name == "GreatPalaceTheme") return Rom::SongTitle::GreatPalaceTheme;
  if (name == "ZeldaTheme") return Rom::SongTitle::ZeldaTheme;
  if (name == "CreditsTheme") return Rom::SongTitle::CreditsTheme;
  if (name == "GreatPalaceItemFanfare")
    return Rom::SongTitle::GreatPalaceItemFanfare;
  if (name == "TriforceFanfare") return Rom::SongTitle::TriforceFanfare;
  if (name == "FinalBossTheme") return Rom::SongTitle::FinalBossTheme;

  return Rom::SongTitle::Unknown;
}

Address Rom::get_song_table_address(Address loader_address) const {
  // Ensure that we are seing an LDA $addr,y instruction
  assert(getc(loader_address) == 0xb9);

  // Add the bank offset to the address read
  const Address addr = getw(loader_address + 1) + 0x10000;

  LOG(INFO) << "Got address " << addr << ", from LSA $" << std::hex
            << std::setw(4) << std::setfill('0') << (addr & 0xffff) << ",y at "
            << loader_address;
  return addr;
}

PitchLUT Rom::read_pitch_lut(Address address) const {
  PitchLUT lut;
  LOG(INFO) << "Reading pitch data from " << address;
  for (byte i = 0; i < 0x7a; i += byte(2)) {
    const Pitch pitch{getwr(address + i)};
    lut.add_pitch(pitch);
    LOG(INFO) << "Value at offset " << i << ": " << pitch;
  }
  return lut;
}

Song Rom::read_song(Address address, byte entry) const {
  byte table[8];
  read(table, address, 8);

  std::unordered_map<byte, byte> offset_map;
  byte n = 0;

  Song song;

  for (byte i = 0; true; ++i) {
    byte offset = getc(address + table[entry] + i);

    if (offset == 0) break;
    if (offset_map.find(offset) == offset_map.end()) {
      offset_map[offset] = n++;
      song.add_pattern(read_pattern(address + offset));
    }
    song.append_sequence(offset_map.at(offset));
  }

  return song;
}

Pattern Rom::read_pattern(Address address) const {
  Pattern pattern;

  byte header[6];
  read(header, address, 6);

  pattern.tempo(header[0]);

  if (pattern.voiced()) {
    pattern.set_voicing(getc(address + 6), getc(address + 7));
  }

  Address note_base = (header[2] << 8) + header[1] + 0x10000;

  pattern.add_notes(Pattern::Channel::Pulse1, read_notes(note_base));
  size_t max_length = pattern.length();

  if (header[3])
    pattern.add_notes(Pattern::Channel::Triangle,
                      read_notes(note_base + header[3], max_length));
  if (header[4])
    pattern.add_notes(Pattern::Channel::Pulse2,
                      read_notes(note_base + header[4], max_length));
  if (header[5])
    pattern.add_notes(Pattern::Channel::Noise,
                      read_notes(note_base + header[5], max_length));

  return pattern;
}

std::vector<Note> Rom::read_notes(Address address, size_t max_length) const {
  size_t length = 0;
  std::vector<Note> notes;

  while (max_length == 0 || length < max_length) {
    const byte b = getc(address++);
    // FIXME only Pulse1 and Noise channels can be null terminated
    if (b == 0x00) break;
    const Note n = decode_note(b);
    length += n.length();
    notes.emplace_back(std::move(n));
  }

  return notes;
}

Credits Rom::read_credits(Address address) const {
  Credits credits;

  for (size_t i = 0; i < Credits::kPages; ++i) {
    const Address addr = address + 4 * i;

    const Address title = getw(addr) + kCreditsBankOffset;
    const Address names = getw(addr + 2) + kCreditsBankOffset;

    credits[i].title = read_string(title);
    credits[i].name1 = read_string(names);
    credits[i].name2 = read_string(names + credits[i].name1.length() + 3);
  }

  return credits;
}

Note Rom::decode_note(byte b) const {
  return {pitch_lut_[PitchLUT::mask(b)],
          static_cast<Note::Duration>(b & 0b11000001)};
}

void Rom::commit_pitch_lut(Address address) {
  for (byte i = 0; i < 0x7a; i += byte(2)) {
    putwr(address + i, pitch_lut_.at(i).timer());
  }
}

void Rom::commit_credits(Address address) {
  Address table = address;
  Address data = address + 4 * Credits::kPages;

  for (const auto& credit : credits_) {
    // Add entry for title
    if (credit.title.length() > 0) {
      putw(table, data - kCreditsBankOffset);
      write(data, {0x22, 0x47});
      data = write_string(data + 2, credit.title);
      putc(data++, 0xff);
    } else {
      // optimization if the title is empty, just point to the previous title
      putw(table, getw(table - 4));
    }

    // Add entry for name1
    putw(table + 2, data - kCreditsBankOffset);
    write(data, {0x22, 0x8b});
    data = write_string(data + 2, credit.name1);

    // Add entry for name2 if present
    if (credit.name2.length() > 0) {
      write(data, {0x22, 0xcb});
      data = write_string(data + 2, credit.name2);
    }

    putc(data++, 0xff);
    table += 4;
  }
}

std::vector<byte> Rom::encode_pattern(const Pattern& pattern) const {
  std::vector<byte> data;
  data.reserve(pattern.note_data_length());

  const std::array<Pattern::Channel, 4> channels = {
      Pattern::Channel::Pulse1,
      Pattern::Channel::Pulse2,
      Pattern::Channel::Triangle,
      Pattern::Channel::Noise,
  };

  for (auto ch : channels) {
    auto c = encode_note_data(pattern.notes(ch), pattern.pad_note_data(ch));
    data.insert(data.end(), c.begin(), c.end());
  }

  return data;
}

std::vector<byte> Rom::encode_note_data(const std::vector<Note>& notes,
                                        bool null_terminated) const {
  std::vector<byte> data;
  data.reserve(notes.size() + null_terminated ? 1 : 0);
  for (const auto n : notes) {
    data.push_back(encode_note(n));
  }
  if (null_terminated) data.push_back(0x00);
  return data;
}

byte Rom::encode_note(const Note& note) const {
  return pitch_lut_.index_for(note.pitch()) |
         static_cast<uint8_t>(note.duration());
}

}  // namespace z2music
