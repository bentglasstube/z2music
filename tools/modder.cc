#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/log.h"

#include "music.h"

ABSL_FLAG(std::string, rom, "", "Path to the rom file to modify.");
ABSL_FLAG(std::string, output, "", "Path where modified rom should be saved.");

std::string read_line(std::istream& file) {
  std::string line;
  std::getline(file, line);
  return line;
}

z2music::Song* get_song_by_name(z2music::Rom& rom, const std::string& name) {
  if (name == "TitleIntro") return rom.song(z2music::Rom::SongTitle::TitleIntro);
  if (name == "TitleThemeStart") return rom.song(z2music::Rom::SongTitle::TitleThemeStart);
  if (name == "TitleThemeBuildup") return rom.song(z2music::Rom::SongTitle::TitleThemeBuildup);
  if (name == "TitleThemeMain") return rom.song(z2music::Rom::SongTitle::TitleThemeMain);
  if (name == "TitleThemeBreakdown") return rom.song(z2music::Rom::SongTitle::TitleThemeBreakdown);

  if (name == "OverworldIntro") return rom.song(z2music::Rom::SongTitle::OverworldIntro);
  if (name == "OverworldTheme") return rom.song(z2music::Rom::SongTitle::OverworldTheme);
  if (name == "BattleTheme") return rom.song(z2music::Rom::SongTitle::BattleTheme);
  if (name == "CaveItemFanfare") return rom.song(z2music::Rom::SongTitle::CaveItemFanfare);

  if (name == "TownIntro") return rom.song(z2music::Rom::SongTitle::TownIntro);
  if (name == "TownTheme") return rom.song(z2music::Rom::SongTitle::TownTheme);
  if (name == "HouseTheme") return rom.song(z2music::Rom::SongTitle::HouseTheme);
  if (name == "TownItemFanfare") return rom.song(z2music::Rom::SongTitle::TownItemFanfare);

  if (name == "PalaceIntro") return rom.song(z2music::Rom::SongTitle::PalaceIntro);
  if (name == "PalaceTheme") return rom.song(z2music::Rom::SongTitle::PalaceTheme);
  if (name == "BossTheme") return rom.song(z2music::Rom::SongTitle::BossTheme);
  if (name == "PalaceItemFanfare") return rom.song(z2music::Rom::SongTitle::PalaceItemFanfare);
  if (name == "CrystalFanfare") return rom.song(z2music::Rom::SongTitle::CrystalFanfare);

  if (name == "GreatPalaceIntro") return rom.song(z2music::Rom::SongTitle::GreatPalaceIntro);
  if (name == "GreatPalaceTheme") return rom.song(z2music::Rom::SongTitle::GreatPalaceTheme);
  if (name == "ZeldaTheme") return rom.song(z2music::Rom::SongTitle::ZeldaTheme);
  if (name == "CreditsTheme") return rom.song(z2music::Rom::SongTitle::CreditsTheme);
  if (name == "GreatPalaceItemFanfare") return rom.song(z2music::Rom::SongTitle::GreatPalaceItemFanfare);
  if (name == "TriforceFanfare") return rom.song(z2music::Rom::SongTitle::TriforceFanfare);
  if (name == "FinalBossTheme") return rom.song(z2music::Rom::SongTitle::FinalBossTheme);

  LOG(FATAL) << "Unknown song name " << name;

  return nullptr;
}

void process_modfile(z2music::Rom& rom, std::istream& file) {
  int transpose = 0;
  size_t patterns = 0;
  bool sequenced = false;
  z2music::Song* song;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream input(line);
    std::string command;

    if (!(input >> command)) {
      LOG(FATAL) << "Could not parse line: " << line;
    }

    if (command == "song ") {
      std::string name;
      if (!(input >> name)) {
        LOG(FATAL) << "Song requires name";
      }

      if (song && !sequenced) {
        LOG(WARNING) << "Song changed without setting sequence";
      }
      song = get_song_by_name(rom, name);
      song->clear();
      transpose = 0;
      patterns = 0;
      sequenced = false;

    } else if (command == "transpose") {
      if (!(input >> transpose)) {
        LOG(FATAL) << "Transpose requires offset";
      }

    } else if (command == "pattern") {
      if (sequenced) {
        LOG(WARNING) << "Song already sequenced";
      }

      uint8_t tempo;
      if (!(input >> std::hex >> tempo)) {
        LOG(FATAL) << "Pattern requires tempo";
      }

      song->add_pattern({
          tempo,
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
          });

      ++patterns;

    } else if (command == "sequence") {
      if (sequenced) {
        LOG(WARNING) << "Song already sequenced";
      }

      size_t n;
      std::vector<size_t> sequence;
      while (input >> n) {
        if (n > patterns || n == 0) {
          LOG(ERROR) << "No such pattern: " << n << "";
        }
        sequence.push_back(n - 1);
        sequenced = true;
      }

    } else {
      LOG(WARNING) << "Unknown keyword '" << command << "'";
    }
  }

  if (song && !sequenced) {
    LOG(WARNING) << "Reached end of file with unsequenced song";
  }
}

int main(int argc, char** argv) {
  std::ostringstream usage;
  usage << "Modifies a Zelda 2 ROM file with music as indicated in the input file." << std::endl;
  usage << "Example usage:" << std::endl;
  usage << argv[0] << " <musicfile> --rom <rom> --output <output>";
  absl::SetProgramUsageMessage(usage.str());

  auto args = absl::ParseCommandLine(argc, argv);
  z2music::Rom rom(absl::GetFlag(FLAGS_rom));

  if (args.size() > 1) {
    std::ifstream file(args[1]);
    process_modfile(rom, file);
  } else {
    process_modfile(rom, std::cin);
  }

  rom.save(absl::GetFlag(FLAGS_output));
  return 0;
}