cc_library(
  name = "music",
  visibility = ["//visibility:public"],
  hdrs = [
    "credits.h",
    "pattern.h",
    "pitch.h",
    "rom.h",
    "song.h",
  ],
  srcs = [
    "credits.cc",
    "pattern.cc",
    "pitch.cc",
    "rom.cc",
    "song.cc",
  ],
  deps = [
    ":note",
    "@absl//absl/log:log",
  ]
)

cc_library(
  name = "note",
  hdrs = ["note.h"],
  srcs = ["note.cc"],
  deps = [
    "@absl//absl/log:log",
  ],
)
