#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "rom.h"

ABSL_FLAG(std::string, rom, "", "Path to the rom file to dump.");
ABSL_FLAG(std::optional<std::string>, song, std::nullopt,
          "Dump only the listed song.");

// Force desired output format for bytes
std::ostream& operator<<(std::ostream& os, uint8_t byte) {
  os << "0x" << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint16_t>(byte);
  return os;
}

const z2music::Song* get_song_by_name(const z2music::Rom& rom,
                                      const std::string& name) {
  auto song = rom.song(name);
  if (!song) LOG(FATAL) << "Unknown song name " << name;
  return song;
}

void dump_notes(const std::vector<z2music::Note>& notes) {
  z2music::Note::Duration prev_dur = z2music::Note::Duration::Unknown;
  for (const auto note : notes) {
    auto duration = note.duration();

    std::cout << note.pitch_string();

    if (duration != prev_dur) {
      std::cout << "." << note.duration_string();
      prev_dur = duration;
    }

    std::cout << " ";
  }
  std::cout << std::endl;
}

void dump_song(const std::string& title, const z2music::Song* song) {
  std::cout << "song " << title << std::endl;

  for (const auto& pattern : song->patterns()) {
    if (pattern.voiced()) {
      std::cout << "pattern " << pattern.voice1() << " " << pattern.voice2()
                << std::endl;
    } else {
      std::cout << "pattern " << pattern.tempo() << std::endl;
    }
    dump_notes(pattern.notes(z2music::Pattern::Channel::Pulse1));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Pulse2));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Triangle));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Noise));
  }

  if (!song->empty()) {
    std::cout << "sequence";
    for (size_t n : song->sequence()) {
      std::cout << " " << std::dec << (n + 1);
    }
    std::cout << std::endl << std::endl;
  }
}

int main(int argc, char** argv) {
  std::ostringstream usage;
  usage << "Dumps the music from a Zelda 2 ROM." << std::endl;
  usage << "Example usage:" << std::endl;
  usage << argv[0] << " --rom <rom> [--song <songname>]";
  absl::SetProgramUsageMessage(usage.str());

  auto args = absl::ParseCommandLine(argc, argv);
  const z2music::Rom rom(absl::GetFlag(FLAGS_rom));

  if (absl::GetFlag(FLAGS_song).has_value()) {
    const std::string title = absl::GetFlag(FLAGS_song).value();
    const z2music::Song* song = get_song_by_name(rom, title);
    dump_song(title, song);
  } else {
    dump_song("TitleIntro", rom.song(z2music::Rom::SongTitle::TitleIntro));
    dump_song("TitleThemeStart",
              rom.song(z2music::Rom::SongTitle::TitleThemeStart));
    dump_song("TitleThemeBuildup",
              rom.song(z2music::Rom::SongTitle::TitleThemeBuildup));
    dump_song("TitleThemeMain",
              rom.song(z2music::Rom::SongTitle::TitleThemeMain));
    dump_song("TitleThemeBreakdown",
              rom.song(z2music::Rom::SongTitle::TitleThemeBreakdown));

    dump_song("OverworldIntro",
              rom.song(z2music::Rom::SongTitle::OverworldIntro));
    dump_song("OverworldTheme",
              rom.song(z2music::Rom::SongTitle::OverworldTheme));
    dump_song("BattleTheme", rom.song(z2music::Rom::SongTitle::BattleTheme));
    dump_song("CaveItemFanfare",
              rom.song(z2music::Rom::SongTitle::CaveItemFanfare));

    dump_song("TownIntro", rom.song(z2music::Rom::SongTitle::TownIntro));
    dump_song("TownTheme", rom.song(z2music::Rom::SongTitle::TownTheme));
    dump_song("HouseTheme", rom.song(z2music::Rom::SongTitle::HouseTheme));
    dump_song("TownItemFanfare",
              rom.song(z2music::Rom::SongTitle::TownItemFanfare));

    dump_song("PalaceIntro", rom.song(z2music::Rom::SongTitle::PalaceIntro));
    dump_song("PalaceTheme", rom.song(z2music::Rom::SongTitle::PalaceTheme));
    dump_song("BossTheme", rom.song(z2music::Rom::SongTitle::BossTheme));
    dump_song("PalaceItemFanfare",
              rom.song(z2music::Rom::SongTitle::PalaceItemFanfare));
    dump_song("CrystalFanfare",
              rom.song(z2music::Rom::SongTitle::CrystalFanfare));

    dump_song("GreatPalaceIntro",
              rom.song(z2music::Rom::SongTitle::GreatPalaceIntro));
    dump_song("GreatPalaceTheme",
              rom.song(z2music::Rom::SongTitle::GreatPalaceTheme));
    dump_song("ZeldaTheme", rom.song(z2music::Rom::SongTitle::ZeldaTheme));
    dump_song("CreditsTheme", rom.song(z2music::Rom::SongTitle::CreditsTheme));
    dump_song("GreatPalaceItemFanfare",
              rom.song(z2music::Rom::SongTitle::GreatPalaceItemFanfare));
    dump_song("TriforceFanfare",
              rom.song(z2music::Rom::SongTitle::TriforceFanfare));
    dump_song("FinalBossTheme",
              rom.song(z2music::Rom::SongTitle::FinalBossTheme));
  }

  return 0;
}
