#include <iostream>

#include "music.h"

void dump_notes(std::vector<z2music::Note> notes) {
  for (z2music::Note n : notes) {
    std::cout << n.pitch_string();
    size_t left = n.length() / 4 - 4;
    std::cout << std::string(left, '.');
  }
  std::cout << std::endl;
}

void dump_song(const z2music::Song& song) {
  std::cout << "Song length: " << song.sequence_length() << " phrases"
            << std::endl;

  for (size_t i = 0; i < song.sequence_length(); ++i) {
    const z2music::Pattern* p = song.at(i);
    dump_notes(p->notes(z2music::Pattern::Channel::Pulse1));
    dump_notes(p->notes(z2music::Pattern::Channel::Pulse2));
    dump_notes(p->notes(z2music::Pattern::Channel::Triangle));
    dump_notes(p->notes(z2music::Pattern::Channel::Noise));
    std::cout << std::endl;
  }
}

void set_overworld_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x18,
      {0xd0, 0x07, 0x90, 0x10, 0x14, 0x16, 0x1a, 0x5f, 0x43, 0xe8, 0x1f, 0xa8,
       0x28, 0x2c, 0x2e, 0x32, 0x77, 0x43},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0xa8, 0xae, 0xb6, 0xa8, 0xae, 0xa8, 0xb6, 0xa8, 0xa4, 0xac, 0xb6,
       0xa4, 0xac, 0xa4, 0xb6, 0xa4, 0xa0, 0xa8, 0xb2, 0xa0, 0xa8, 0xa0,
       0xb2, 0xa0, 0xa4, 0xac, 0xb2, 0xa4, 0xac, 0xa4, 0xb2, 0xa4},
      {},
  });

  intro->set_sequence({0});
}

void set_overworld_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x18,
      {0x29, 0x9e, 0x68, 0x6c, 0xae, 0x6d, 0xe8, 0xe4, 0x69, 0x43, 0x5b, 0x5f},
      {0x90, 0x90, 0x82, 0x90, 0x90, 0x90, 0x82, 0x90, 0x86, 0x86, 0x82,
       0x86, 0x86, 0x86, 0x82, 0x86, 0x88, 0x88, 0x82, 0x88, 0x88, 0x88,
       0x82, 0x88, 0x8c, 0x8c, 0x82, 0x8c, 0x8c, 0x8c, 0x82, 0x8c},
      {0xa8, 0xae, 0xb6, 0xa8, 0xae, 0xa8, 0xb6, 0xa8, 0xa4, 0xac, 0xb6,
       0xa4, 0xac, 0xa4, 0xb6, 0xa4, 0xa0, 0xa8, 0xb2, 0xa0, 0xa8, 0xa0,
       0xb2, 0xa0, 0xa4, 0xac, 0xb2, 0xa4, 0xac, 0xa4, 0xb2, 0xa4},
      {0x49, 0xc8, 0x88, 0x08, 0x08, 0x49, 0xc8, 0x88, 0x08, 0x08, 0x49, 0xc8,
       0x88, 0x88, 0x88, 0x08, 0x08, 0x88, 0x88, 0x08, 0x08, 0x88, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x18,
      {0x29, 0x9e, 0x68, 0x6c, 0xae, 0x6d, 0xe8, 0xe4, 0x6f, 0x43, 0x73, 0x43},
      {0x90, 0x90, 0x82, 0x90, 0x90, 0x90, 0x82, 0x90, 0x8c, 0x8c, 0x82,
       0x8c, 0x8c, 0x8c, 0x82, 0x8c, 0x96, 0x96, 0x82, 0x96, 0x96, 0x96,
       0x82, 0x96, 0x9a, 0x9a, 0x82, 0x9a, 0x9a, 0x9a, 0x82, 0x9a},
      {0xa8, 0xae, 0xb6, 0xa8, 0xae, 0xa8, 0xb6, 0xa8, 0xa4, 0xac, 0xb6,
       0xa4, 0xac, 0xa4, 0xb6, 0xa4, 0xa0, 0xa8, 0xb2, 0xa0, 0xa8, 0xa0,
       0xb2, 0xa0, 0xa4, 0xac, 0xb2, 0xa4, 0xac, 0xa4, 0xb2, 0xa4},
      {0x49, 0xc8, 0x88, 0x08, 0x08, 0x49, 0xc8, 0x88, 0x08, 0x08, 0x49, 0xc8,
       0x88, 0x88, 0x88, 0x08, 0x08, 0x88, 0x88, 0x08, 0x08, 0x88, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x18,
      {0xb6, 0xb2, 0xae, 0xac, 0xe8, 0xa4, 0xa8, 0xec, 0xae, 0xb2,
       0xee, 0xec, 0xae, 0xac, 0x69, 0xc2, 0xac, 0xa8, 0x65, 0xc2},
      {0x9e, 0x9e, 0x82, 0x9e, 0x9e, 0x9e, 0x82, 0x9e, 0x94, 0x94, 0x82,
       0x94, 0x94, 0x94, 0x82, 0x94, 0x96, 0x96, 0x82, 0x96, 0x96, 0x96,
       0x82, 0x96, 0x9a, 0x9a, 0x82, 0x9a, 0x9a, 0x9a, 0x82, 0x9a},
      {0xa8, 0xae, 0xb6, 0xa8, 0xae, 0xa8, 0xb6, 0xa8, 0xa4, 0xac, 0xb6,
       0xa4, 0xac, 0xa4, 0xb6, 0xa4, 0xa0, 0xa8, 0xb2, 0xa0, 0xa8, 0xa0,
       0xb2, 0xa0, 0xa4, 0xac, 0xb2, 0xa4, 0xac, 0xa4, 0xb2, 0xa4},
      {0xc8, 0xc8, 0xc8, 0x88, 0x08, 0x08, 0xc8, 0xc8, 0xc8, 0x88,
       0x88, 0xc8, 0xc8, 0xc8, 0xc8, 0x08, 0x08, 0x88, 0x88, 0x88,
       0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08},
  });

  theme->add_pattern({
      0x18,
      {0xb6, 0xb2, 0xae, 0xac, 0xe8, 0xa4, 0xa8, 0xec, 0xae, 0xb2, 0xee,
       0xec, 0xae, 0xac, 0x6f, 0xc2, 0xb2, 0xae, 0xf2, 0xec, 0xe6},
      {0x9e, 0x9e, 0x82, 0x9e, 0x9e, 0x9e, 0x82, 0x9e, 0x94, 0x94, 0x82,
       0x94, 0x94, 0x94, 0x82, 0x94, 0x96, 0x96, 0x82, 0x96, 0x96, 0x96,
       0x82, 0x96, 0x9a, 0x9a, 0x82, 0x9a, 0x9a, 0x8e, 0x82, 0x8e},
      {0xa8, 0xae, 0xb6, 0xa8, 0xae, 0xa8, 0xb6, 0xa8, 0xa4, 0xac, 0xb6,
       0xa4, 0xac, 0xa4, 0xb6, 0xa4, 0xa0, 0xa8, 0xb2, 0xa0, 0xa8, 0xa0,
       0xb2, 0xa0, 0xa4, 0xac, 0xb2, 0xa4, 0xac, 0xa4, 0xb2, 0xa4},
      {0xc8, 0xc8, 0xc8, 0x88, 0x08, 0x08, 0xc8, 0xc8, 0xc8, 0x88,
       0x88, 0xc8, 0xc8, 0xc8, 0xc8, 0x08, 0x08, 0x88, 0x88, 0x88,
       0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08},
  });

  theme->set_sequence({0, 1, 0, 1, 2, 3});
}

