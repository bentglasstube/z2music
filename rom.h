#ifndef Z2MUSIC_ROM
#define Z2MUSIC_ROM

#include <string>
#include <unordered_map>
#include <vector>

#include "credits.h"
#include "pitch.h"
#include "pitch_lut.h"
#include "song.h"
#include "util.h"

namespace z2music {

class Rom {
 public:
  enum class SongTitle {
    Unknown,

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

  static constexpr Address kTitleScreenLoader = 0x0182fd;
  static constexpr Address kOverworldLoader = 0x019b90;
  static constexpr Address kTownLoader = 0x019bcf;
  static constexpr Address kPalaceLoader = 0x019c0e;
  static constexpr Address kGreatPalaceLoader = 0x019c4b;

  Rom(const std::string& filename);

  byte getc(Address address) const;
  WordLE getw(Address address) const;
  WordBE getwr(Address address) const;

  void putc(Address address, byte data);
  void putw(Address address, WordLE data);
  void putwr(Address address, WordBE data);

  void read(byte* buffer, Address address, size_t length) const;
  void write(Address address, std::vector<byte> data);

  bool commit();
  void save(const std::string& filename);
  void move_song_table(Address loader_address, Address base_address);

  Song* song(const std::string& name);
  const Song* song(const std::string& name) const;
  Song* song(SongTitle title) { return &songs_[title]; }
  const Song* song(SongTitle title) const { return &songs_.at(title); }
  Credits* credits() { return &credits_; }
  PitchLUT* pitch_lut() { return &pitch_lut_; }

 private:
  static constexpr size_t kHeaderSize = 0x10;
  static constexpr size_t kRomSize = 0x040000;
  static constexpr size_t kPitchLUTAddress = 0x01918f;

  byte header_[kHeaderSize];
  byte data_[kRomSize];

  Address title_screen_table = 0x0184da;
  Address overworld_song_table = 0x01a000;
  Address town_song_table = 0x01a3ca;
  Address palace_song_table = 0x01a62f;
  Address great_palace_song_table = 0x01a936;

  std::unordered_map<SongTitle, Song> songs_;
  Credits credits_;
  PitchLUT pitch_lut_;

  void commit(Address address, std::vector<SongTitle> songs);
  Address get_song_table_address(Address loader_address) const;

  void read_pitch_lut();

  void commit_pitch_lut();
};

}  // namespace z2music

#endif  // define Z2MUSIC_ROM
