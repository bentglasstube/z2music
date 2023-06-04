#ifndef Z2MUSIC_ROM
#define Z2MUSIC_ROM

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "credits.h"
#include "pitch.h"
#include "song.h"

namespace z2music {

class Rom {
 public:
  enum class SongTitle {
    TitleIntro,
    TitleThemeStart,
    TitleThemeBuildup,
    TitleThemeMain,
    TitleThemeBreakdown,

    OverworldIntro,
    OverworldTheme,
    BattleTheme,
    CaveItemFanfare,

    TownIntro,
    TownTheme,
    HouseTheme,
    TownItemFanfare,

    PalaceIntro,
    PalaceTheme,
    BossTheme,
    PalaceItemFanfare,
    CrystalFanfare,

    GreatPalaceIntro,
    GreatPalaceTheme,
    ZeldaTheme,
    CreditsTheme,
    GreatPalaceItemFanfare,
    TriforceFanfare,
    FinalBossTheme,
  };

  static constexpr size_t kTitleScreenLoader = 0x0182fd;
  static constexpr size_t kOverworldLoader = 0x019b90;
  static constexpr size_t kTownLoader = 0x019bcf;
  static constexpr size_t kPalaceLoader = 0x019c0e;
  static constexpr size_t kGreatPalaceLoader = 0x019c4b;

  Rom(const std::string& filename);

  uint8_t getc(size_t address) const;
  uint16_t getw(size_t address) const;
  uint16_t getwr(size_t address) const;

  void putc(size_t address, uint8_t data);
  void putw(size_t address, uint16_t data);

  void read(uint8_t* buffer, size_t address, size_t length) const;
  void write(size_t address, std::vector<uint8_t> data);

  bool commit();
  void save(const std::string& filename);
  void move_song_table(size_t loader_address, uint16_t base_address);

  Song* song(SongTitle title) { return &songs_[title]; }
  Credits* credits() { return &credits_; }
  PitchLUT* pitch_lut() { return &pitch_lut_; }

 private:
  static constexpr size_t kHeaderSize = 0x10;
  static constexpr size_t kRomSize = 0x040000;
  static constexpr size_t kPitchLUTAddress = 0x01918f;

  uint8_t header_[kHeaderSize];
  uint8_t data_[kRomSize];

  size_t title_screen_table = 0x0184da;
  size_t overworld_song_table = 0x01a000;
  size_t town_song_table = 0x01a3ca;
  size_t palace_song_table = 0x01a62f;
  size_t great_palace_song_table = 0x01a936;

  std::unordered_map<SongTitle, Song> songs_;
  Credits credits_;
  PitchLUT pitch_lut_;

  void commit(size_t address, std::vector<SongTitle> songs);
  size_t get_song_table_address(size_t loader_address) const;
};

}  // namespace z2music

#endif  // define Z2MUSIC_ROM