void set_dungeon_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x10,
      {0x5c, 0x58, 0x54, 0x58, 0x54, 0x52, 0x4e, 0x52, 0x54, 0x58, 0x5e, 0x5c},
      {0x43, 0x43, 0x43, 0x43, 0xc2},
      {0x67, 0x02, 0x63, 0x02, 0x61, 0x02, 0x62, 0x68, 0x66},
      {},
  });

  intro->set_sequence({0});
}

void set_dungeon_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x10,
      {0x5d, 0x43, 0xc2, 0xd8, 0xdc, 0xde, 0x63, 0x5f, 0x5d, 0x59},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0xa6, 0xa0, 0x9a, 0x94, 0xa6, 0xa0, 0x9a, 0x94, 0xa6, 0xa0, 0x9a,
       0x94, 0xa6, 0xa0, 0x9a, 0x94, 0xa2, 0x9c, 0x96, 0x90, 0xa2, 0x9c,
       0x96, 0x90, 0xa2, 0x9c, 0x96, 0x90, 0xa2, 0x9c, 0x96, 0x90},
      {0x49, 0x49, 0xc0, 0xc8, 0x88, 0x09},
  });

  theme->add_pattern({
      0x10,
      {0x55, 0x43, 0xc2, 0xd2, 0xd4, 0xd8, 0x4f, 0x43, 0x53, 0x43},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x9e, 0x98, 0x92, 0x8c, 0x9e, 0x98, 0x92, 0x8c, 0x9e, 0x98, 0x92,
       0x8c, 0x9e, 0x98, 0x92, 0x8c, 0x9a, 0x94, 0x8e, 0x88, 0x9a, 0x94,
       0x8e, 0x88, 0x9a, 0x94, 0x8e, 0x88, 0x88, 0x8e, 0x94, 0x9a},
      {0x49, 0x49, 0xc0, 0xc8, 0x88, 0x09},
  });

  theme->add_pattern({
      0x10,
      {0x55, 0x43, 0xc2, 0xd2, 0xd4, 0xd8, 0x5d, 0x59, 0x63, 0x5f},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x9e, 0x98, 0x92, 0x8c, 0x9e, 0x98, 0x92, 0x8c, 0x9e, 0x98, 0x92,
       0x8c, 0x9e, 0x98, 0x92, 0x8c, 0x9a, 0x94, 0x8e, 0x88, 0x9a, 0x94,
       0x8e, 0x88, 0x9a, 0x94, 0x8e, 0x88, 0x8e, 0x94, 0x9a, 0xa0},
      {0x49, 0x49, 0xc0, 0xc8, 0x88, 0x09},
  });

  theme->set_sequence({0, 0, 1, 2});
}

void set_town_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x20,
      {0x53, 0xd0, 0xcc},
      {0x43, 0x43},
      {0x9c, 0x9a, 0x96, 0x92, 0x96, 0x92, 0x90, 0x8c},
      {},
  });

  intro->set_sequence({0});
}

void set_town_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0x82, 0xba, 0xb8, 0xb4, 0xb2, 0x43, 0xba, 0xb8, 0xb4,
       0xb2, 0xae, 0xb2, 0xb4, 0x82, 0xba, 0xb8, 0xb4, 0xb2,
       0x43, 0xba, 0xb8, 0xb4, 0xb2, 0xae, 0xb2, 0xb4},
      {0x65, 0xa2, 0x43, 0xa4, 0xa2, 0xa4, 0xe8, 0xea, 0x65, 0xa2, 0x43, 0xa4,
       0xa2, 0xa4, 0xa8, 0xaa, 0xa8, 0xa4},
      {0xd0, 0xd6, 0x90, 0xd6, 0xdc, 0xd6, 0x90, 0xd6, 0xdc, 0xd0, 0xd6, 0x90,
       0xd6, 0xdc, 0xd6, 0x90, 0xd6, 0xdc},
      {0x49, 0x49, 0x49, 0xc8, 0x88, 0x88},
  });

  theme->add_pattern({
      0x20,
      {0xea, 0xee, 0xb2, 0x43, 0xaa, 0x82, 0xae, 0xb2, 0x82, 0xae, 0x82,
       0xea, 0xee, 0xb2, 0x43, 0xaa, 0x82, 0xae, 0xb2, 0xb4, 0xb8, 0xb4},
      {0x63, 0xa4, 0x43, 0xa2, 0xa4, 0xa8, 0xea, 0xe8, 0x63, 0xa4, 0x43, 0xa2,
       0xa4, 0xa8, 0xaa, 0xa8, 0xa4, 0xa8},
      {0xcc, 0xd2, 0xa4, 0xd2, 0xda, 0xd2, 0x8c, 0xd2, 0xda, 0xcc, 0xd2, 0x8c,
       0xd2, 0xda, 0xd2, 0x8c, 0xd2, 0xda},
      {0x49, 0x49, 0x49, 0xc8, 0x88, 0x88},
  });

  theme->set_sequence({0, 1});
}

