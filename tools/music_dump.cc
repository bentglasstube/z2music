#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "rom.h"

ABSL_FLAG(std::string, rom, "", "Path to the rom file to dump.");
ABSL_FLAG(std::string, song, "", "Which song to dump.");

const z2music::Song* get_song_by_name(const z2music::Rom& rom,
                                      const std::string& name) {
  auto song = rom.song(name);
  if (!song) LOG(FATAL) << "Unknown song name " << name;
  return song;
}

void dump_notes(const std::vector<z2music::Note>& notes) {
  // z2music::Note::Duration duration = Note::Duration::Unknown;
  for (const auto note : notes) {
    std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0')
              << (int)note << " ";
  }
  std::cout << std::endl;
}

void dump_song(const z2music::Song* song) {
  for (const auto& pattern : song->patterns()) {
    int tempo = pattern.tempo();  // cast to int to output as number
    std::cout << "pattern 0x" << std::hex << std::setw(2) << tempo << std::endl;
    dump_notes(pattern.notes(z2music::Pattern::Channel::Pulse1));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Pulse2));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Triangle));
    dump_notes(pattern.notes(z2music::Pattern::Channel::Noise));
  }

  std::cout << "sequence";
  for (size_t n : song->sequence()) {
    std::cout << " " << (n + 1);
  }
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::ostringstream usage;
  usage << "Dumps the music from a Zelda 2 ROM." << std::endl;
  usage << "Example usage:" << std::endl;
  usage << argv[0] << " --rom <rom> --song <songname>";
  absl::SetProgramUsageMessage(usage.str());

  auto args = absl::ParseCommandLine(argc, argv);
  const z2music::Rom rom(absl::GetFlag(FLAGS_rom));
  const std::string title = absl::GetFlag(FLAGS_song);
  const z2music::Song* song = get_song_by_name(rom, title);

  std::cout << "song " << title << std::endl;
  dump_song(song);

  return 0;
}
