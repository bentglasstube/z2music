#include "rom.h"

#include <fstream>

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

    songs_[SongTitle::TitleIntro] = Song(*this, title_screen_table, 0);
    songs_[SongTitle::TitleThemeStart] = Song(*this, title_screen_table, 1);
    songs_[SongTitle::TitleThemeBuildup] = Song(*this, title_screen_table, 2);
    songs_[SongTitle::TitleThemeMain] = Song(*this, title_screen_table, 3);
    songs_[SongTitle::TitleThemeBreakdown] = Song(*this, title_screen_table, 4);

    songs_[SongTitle::OverworldIntro] = Song(*this, overworld_song_table, 0);
    songs_[SongTitle::OverworldTheme] = Song(*this, overworld_song_table, 1);
    songs_[SongTitle::BattleTheme] = Song(*this, overworld_song_table, 2);
    songs_[SongTitle::CaveItemFanfare] = Song(*this, overworld_song_table, 4);

    songs_[SongTitle::TownIntro] = Song(*this, town_song_table, 0);
    songs_[SongTitle::TownTheme] = Song(*this, town_song_table, 1);
    songs_[SongTitle::HouseTheme] = Song(*this, town_song_table, 2);
    songs_[SongTitle::TownItemFanfare] = Song(*this, town_song_table, 4);

    songs_[SongTitle::PalaceIntro] = Song(*this, palace_song_table, 0);
    songs_[SongTitle::PalaceTheme] = Song(*this, palace_song_table, 1);
    songs_[SongTitle::BossTheme] = Song(*this, palace_song_table, 3);
    songs_[SongTitle::PalaceItemFanfare] = Song(*this, palace_song_table, 4);
    songs_[SongTitle::CrystalFanfare] = Song(*this, palace_song_table, 6);

    songs_[SongTitle::GreatPalaceIntro] =
        Song(*this, great_palace_song_table, 0);
    songs_[SongTitle::GreatPalaceTheme] =
        Song(*this, great_palace_song_table, 1);
    songs_[SongTitle::ZeldaTheme] = Song(*this, great_palace_song_table, 2);
    songs_[SongTitle::CreditsTheme] = Song(*this, great_palace_song_table, 3);
    songs_[SongTitle::GreatPalaceItemFanfare] =
        Song(*this, great_palace_song_table, 4);
    songs_[SongTitle::TriforceFanfare] =
        Song(*this, great_palace_song_table, 5);
    songs_[SongTitle::FinalBossTheme] = Song(*this, great_palace_song_table, 6);

    credits_ = Credits(*this);
  }
}

uint8_t Rom::getc(size_t address) const {
  if (address > kRomSize) return 0xff;
  return data_[address];
}

uint16_t Rom::getw(size_t address) const {
  return getc(address) + (getc(address + 1) << 8);
}

void Rom::read(uint8_t* buffer, size_t address, size_t length) const {
  // Could use std::copy or std::memcpy but this handles out of range
  // addresses
  for (size_t i = 0; i < length; ++i) {
    buffer[i] = getc(address + i);
  }
}

void Rom::putc(size_t address, uint8_t data) {
  if (address > kRomSize) return;
  data_[address] = data;
}

void Rom::putw(size_t address, uint16_t data) {
  putc(address, data & 0xff);
  putc(address + 1, data >> 8);
}

void Rom::write(size_t address, std::vector<uint8_t> data) {
  for (size_t i = 0; i < data.size(); ++i) {
    putc(address + i, data[i]);
  }
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

  credits_.commit(*this);

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

void Rom::move_song_table(size_t loader_address, uint16_t base_address) {
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
    LOG(ERROR) << std::hex << std::setfill('0') << std::showbase
               << "Unsure what loader is at " << std::setw(6) << loader_address
               << ", need manual update";
  }

  const uint16_t old_base = getw(loader_address + 1);

  // Rewind a bit because there is a load before the main section
  loader_address -= 11;

  while (true) {
    const uint8_t byte = getc(loader_address);
    if (byte == 0xb9) {
      const uint16_t addr = getw(loader_address + 1);
      const uint16_t new_addr = base_address + addr - old_base;
      LOG(INFO) << std::hex << std::setfill('0') << std::noshowbase
                << "Found LDA, replacing " << std::setw(4) << addr << " with "
                << std::setw(4) << new_addr;
      putw(loader_address + 1, new_addr);
      loader_address += 3;
    } else if (byte == 0x4c) {
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

Song* Rom::song(Rom::SongTitle title) { return &songs_[title]; }

Credits* Rom::credits() { return &credits_; }

void Rom::commit(size_t address, std::vector<Rom::SongTitle> songs) {
  std::array<uint8_t, 8> table;

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

  uint8_t offset = 8;
  std::vector<uint8_t> offsets;
  offsets.reserve(8);

  // Calculate song offset table
  for (auto s : songs) {
    offsets.push_back(offset);
    LOG(INFO) << std::hex << std::setfill('0') << std::showbase
              << "Offset for next song: " << std::setw(2)
              << static_cast<uint32_t>(offset);
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

  const uint8_t first_pattern = offset + 1;
  uint8_t seq_offset = 8;
  uint8_t pat_offset = first_pattern;

  for (auto s : songs) {
    const auto& song = songs_.at(s);

    LOG(INFO) << std::hex << std::setfill('0') << std::showbase
              << "Writing seq at " << std::setw(2)
              << static_cast<uint32_t>(seq_offset) << " with pat at "
              << std::setw(2) << static_cast<uint32_t>(pat_offset);
    const std::vector<uint8_t> seq = song.sequence_data(pat_offset);
    write(address + seq_offset, seq);

    std::ostringstream output;

    output << std::hex << std::setfill('0') << std::showbase;
    for (auto b : seq)
      output << std::setw(2) << static_cast<uint32_t>(b) << " ";
    LOG(INFO) << output.str();

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

  size_t note_address = pat_offset + address;
  pat_offset = first_pattern;

  LOG(INFO) << std::hex << std::setfill('0') << std::showbase
            << "Note data to start at " << std::setw(6) << note_address;

  for (auto s : songs) {
    for (auto p : songs_.at(s).patterns()) {
      const std::vector<uint8_t> note_data = p.note_data();
      const std::vector<uint8_t> meta_data = p.meta_data(note_address);

      LOG(INFO) << std::hex << std::setfill('0') << std::showbase
                << "Pattern at " << std::setw(6) << (address + pat_offset)
                << ", notes at " << std::setw(6) << note_address;
      std::ostringstream output;
      output << std::hex << std::setfill('0') << std::noshowbase;
      for (size_t i = 0; i < meta_data.size(); i += 2) {
        output << std::setw(2) << static_cast<uint32_t>(meta_data[i])
               << std::setw(2) << static_cast<uint32_t>(meta_data[i + 1])
               << " ";
      }
      LOG(INFO) << output.str();

      write(address + pat_offset, meta_data);
      write(note_address, note_data);

      pat_offset += meta_data.size();
      note_address += note_data.size();
    }
  }
}

size_t Rom::get_song_table_address(size_t loader_address) const {
  // Ensure that we are seing an LDA $addr,y instruction
  assert(getc(loader_address) == 0xb9);

  // Add the bank offset to the address read
  const size_t addr = getw(loader_address + 1) + 0x10000;

  LOG(INFO) << std::hex << std::setfill('0') << "Got address " << std::showbase
            << std::setw(6) << addr << ", from LSA $" << std::noshowbase
            << std::setw(4) << (addr & 0xffff) << ",y at " << std::showbase
            << std::setw(6) << loader_address;
  return addr;
}

}  // namespace z2music