void set_house_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0x65, 0xa2, 0x43, 0xa4, 0xa2, 0xa4, 0xe8, 0xea, 0x65, 0xa2, 0x43, 0xa4,
       0xa2, 0xa4, 0xa8, 0xaa, 0xa8, 0xa4},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0xd0, 0xd6, 0x90, 0xd6, 0xdc, 0xd6, 0x90, 0xd6, 0xdc, 0xd0, 0xd6, 0x90,
       0xd6, 0xdc, 0xd6, 0x90, 0xd6, 0xdc},
      {0x49, 0x49, 0x49, 0xc8, 0x88, 0x88},
  });

  theme->add_pattern({
      0x20,
      {0x63, 0xa4, 0x43, 0xa2, 0xa4, 0xa8, 0xea, 0xe8, 0x63, 0xa4, 0x43, 0xa2,
       0xa4, 0xa8, 0xaa, 0xa8, 0xa4, 0xa8},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0xcc, 0xd2, 0xa4, 0xd2, 0xda, 0xd2, 0x8c, 0xd2, 0xda, 0xcc, 0xd2, 0x8c,
       0xd2, 0xda, 0xd2, 0x8c, 0xd2, 0xda},
      {0x49, 0x49, 0x49, 0xc8, 0x88, 0x88},
  });

  theme->set_sequence({0, 1});
}

void set_zelda_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0x82, 0xe2, 0x1e, 0x02, 0x9e, 0x98, 0x9e, 0xa2, 0x82, 0xde, 0x18, 0x02,
       0x98, 0x94, 0xd8},
      {0x43, 0x43, 0x43, 0x43},
      {0x0e, 0x14, 0x18, 0x1e, 0x0e, 0x14, 0x18, 0x1e, 0x0e, 0x14, 0x18,
       0x1e, 0x0e, 0x14, 0x18, 0x1e, 0x0a, 0x10, 0x18, 0x1e, 0x0a, 0x10,
       0x18, 0x1e, 0x0a, 0x10, 0x18, 0x1e, 0x0a, 0x10, 0x18, 0x1e},
      {},
  });

  theme->set_sequence({0});
}

void set_credits_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x28,
      {0x82, 0xac, 0xb0, 0xb6, 0x71, 0x82, 0xac, 0xb0, 0xb6, 0xf0, 0xac,
       0x82, 0xac, 0xe6, 0xec, 0xb0, 0xec, 0x43, 0x82, 0xac, 0xa6, 0xa2},
      {0x82, 0x98, 0x94, 0x82, 0x94, 0x8e, 0x82, 0x8e, 0x8a, 0x82, 0x8e,
       0x8a, 0x82, 0x8e, 0x94, 0xc2, 0x98, 0x94, 0x82, 0x94, 0x8e, 0x82,
       0x8e, 0x8a, 0x8e, 0x8a, 0x82, 0x94, 0x82, 0x8e, 0x82},
      {0x98, 0xc2, 0x98, 0x9e, 0x82, 0xa6, 0x82, 0x98, 0xc2, 0x98,
       0x9e, 0x82, 0xa6, 0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa6,
       0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa6, 0x82},
      {0xc8, 0x88, 0x09, 0x09, 0xc8, 0x88},
  });

  theme->add_pattern({
      0x28,
      {0xe6, 0xac, 0x03, 0xa6, 0x82, 0xa6, 0xac, 0x82, 0xe2, 0x82, 0xa6, 0x82,
       0xe6, 0x82, 0xe2, 0x82, 0xe6, 0xc2, 0xac, 0x82, 0xb0, 0xb6, 0xb0, 0xac},
      {0x82, 0x98, 0x94, 0x82, 0x94, 0x8e, 0x82, 0x8a, 0x8e, 0x94,
       0x98, 0x94, 0x98, 0x82, 0x9e, 0xc2, 0x98, 0x94, 0x8e, 0x94,
       0x82, 0x94, 0x8e, 0x8e, 0x82, 0x8a, 0x8e, 0x84, 0x03},
      {0x90, 0xc2, 0x90, 0x98, 0x82, 0xa2, 0x82, 0x90, 0xc2, 0x90,
       0x98, 0x82, 0xa2, 0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa2,
       0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa2, 0x82},
      {0xc8, 0x88, 0x09, 0x09, 0xc8, 0x88},
  });

  theme->add_pattern({
      0x28,
      {0xc2, 0xb0, 0x82, 0xac, 0xb0, 0xb6, 0xb0, 0xb0, 0x82,
       0xb0, 0x82, 0xac, 0x82, 0xb0, 0x82, 0xb6, 0xf8, 0xf6,
       0xf0, 0xb6, 0xc2, 0xb0, 0x82, 0xf6, 0xf8},
      {0x82, 0x98, 0x94, 0x82, 0x94, 0x8e, 0x82, 0x8e, 0x8a, 0x8e,
       0x94, 0x82, 0x98, 0x82, 0x94, 0x03, 0x94, 0x98, 0x9e, 0x82,
       0x9e, 0x98, 0x9e, 0x82, 0xa2, 0x82, 0x9e, 0x82, 0x98, 0x82},
      {0x98, 0xc2, 0x98, 0x9e, 0x82, 0xa6, 0x82, 0x98, 0xc2, 0x98,
       0x9e, 0x82, 0xa6, 0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa6,
       0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa6, 0x82},
      {0xc8, 0x88, 0x09, 0x09, 0xc8, 0x88},
  });

  theme->add_pattern({
      0x28,
      {0xb6, 0xf0, 0xec, 0x03, 0xc2, 0xf0, 0xec, 0xe6, 0xe2, 0x82, 0xe6, 0x03,
       0xc2, 0xec, 0xb0, 0xb6, 0xb8, 0xb6},
      {0x98, 0x94, 0x98, 0x94, 0xc2, 0x8e, 0x8a, 0x8e, 0x82, 0x94,
       0x82, 0x98, 0x82, 0x94, 0x82, 0x98, 0x94, 0x8e, 0x8a, 0xc2,
       0x8a, 0x8e, 0x8a, 0x82, 0x8a, 0x82, 0x98, 0x03},
      {0x90, 0xc2, 0x90, 0x98, 0x82, 0xa2, 0x82, 0x90, 0xc2, 0x90,
       0x98, 0x82, 0xa2, 0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa2,
       0x82, 0x94, 0xc2, 0x94, 0x9c, 0x82, 0xa2, 0x82},
      {0xc8, 0x88, 0x09, 0x09, 0xc8, 0x88},
  });

  theme->set_sequence({0, 1, 2, 3});
}

