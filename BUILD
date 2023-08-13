load("@crt//rules:pkg_win.bzl", "pkg_win");

config_setting(
  name = "windows",
  constraint_values = [
    "@platforms//os:windows",
  ],
)

cc_library(
  name = "music",
  visibility = ["//visibility:public"],
  hdrs = [
    "credits.h",
    "pattern.h",
    "pitch_lut.h",
    "rom.h",
    "song.h",
  ],
  srcs = [
    "credits.cc",
    "pattern.cc",
    "pitch_lut.cc",
    "rom.cc",
    "song.cc",
  ],
  deps = [
    ":note",
    ":pitch",
    ":util",
    "@absl//absl/log:log",
  ]
)

cc_library(
  name = "note",
  hdrs = ["note.h"],
  srcs = ["note.cc"],
  deps = [
    ":util",
    "@absl//absl/log:log",
  ],
)

cc_library(
  name = "pitch",
  hdrs = ["pitch.h"],
  srcs = ["pitch.cc"],
  deps = [
    ":util",
    "@absl//absl/log:log",
  ],
)

cc_library(
  name = "pitch_lut",
  hdrs = ["pitch_lut.h"],
  srcs = ["pitch_lut.cc"],
  deps = [
    ":music",
    ":pitch",
    ":util",
    "@absl//absl/log:log",
  ],
)

cc_library(
  name = "util",
  hdrs = ["util.h"],
  srcs = ["util.cc"],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "pitch_test",
  srcs = ["pitch_test.cc"],
  deps = [
    ":pitch",
    "@googletest//:gtest_main",
  ],
  size = 'small',
)

cc_test(
  name = "pattern_test",
  srcs = ["pattern_test.cc"],
  deps = [
    ":music",
    "@googletest//:gtest_main",
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
