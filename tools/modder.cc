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
#include "util.h"

ABSL_FLAG(std::string, rom, "", "Path to the rom file to modify.");
ABSL_FLAG(std::string, output, "", "Path where modified rom should be saved.");

std::string read_line(std::istream& file) {
  std::string line;
  std::getline(file, line);
  return line;
}

z2music::Song* get_song_by_name(z2music::Rom& rom, const std::string& name) {
  auto title = rom.title_by_name(name);
  if (title == z2music::Rom::SongTitle::Unknown)
    LOG(FATAL) << "Unknown song name " << name;
  return &rom.song(title);
}

z2music::Address get_loader_by_name(z2music::Rom& rom,
                                    const std::string& name) {
  if (name == "Title") return z2music::Rom::kTitleScreenLoader;
  if (name == "Overworld") return z2music::Rom::kOverworldLoader;
  if (name == "Town") return z2music::Rom::kTownLoader;
  if (name == "Palace") return z2music::Rom::kPalaceLoader;
  if (name == "GreatPalace") return z2music::Rom::kGreatPalaceLoader;
  return z2music::Address(0);
}

void rtrim(std::string& str) {
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         [](unsigned char c) { return !std::isspace(c); })
                .base(),
            str.end());
}

void process_modfile(z2music::Rom& rom, std::istream& file) {
  int transpose = 0;
  size_t patterns = 0;
  bool sequenced = false;
  z2music::Song* song = nullptr;

  std::string line;
  size_t line_number = 0;
  while (std::getline(file, line)) {
    rtrim(line);
    ++line_number;

    if (line == "") continue;

    std::istringstream input(line);
    std::string command;

    if (!(input >> command)) {
      LOG(FATAL) << "Could not parse line: " << line;
    }

    if (command == "song") {
      std::string name;
      if (!(input >> name)) {
        LOG(FATAL) << "Song requires name";
      }

      if (song && !sequenced && patterns > 0) {
        LOG(WARNING) << "Song changed without setting sequence";
      }
      song = get_song_by_name(rom, name);
      song->clear();
      transpose = 0;
      patterns = 0;
      sequenced = false;

      LOG(INFO) << "Clearing song " << name;

    } else if (command == "loader") {
      std::string name;
      z2music::Address address;

      if (!(input >> name >> address)) {
        LOG(FATAL) << "Loader requires name and address";
      }

      z2music::Address loader = get_loader_by_name(rom, name);
      if (loader == 0) LOG(FATAL) << "Unknown loader name: " << name;
      rom.move_song_table(loader, address);

      LOG(INFO) << "Moving loader " << name << " to " << address;

    } else if (command == "transpose") {
      if (!(input >> transpose)) {
        LOG(FATAL) << "Transpose requires offset";
      }

      LOG(INFO) << "Transpose set to " << transpose;

    } else if (command == "pattern") {
      if (!song) LOG(FATAL) << "Pattern set with no song";
      if (sequenced) LOG(WARNING) << "Song already sequenced";

      z2music::byte tempo, voice2;
      if (!(input >> tempo)) {
        LOG(FATAL) << "Pattern requires tempo";
      }

      if (input >> voice2) {
        // two bytes means a voiced pattern (i.e. title music)
        song->add_pattern({
            tempo,
            voice2,
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
        });
      } else {
        song->add_pattern({
            tempo,
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
            z2music::Pattern::parse_notes(read_line(file)),
        });
      }

      ++patterns;
      LOG(INFO) << "Added pattern " << patterns;

    } else if (command == "sequence") {
      if (!song) LOG(FATAL) << "Sequence set with no song";
      if (sequenced) LOG(WARNING) << "Song already sequenced";

      z2music::byte n;
      std::vector<z2music::byte> sequence;
      while (input >> n) {
        if (n > patterns || n == 0) {
          LOG(ERROR) << "No such pattern: " << n << " (" << patterns << ")";
        }
        sequence.push_back(n - 1);
        sequenced = true;
      }

      song->set_sequence(sequence);
      LOG(INFO) << "Sequence set";

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