void set_encounter_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x10,
      {0x27, 0x5d, 0x03, 0xe6, 0xdc, 0xe6, 0x23, 0x59, 0x82, 0x43, 0x43},
      {0xc2, 0xe6, 0x9c, 0x27, 0xc2, 0xe6, 0x9c, 0x27, 0x23, 0x19, 0x23, 0xd8,
       0x82, 0x98, 0x9c, 0x9e, 0x9c},
      {0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8c, 0x8c, 0x8c,
       0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a,
       0x8a, 0x8a, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8a, 0x8c},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88},
  });

  theme->add_pattern({
      0x10,
      {0x27, 0x5d, 0x03, 0xe6, 0xdc, 0xe6, 0x55, 0x43, 0x6b, 0x63},
      {0xc2, 0xe6, 0x9c, 0x27, 0xc2, 0xe6, 0x9c, 0x27, 0x2d, 0x27, 0x2b, 0x27,
       0xea, 0xe2},
      {0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8c, 0x8c, 0x8c,
       0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a,
       0x8a, 0x8a, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8a, 0x8c},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88},
  });

  theme->add_pattern({
      0x10,
      {0x27, 0x5d, 0x03, 0xe6, 0xdc, 0xe6, 0x55, 0x43, 0x2b, 0xec, 0xb0, 0xb4,
       0x82},
      {0xc2, 0xe6, 0x9c, 0x27, 0xc2, 0xe6, 0x9c, 0x27, 0x2d, 0x27, 0x2b, 0x27,
       0xea, 0xe2},
      {0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8c, 0x8c, 0x8c,
       0x8c, 0x8c, 0x8c, 0x8c, 0x8c, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a, 0x8a,
       0x8a, 0x8a, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8a, 0x8c},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88},
  });

  theme->add_pattern({
      0x10,
      {0x77, 0xc2, 0xf6, 0xf4, 0xb0, 0x2b, 0xec, 0x77, 0xc2, 0xf6, 0xf4, 0xb0,
       0x75, 0x82},
      {0xc2, 0xe6, 0xac, 0x27, 0xf0, 0xac, 0xea, 0x23, 0xc2, 0xe6, 0xac, 0x27,
       0x2d, 0x31, 0xf4},
      {0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x92, 0x92, 0x92,
       0x92, 0x92, 0x92, 0x92, 0x92, 0x94, 0x94, 0x94, 0x94, 0x94, 0x94,
       0x94, 0x94, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98},
      {0x09, 0x09, 0xc8},
  });

  theme->add_pattern({
      0x10,
      {0x77, 0xc2, 0xf6, 0xf4, 0xb0, 0x2b, 0xec, 0xf0, 0xf4, 0xf0, 0xec, 0xea,
       0xec, 0xea, 0xe2},
      {0xc2, 0xe6, 0xac, 0x27, 0xf0, 0xac, 0xea, 0x23, 0x37, 0x35, 0x31, 0x35,
       0xf0, 0xea},
      {0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x8e, 0x92, 0x92, 0x92,
       0x92, 0x92, 0x92, 0x92, 0x92, 0x94, 0x94, 0x94, 0x94, 0x94, 0x94,
       0x94, 0x94, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98},
      {0x09, 0x09, 0xc8},
  });

  theme->set_sequence({0, 1, 0, 2, 3, 4});
}

void set_final_boss_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x10,
      {0x28, 0x02, 0xa8, 0xae, 0x28, 0x02, 0xa8, 0xb6, 0xa8, 0xae, 0xa8, 0xac,
       0xae, 0xac, 0xa8, 0xa4, 0xa0, 0xa4, 0x28, 0x02, 0xa8, 0xae, 0x28, 0x02,
       0xa8, 0xb6, 0xa8, 0xae, 0xb2, 0xae, 0xac, 0xa8, 0xac, 0xae, 0xec},
      {0xd6, 0xc2, 0x43, 0xd6, 0x82, 0xd6, 0x82, 0xd4, 0xd6, 0xc2, 0x43, 0xda,
       0xc2, 0xde, 0xc2},
      {0x90, 0x96, 0x9a, 0x9e, 0x90, 0x96, 0x9a, 0x9e, 0x90, 0x96, 0x9a,
       0x9e, 0x90, 0x96, 0x9a, 0x9e, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92,
       0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a},
      {0x49, 0x49, 0x09, 0x88, 0xc8, 0xc8},
  });

  theme->add_pattern({
      0x10,
      {0x28, 0x02, 0xa8, 0xae, 0x28, 0x02, 0xa8, 0xb6, 0xa8, 0xae, 0xa8,
       0xac, 0xae, 0xac, 0xa8, 0xa4, 0xa0, 0xa4, 0x28, 0x02, 0xa8, 0xae,
       0x28, 0x02, 0xa8, 0xb6, 0xa8, 0xae, 0xb2, 0xae, 0xac, 0xae, 0x69},
      {0xd6, 0xc2, 0x43, 0xd6, 0x82, 0xd6, 0x82, 0xd4, 0xd6, 0xc2, 0x43, 0xd0,
       0xc2, 0xd4, 0xc2},
      {0x90, 0x96, 0x9a, 0x9e, 0x90, 0x96, 0x9a, 0x9e, 0x90, 0x96, 0x9a,
       0x9e, 0x90, 0x96, 0x9a, 0x9e, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92,
       0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a},
      {0x49, 0x49, 0x09, 0x88, 0xc8, 0xc8},
  });

  theme->set_sequence({0, 1});
}

