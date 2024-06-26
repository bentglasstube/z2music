load("@crt//rules:pkg_win.bzl", "pkg_win");

package(
  default_visibility = ["//visibility:public"],
)

config_setting(
  name = "windows",
  constraint_values = [
    "@platforms//os:windows",
  ],
)

cc_library(
  name = "credits",
  hdrs = ["credits.h"],
  deps = [":util"],
)

cc_library(
  name = "duration_lut",
  hdrs = ["duration_lut.h"],
  srcs = ["duration_lut.cc"],
  deps = [
    ":note",
    ":util",
  ],
)

cc_library(
  name = "fake_rom",
  hdrs = ["fake_rom.h"],
  srcs = ["fake_rom.cc"],
  deps = [
    ":pitch",
    ":rom",
    ":util",
  ],
)

cc_library(
  name = "note",
  hdrs = ["note.h"],
  srcs = ["note.cc"],
  deps = [
    "@absl//absl/log:log",
    ":pitch",
    ":util",
  ],
)

cc_library(
  name = "pattern",
  hdrs = ["pattern.h"],
  srcs = ["pattern.cc"],
  deps = [
    "@absl//absl/log:log",
    ":note",
    ":pitch",
    ":util",
  ],
)

cc_library(
  name = "pitch",
  hdrs = ["pitch.h"],
  srcs = ["pitch.cc"],
  deps = [":util"],
)

cc_library(
  name = "pitch_lut",
  hdrs = ["pitch_lut.h"],
  srcs = ["pitch_lut.cc"],
  deps = [
    "@absl//absl/log:log",
    ":pitch",
    ":util",
  ],
)

cc_library(
  name = "rom",
  hdrs = ["rom.h"],
  srcs = ["rom.cc"],
  deps = [
    "@absl//absl/log:log",
    ":credits",
    ":duration_lut",
    ":note",
    ":pattern",
    ":pitch",
    ":pitch_lut",
    ":sfx_notes",
    ":song",
    ":util",
  ]
)

cc_library(
  name = "sfx_notes",
  hdrs = ["sfx_notes.h"],
  deps = [
    ":note",
    ":util",
  ],
)

cc_library(
  name = "song",
  hdrs = ["song.h"],
  srcs = ["song.cc"],
  deps = [
    ":pattern",
    ":pitch",
    ":util",
  ],
)

cc_library(
  name = "util",
  hdrs = ["util.h"],
  srcs = ["util.cc"],
)

cc_test(
  name = "pattern_test",
  srcs = ["pattern_test.cc"],
  deps = [
    "@googletest//:gtest_main",
    ":fake_rom",
    ":note",
    ":pattern",
    ":pitch",
    ":rom",
  ],
  size = 'small',
)

cc_test(
  name = "pitch_test",
  srcs = ["pitch_test.cc"],
  deps = [
    "@absl//absl/log:log",
    "@googletest//:gtest_main",
    ":pitch",
  ],
  size = 'small',
)

cc_test(
  name = "rom_test",
  srcs = ["rom_test.cc"],
  deps = [
    "@googletest//:gtest_main",
    ":fake_rom",
    ":pitch",
    ":rom",
  ],
  size = 'small',
)

pkg_win(
  name = "release",
  srcs = [
    "//tools:credits_dump",
    "//tools:modder",
    "//tools:music_dump",
    "//tools:pitch_dump",
    "//projects:demo_projects",
  ],
)
