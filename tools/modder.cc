#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/log.h"
#include "rom.h"

ABSL_FLAG(std::string, rom, "", "Path to the rom file to modify.");
ABSL_FLAG(std::string, output, "", "Path where modified rom should be saved.");

std::string read_line(std::istream& file) {
  std::string line;
  std::getline(file, line);
  return line;
}

z2music::Song* get_song_by_name(z2music::Rom& rom, const std::string& name) {
  if (name == "TitleIntro")
    return rom.song(z2music::Rom::SongTitle::TitleIntro);
  if (name == "TitleThemeStart")
    return rom.song(z2music::Rom::SongTitle::TitleThemeStart);
  if (name == "TitleThemeBuildup")
    return rom.song(z2music::Rom::SongTitle::TitleThemeBuildup);
  if (name == "TitleThemeMain")
    return rom.song(z2music::Rom::SongTitle::TitleThemeMain);
  if (name == "TitleThemeBreakdown")
    return rom.song(z2music::Rom::SongTitle::TitleThemeBreakdown);

  if (name == "OverworldIntro")
    return rom.song(z2music::Rom::SongTitle::OverworldIntro);
  if (name == "OverworldTheme")
    return rom.song(z2music::Rom::SongTitle::OverworldTheme);
  if (name == "BattleTheme")
    return rom.song(z2music::Rom::SongTitle::BattleTheme);
  if (name == "CaveItemFanfare")
    return rom.song(z2music::Rom::SongTitle::CaveItemFanfare);

  if (name == "TownIntro") return rom.song(z2music::Rom::SongTitle::TownIntro);
  if (name == "TownTheme") return rom.song(z2music::Rom::SongTitle::TownTheme);
  if (name == "HouseTheme")
    return rom.song(z2music::Rom::SongTitle::HouseTheme);
  if (name == "TownItemFanfare")
    return rom.song(z2music::Rom::SongTitle::TownItemFanfare);

  if (name == "PalaceIntro")
    return rom.song(z2music::Rom::SongTitle::PalaceIntro);
  if (name == "PalaceTheme")
    return rom.song(z2music::Rom::SongTitle::PalaceTheme);
  if (name == "BossTheme") return rom.song(z2music::Rom::SongTitle::BossTheme);
  if (name == "PalaceItemFanfare")
    return rom.song(z2music::Rom::SongTitle::PalaceItemFanfare);
  if (name == "CrystalFanfare")
    return rom.song(z2music::Rom::SongTitle::CrystalFanfare);

  if (name == "GreatPalaceIntro")
    return rom.song(z2music::Rom::SongTitle::GreatPalaceIntro);
  if (name == "GreatPalaceTheme")
    return rom.song(z2music::Rom::SongTitle::GreatPalaceTheme);
  if (name == "ZeldaTheme")
    return rom.song(z2music::Rom::SongTitle::ZeldaTheme);
  if (name == "CreditsTheme")
    return rom.song(z2music::Rom::SongTitle::CreditsTheme);
  if (name == "GreatPalaceItemFanfare")
    return rom.song(z2music::Rom::SongTitle::GreatPalaceItemFanfare);
  if (name == "TriforceFanfare")
    return rom.song(z2music::Rom::SongTitle::TriforceFanfare);
  if (name == "FinalBossTheme")
    return rom.song(z2music::Rom::SongTitle::FinalBossTheme);

  LOG(FATAL) << "Unknown song name " << name;

  return nullptr;
}

void process_modfile(z2music::Rom& rom, std::istream& file) {
  int transpose = 0;
  size_t patterns = 0;
  bool sequenced = false;
  z2music::Song* song = nullptr;

  std::string line;
  size_t line_number = 0;
  while (std::getline(file, line)) {
    ++line_number;
    std::istringstream input(line);
    std::string command;

    if (!(input >> command)) {
      LOG(ERROR) << "Could not parse line: " << line;
      continue;
    }

    if (command == "song") {
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

      LOG(INFO) << "Clearing song " << song;

    } else if (command == "transpose") {
      if (!(input >> transpose)) {
        LOG(FATAL) << "Transpose requires offset";
      }

      LOG(INFO) << "Transpose set to " << transpose;

    } else if (command == "pattern") {
      if (!song) LOG(FATAL) << "Pattern set with no song";
      if (sequenced) LOG(WARNING) << "Song already sequenced";

      uint32_t tempo;
      if (!(input >> std::hex >> tempo)) {
        LOG(FATAL) << "Pattern requires tempo";
      }

      song->add_pattern({
          static_cast<uint8_t>(tempo & 0xff),
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
          z2music::Pattern::parse_notes(read_line(file)),
      });

      ++patterns;
      LOG(INFO) << "Added pattern " << patterns;

    } else if (command == "sequence") {
      if (!song) LOG(FATAL) << "Sequenc set with no song";
      if (sequenced) LOG(WARNING) << "Song already sequenced";

      size_t n;
      std::vector<size_t> sequence;
      while (input >> n) {
        if (n > patterns || n == 0) {
          LOG(ERROR) << "No such pattern: " << n << "";
        }
        sequence.push_back(n - 1);
        sequenced = true;
      }

      song->set_sequence(sequence);
      LOG(INFO) << "Sequence set";

    } else if (command == "pitch") {
      // TODO set pitch of notes
      // pitch LUT $918F
      // pitch = 1789773 / (16 * (timer + 1))
      // pitch * (16 * (timer + 1)) = 1789773
      // (16 * (timer + 1)) = 1789773 / pitch
      // timer + 1 = 1789773 / (16 * pitch)
      // timer = 1789773 / (16 * pitch) -1

      float pitch = 440.0f;
      uint16_t timer = 1789773 / (16 * pitch) - 1;
      uint8_t offset = 0x1e;

      rom.putc(0x1918f + offset, timer >> 8);
      rom.putc(0x1918f + offset + 1, timer & 0xff);

    } else {
      LOG(WARNING) << "Unknown keyword '" << command << "'";
    }
  }

  LOG(INFO) << "Parsed " << line_number << " lines of music data";

  if (song && !sequenced) {
    LOG(WARNING) << "Reached end of file with unsequenced song";
  }
}

int main(int argc, char** argv) {
  std::ostringstream usage;
  usage << "Modifies the music in a Zelda 2 ROM." << std::endl;
  usage << "Example usage:" << std::endl;
  usage << argv[0] << " <musicfile> --rom <rom> --output <output>";
  absl::SetProgramUsageMessage(usage.str());

  auto args = absl::ParseCommandLine(argc, argv);
  z2music::Rom rom(absl::GetFlag(FLAGS_rom));

  if (args.size() > 1) {
    LOG(INFO) << "Parsing data from given filename: " << args[1];
    std::ifstream file(args[1]);
    process_modfile(rom, file);
  } else {
    LOG(INFO) << "Parsing data from STDIN";
    process_modfile(rom, std::cin);
  }

  rom.save(absl::GetFlag(FLAGS_output));
  return 0;
}