void set_boss_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x10,
      {0x20, 0x02, 0xa0, 0xa6, 0x24, 0x02, 0xa4, 0xa6, 0xa4, 0xa0, 0x9c, 0xa0,
       0x9c, 0x96, 0x92, 0x96, 0x9c, 0x96, 0x20, 0x02, 0xa0, 0xa6, 0xa0, 0xa4,
       0xa6, 0xa4, 0xa0, 0xaa, 0xa6, 0xa4, 0xa0, 0xa4, 0xa6, 0xe0},
      {0x43, 0x43, 0xd2, 0xd6, 0xdc, 0xa0, 0x9c, 0x43, 0x43, 0xe6, 0xe4, 0xe0,
       0xa4, 0xa0},
      {0x88, 0x8e, 0x92, 0x96, 0x88, 0x8e, 0x92, 0x96, 0x88, 0x8e, 0x92,
       0x96, 0x88, 0x8e, 0x92, 0x96, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92,
       0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a},
      {0x49, 0x49, 0x09, 0x88, 0xc8, 0xc8},
  });

  theme->add_pattern({
      0x10,
      {0x20, 0x02, 0xa0, 0xa6, 0xa0, 0xa4, 0xa6, 0xa4, 0xa0, 0x9c, 0x96,
       0x92, 0x96, 0x9c, 0x96, 0x9c, 0xa4, 0x20, 0x02, 0xa0, 0xa6, 0xa0,
       0xa4, 0xa6, 0xa4, 0xa0, 0xa4, 0xa6, 0xa4, 0xa0, 0xa4, 0x9c, 0xe0},
      {0x43, 0x43, 0xd2, 0xd6, 0xdc, 0xa0, 0x9c, 0x43, 0xc2, 0xe4, 0x61, 0x5d},
      {0x88, 0x8e, 0x92, 0x96, 0x88, 0x8e, 0x92, 0x96, 0x88, 0x8e, 0x92,
       0x96, 0x88, 0x8e, 0x92, 0x96, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92,
       0x96, 0x9a, 0x8c, 0x92, 0x96, 0x9a, 0x8c, 0x92, 0x9a, 0x96},
      {0x49, 0x49, 0x09, 0x88, 0xc8, 0xc8},
  });

  theme->set_sequence({0, 1});
}

void set_great_palace_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x20,
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  intro->set_sequence({0});
}

void set_great_palace_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x6f, 0x03, 0xee, 0x6d, 0xc2, 0xec,
       0xe8, 0xac, 0xae, 0x82, 0xac, 0xe8},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xcc, 0x88, 0x0d, 0x07, 0x09,
       0xe4},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x6f, 0x03, 0xee, 0x6d, 0xc2, 0xec,
       0xa8, 0xac, 0xae, 0xb2, 0x82, 0xb6, 0xf2},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xcc, 0x88, 0x0d, 0xc6, 0x88,
       0xcc, 0x88, 0x86, 0x88},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x6f, 0x03, 0xee, 0x6d,
       0xc2, 0xec, 0xa8, 0xa4, 0xa0, 0x9e, 0xa0, 0x9e, 0x9a, 0x96},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xcc, 0x88, 0x0d, 0xc6, 0x88,
       0xcc, 0x88, 0x86, 0x88},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x5f, 0x03, 0xde, 0x5b, 0xc2, 0xda,
       0x96, 0x82, 0x9a, 0x9e, 0x82, 0x9a, 0xd6},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xd0, 0x8c, 0x11, 0xd6, 0x94,
       0xd0, 0x0d},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x5f, 0x03, 0xde, 0x5b, 0xc2, 0xda,
       0x96, 0x9a, 0x9e, 0xa0, 0x82, 0xa4, 0xe0},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xd0, 0x8c, 0x11, 0xd6, 0x94,
       0xd0, 0x94, 0xd6},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0xc2, 0xa8, 0x82, 0x9e, 0xa8, 0x82, 0x5f, 0x03, 0xde, 0x5b, 0xc2, 0xda,
       0xd6, 0x9a, 0x96, 0x82, 0x94, 0xd0},
      {0x43, 0x43, 0xc2, 0xd0, 0x8c, 0x11, 0xc2, 0xd0, 0x8c, 0x11, 0xd6, 0x94,
       0xd0, 0x94, 0xd6},
      {0x90, 0xc2, 0x90, 0xc2, 0x96, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x9a, 0x82,
       0x90, 0xc2, 0x90, 0xc2, 0x9e, 0x82, 0x90, 0xc2, 0x90, 0xc2, 0x8c, 0x82},
      {0x88, 0x88, 0x88, 0xc8, 0x88, 0xc8},
  });

  theme->set_sequence({3, 4, 3, 5, 0, 1, 0, 2});
}

void set_funky_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x20,
      {0x43, 0x43, 0x43, 0x43},
      {0x43, 0x43, 0x43, 0x43},
      {0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4a, 0x0a, 0x86, 0xca, 0x8a,
       0x90, 0x8a, 0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4b, 0x43},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88, 0xc0, 0x49, 0x49, 0xc8, 0xc8, 0xc8},
  });

  intro->set_sequence({0});
}

void set_funky_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0x82, 0xb0, 0xac, 0xb0, 0xb2, 0xb0, 0xac, 0xb0, 0x82, 0xb0, 0xac,
       0xb0, 0xac, 0xa8, 0xa6, 0xa8, 0x82, 0xb0, 0xac, 0xb0, 0xb2, 0xb0,
       0xac, 0xb0, 0xac, 0xa8, 0xa6, 0xa8, 0xa6, 0xa2, 0xa6, 0xa8},
      {0x82, 0x18, 0x02, 0x18, 0x02, 0xd8, 0x14, 0x02, 0x14, 0x02, 0xd4, 0x10,
       0x02, 0x10, 0x02, 0xd0, 0x0e, 0x02, 0x0e, 0x02, 0xce, 0x0a, 0x02, 0x0a,
       0x02, 0x4a, 0x02, 0x0a, 0x02, 0x06, 0x02, 0x0a, 0x02, 0x4b, 0x43},
      {0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4a, 0x0a, 0x86, 0xca, 0x8a,
       0x90, 0x8a, 0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4b, 0x43},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88, 0xc0, 0x49, 0x49, 0xc8, 0xc8, 0xc8},
  });

  theme->add_pattern({
      0x20,
      {0x82, 0xa2, 0x82, 0xa2, 0x9e, 0xa2, 0xa6, 0xa8, 0x82,
       0xa8, 0x82, 0xa8, 0xa6, 0x82, 0xe2, 0x82, 0xa2, 0x82,
       0xa2, 0x9e, 0xa2, 0xa6, 0xa2, 0x43, 0x43},
      {0x82, 0x0a, 0x02, 0x0a, 0x02, 0xca, 0x06, 0x02, 0x06, 0x02,
       0xc6, 0x0a, 0x02, 0x0a, 0x02, 0xca, 0x43, 0x0a, 0x02, 0x0a,
       0x02, 0xca, 0x06, 0x02, 0x06, 0x02, 0xca, 0x03, 0x43},
      {0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4a, 0x0a, 0x86, 0xca, 0x8a,
       0x90, 0x8a, 0x4a, 0x0a, 0x86, 0xca, 0x8a, 0x84, 0x86, 0x4b, 0x43},
      {0xc0, 0x49, 0x49, 0x49, 0x88, 0x88, 0xc0, 0x49, 0x49, 0xc8, 0xc8, 0xc8},
  });

  theme->set_sequence({0, 0, 1});
}

void set_new_gp_intro(z2music::Song* intro) {
  intro->clear();

  intro->add_pattern({
      0x20,
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43},
      {0x50, 0x10, 0x96, 0xd0, 0x90, 0x16, 0x10, 0x96, 0x50, 0x10, 0x96,
       0xd0, 0x90, 0x16, 0x10, 0x9a, 0x4e, 0x0e, 0x94, 0xce, 0x8e, 0x14,
       0x0e, 0x94, 0x4e, 0x0e, 0x94, 0x8e, 0x9a, 0x8e, 0x94, 0x8e},
      {0x09, 0xc8, 0x88, 0xc8, 0x09, 0xc8, 0x88, 0x88, 0x88},
  });

  intro->set_sequence({0});
}

void set_new_gp_theme(z2music::Song* theme) {
  theme->clear();

  theme->add_pattern({
      0x20,
      {0x82, 0x36, 0x32, 0x36, 0x32, 0x2e, 0x2c, 0xae, 0x43, 0x36, 0x32,
       0x36, 0x32, 0x2e, 0x2c, 0xae, 0xa8, 0xac, 0xae, 0x2c, 0x2e, 0x2c,
       0x02, 0x03, 0xac, 0xa8, 0xa6, 0xe8, 0xec, 0xee, 0xec},
      {0x43, 0x82, 0x90, 0x94, 0x96, 0x94, 0x43, 0x90, 0x94, 0x96,
       0x94, 0x43, 0x43, 0x94, 0x90, 0x94, 0x82, 0x94, 0x90, 0x94},
      {0x50, 0x10, 0x96, 0xd0, 0x90, 0x16, 0x10, 0x96, 0x50, 0x10, 0x96,
       0xd0, 0x90, 0x16, 0x10, 0x9a, 0x4e, 0x0e, 0x94, 0xce, 0x8e, 0x14,
       0x0e, 0x94, 0x4e, 0x0e, 0x94, 0x8e, 0x9a, 0x8e, 0x94, 0x8e},
      {0x09, 0xc8, 0x88, 0xc8, 0x09, 0xc8, 0x88, 0x88, 0x88},
  });

  theme->add_pattern({
      0x20,
      {0x29, 0xa8, 0x27, 0xa6, 0x2d, 0xac, 0x68, 0x6c, 0xa8, 0x29, 0xa8, 0x27,
       0xa6, 0x69, 0x43},
      {0x82, 0x90, 0x94, 0x96, 0x55, 0x82, 0x9a, 0x9e, 0x9a, 0xd6, 0xd4, 0x82,
       0x9e, 0x9a, 0x96, 0xd4, 0xd6, 0x4f, 0x51},
      {0x50, 0x5e, 0x96, 0x50, 0x5e, 0x96, 0x4e, 0x54, 0x9a, 0x5a, 0x54,
       0x8e, 0x50, 0x5e, 0x96, 0x50, 0x5e, 0x96, 0x4e, 0x54, 0x9a, 0x51},
      {0xc8, 0x09, 0xc8, 0x88, 0xc8, 0x09, 0x88, 0x88, 0x88},
  });

  theme->add_pattern({
      0x20,
      {0x82, 0x28, 0x26, 0x28, 0x26, 0x28, 0x2c, 0xae, 0xac, 0xa8,
       0xa6, 0x82, 0x28, 0x26, 0x28, 0x2c, 0x28, 0x2c, 0xae, 0xb2,
       0xb6, 0xac, 0x82, 0x28, 0x26, 0x28, 0x26, 0x28, 0x2c, 0xae,
       0xac, 0xae, 0xb2, 0x76, 0x6e, 0xac, 0xae, 0xac, 0xe8},
      {0x82, 0x90, 0x94, 0x96, 0xd4, 0x03, 0x9a, 0x9e, 0x9a, 0xd6, 0xd4, 0x82,
       0x9e, 0x9a, 0x96, 0xd4, 0xd6, 0x4f, 0x51},
      {0x11, 0x90, 0x15, 0x94, 0x17, 0x96, 0x9a, 0xd6, 0x94, 0x11, 0x90, 0x15,
       0x94, 0x17, 0x96, 0x9a, 0xde, 0xa0},
      {0xc0, 0x49, 0x88, 0x09, 0x49, 0x88, 0x09, 0x49, 0x88, 0xc8, 0xc8, 0xc8,
       0x88, 0x88, 0x88},
  });

  theme->add_pattern({
      0x20,
      {0x82, 0x28, 0x26, 0x28, 0x26, 0x28, 0x2c, 0xae, 0xac, 0xa8, 0xa6, 0x82,
       0x28, 0x26, 0x28, 0x2c, 0x28, 0x2c, 0xae, 0xb2, 0xb6, 0xac, 0x82, 0x76,
       0x72, 0x6e, 0x72, 0xae, 0xac, 0x82, 0xae, 0x82, 0xa8, 0x03},
      {0x82, 0x90, 0x94, 0x96, 0xd4, 0x03, 0x9a, 0x9e, 0x9a, 0xd6, 0xd4, 0x82,
       0x9e, 0x9a, 0x96, 0xd4, 0xd6, 0x4f, 0x51},
      {0x11, 0x90, 0x15, 0x94, 0x17, 0x96, 0x9a, 0xd6, 0x15, 0xe0, 0x9e, 0x9a,
       0x96, 0x94, 0xd0, 0xce, 0x51},
      {0xc0, 0x49, 0x88, 0x09, 0x49, 0x88, 0x09, 0x49, 0x88, 0xc8, 0xc8, 0x09,
       0xc8},
  });

  theme->set_sequence({0, 0, 1, 1, 0, 0, 2, 3});
}

void set_fanfare(z2music::Song* fanfare) {
  fanfare->clear();

  fanfare->add_pattern({
      0x10,
      {0xa0, 0xa2, 0xa4, 0x27},
      {0x9a, 0x9c, 0x9e, 0x21},
      {0x9a, 0x9c, 0x9e, 0x21},
      {},
  });

  fanfare->set_sequence({0});
}

void set_title_intro(z2music::Song* title) {
  title->clear();

  title->add_pattern({
      0x30,
      0x30,
      {0x82, 0x60, 0x5e, 0x56, 0x4c},
      {},
      {},
      {},
  });

  title->add_pattern({
      0x30,
      0x30,
      {0x82, 0x60, 0x5c, 0x56, 0x4c, 0x60, 0x5c, 0x56, 0x4c, 0x60, 0x5c, 0x54,
       0x4c, 0x60, 0x5a, 0x54, 0x4c, 0x83, 0x54, 0x84, 0x5a, 0x60, 0x86, 0x64},
      {},
      {},
      {},
  });

  title->set_sequence({0, 0, 0, 0, 1});
}

void set_title_theme_buildup(z2music::Song* title) {
  title->clear();

  title->add_pattern({
      0x00,
      0x00,
      {0x86, 0x02, 0x02, 0x85, 0x02},
      {},
      {0x82, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26,
       0x1c, 0x26, 0x1c, 0x26, 0x1c},
      {0x85, 0x0a, 0x0a, 0x0a, 0x83, 0x0a, 0x82, 0x0a, 0x02},
  });

  title->add_pattern({
      0x00,
      0x00,
      {0x86, 0x02, 0x02, 0x85, 0x02},
      {},
      {0x82, 0x22, 0x02, 0x22, 0x18, 0x22, 0x02, 0x22, 0x18, 0x22, 0x02, 0x22,
       0x18, 0x22, 0x18, 0x22, 0x18},
      {0x85, 0x0a, 0x0a, 0x0a, 0x83, 0x0a, 0x82, 0x0a, 0x02},
  });

  title->set_sequence({0, 0, 1, 1});
}

void set_title_theme_main(z2music::Song* title) {
  title->clear();

  title->add_pattern({
      0x00,
      0x00,
      {0x83, 0x56, 0x84, 0x4c, 0x80, 0x56, 0x02, 0x56, 0x5a, 0x5c, 0x60, 0x8a,
       0x5a, 0x82, 0x56, 0x52, 0x56},
      {0x85, 0x02, 0x8a, 0x02, 0x80, 0x3e, 0x02, 0x3e, 0x42, 0x44, 0x48, 0x85,
       0x42},
      {0x82, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26,
       0x1c, 0x26, 0x1c, 0x26, 0x1c},
      {0x83, 0x0a, 0x0a, 0x82, 0x02, 0x0a, 0x80, 0x02, 0x0a, 0x82, 0x02},
  });

  title->add_pattern({
      0x00,
      0x00,
      {0x8a, 0x5a, 0x82, 0x56, 0x52, 0x56, 0x83, 0x5a, 0x5c, 0x60, 0x5c},
      {0x82, 0x02, 0x80, 0x42, 0x3e, 0x83, 0x42, 0x82, 0x02, 0x80,
       0x42, 0x3e, 0x82, 0x42, 0x44, 0x83, 0x48, 0x44, 0x42, 0x3e},
      {0x82, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26,
       0x1c, 0x26, 0x1c, 0x26, 0x1c},
      {0x83, 0x0a, 0x0a, 0x82, 0x02, 0x0a, 0x80, 0x02, 0x0a, 0x82, 0x02},
  });

  title->add_pattern({
      0x00,
      0x00,
      {0x8a, 0x5a, 0x82, 0x56, 0x52, 0x56, 0x83, 0x5a, 0x5c, 0x60, 0x64},
      {0x82, 0x02, 0x80, 0x42, 0x3e, 0x83, 0x42, 0x82, 0x02, 0x80,
       0x42, 0x3e, 0x82, 0x42, 0x44, 0x83, 0x48, 0x44, 0x48, 0x4c},
      {0x82, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26, 0x1c, 0x26, 0x02, 0x26,
       0x1c, 0x26, 0x1c, 0x26, 0x1c},
      {0x83, 0x0a, 0x0a, 0x82, 0x02, 0x0a, 0x80, 0x02, 0x0a, 0x82, 0x02},
  });

  title->set_sequence({0, 1, 0, 2});
}

void set_title_theme_breakdown(z2music::Song* title) {
  title->clear();

  title->add_pattern({
      0x00,
      0x00,
      {0x80, 0x64, 0x60, 0x5c, 0x5a, 0x82, 0x5c, 0x80, 0x5a, 0x56, 0x82, 0x52,
       0x80, 0x56, 0x5a, 0x82, 0x60, 0x5c, 0x80, 0x64, 0x60, 0x5c, 0x5a, 0x82,
       0x5c, 0x80, 0x5a, 0x56, 0x82, 0x52, 0x80, 0x56, 0x5a, 0x83, 0x56, 0x80,
       0x64, 0x60, 0x5c, 0x5a, 0x82, 0x5c, 0x80, 0x5a, 0x56, 0x82, 0x52, 0x80,
       0x56, 0x5a, 0x82, 0x60, 0x5c, 0x80, 0x64, 0x60, 0x5c, 0x5a, 0x5c, 0x5a,
       0x56, 0x52, 0x56, 0x5a, 0x5c, 0x5a, 0x83, 0x56},
      {0x83, 0x3e, 0x84, 0x34, 0x82, 0x3e, 0x34, 0x3e, 0x83, 0x42,
       0x84, 0x34, 0x82, 0x42, 0x34, 0x42, 0x83, 0x44, 0x84, 0x34,
       0x82, 0x44, 0x34, 0x44, 0x83, 0x42, 0x34, 0x42, 0x3a},
      {0x82, 0x1c, 0x80, 0x1c, 0x1c, 0x82, 0x1c, 0x80, 0x1c, 0x1c, 0x82, 0x1c,
       0x80, 0x1c, 0x1c, 0x82, 0x1c, 0x80, 0x1c, 0x1c, 0x82, 0x20, 0x80, 0x20,
       0x20, 0x82, 0x20, 0x80, 0x20, 0x20, 0x82, 0x20, 0x80, 0x20, 0x20, 0x82,
       0x20, 0x80, 0x20, 0x20, 0x82, 0x22, 0x80, 0x22, 0x22, 0x82, 0x22, 0x80,
       0x22, 0x22, 0x82, 0x22, 0x80, 0x22, 0x22, 0x82, 0x22, 0x80, 0x22, 0x22,
       0x82, 0x26, 0x80, 0x26, 0x26, 0x82, 0x26, 0x80, 0x26, 0x26, 0x82, 0x26,
       0x80, 0x26, 0x26, 0x26, 0x22, 0x20, 0x1c},
      {0x82, 0x02, 0x80, 0x02, 0x02, 0x82, 0x02, 0x02, 0x02, 0x80, 0x02, 0x02,
       0x82, 0x02, 0x02, 0x02, 0x80, 0x02, 0x02, 0x82, 0x02, 0x02, 0x02, 0x80,
       0x02, 0x02, 0x82, 0x02, 0x02, 0x02, 0x80, 0x02, 0x02, 0x82, 0x02, 0x02,
       0x02, 0x80, 0x02, 0x02, 0x82, 0x02, 0x02, 0x02, 0x80, 0x02, 0x02, 0x82,
       0x02, 0x02, 0x80, 0x02, 0x02, 0x02, 0x02, 0x82, 0x0a, 0x0a},
  });

  title->set_sequence({0});
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " z2_rom" << std::endl;
    return 1;
  }

  const std::string file = std::string(argv[1]);

  z2music::Rom rom(file);

  // Change title music
  set_title_intro(rom.song(z2music::Rom::SongTitle::TitleIntro));
  set_title_theme_buildup(rom.song(z2music::Rom::SongTitle::TitleThemeStart));
  set_title_theme_main(rom.song(z2music::Rom::SongTitle::TitleThemeBuildup));
  set_title_theme_main(rom.song(z2music::Rom::SongTitle::TitleThemeMain));
  set_title_theme_breakdown(
      rom.song(z2music::Rom::SongTitle::TitleThemeBreakdown));

  // Change overworld music
  set_overworld_intro(rom.song(z2music::Rom::SongTitle::OverworldIntro));
  set_overworld_theme(rom.song(z2music::Rom::SongTitle::OverworldTheme));
  set_encounter_theme(rom.song(z2music::Rom::SongTitle::BattleTheme));
  set_town_intro(rom.song(z2music::Rom::SongTitle::TownIntro));
  set_town_theme(rom.song(z2music::Rom::SongTitle::TownTheme));
  set_house_theme(rom.song(z2music::Rom::SongTitle::HouseTheme));

  // Change dungeon music
  set_funky_intro(rom.song(z2music::Rom::SongTitle::PalaceIntro));
  set_funky_theme(rom.song(z2music::Rom::SongTitle::PalaceTheme));
  set_boss_theme(rom.song(z2music::Rom::SongTitle::BossTheme));

  // Change great palace music
  set_new_gp_intro(rom.song(z2music::Rom::SongTitle::GreatPalaceIntro));
  set_new_gp_theme(rom.song(z2music::Rom::SongTitle::GreatPalaceTheme));
  set_final_boss_theme(rom.song(z2music::Rom::SongTitle::FinalBossTheme));

  // Change credits music
  set_zelda_theme(rom.song(z2music::Rom::SongTitle::ZeldaTheme));
  set_credits_theme(rom.song(z2music::Rom::SongTitle::CreditsTheme));

  set_fanfare(rom.song(z2music::Rom::SongTitle::CaveItemFanfare));
  set_fanfare(rom.song(z2music::Rom::SongTitle::TownItemFanfare));
  set_fanfare(rom.song(z2music::Rom::SongTitle::PalaceItemFanfare));
  set_fanfare(rom.song(z2music::Rom::SongTitle::GreatPalaceItemFanfare));

  // Fix a bug in the vanilla game
  rom.write(0x5d6b, {0xea, 0xea, 0xea});

  // Change credits
  z2music::Credits* c = rom.credits();
  c->set(0, {"ZELDA II ROG", "BY GTM604", ""});
  c->set(1, {"HACKING     ", "CF207    ", ""});
  c->set(2, {"MUSIC  ", "BGT  ", ""});
  c->set(3, {"DESIGN", "GTM604", ""});
  c->set(4, {"SPECIAL THANKS", "SAUL R", "LINK7777"});
  c->set(5, {"", "EON   ", "SHEEPDOG"});
  c->set(6, {"", "MAX", "TRAX    "});
  c->set(7, {"", "FISHWERKS", "NINTENDO"});
  c->set(8, {"YOU DID IT/   ", "GO PLAY RANDO", "BY DIGSHAKE"});

  rom.save("/tmp/output.nes");

  return 0;
}
