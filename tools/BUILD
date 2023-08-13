load("@crt//rules:transition.bzl", "platform_target")

package(
  default_visibility = ["//visibility:public"],
)

cc_binary(
  name = "credits_dump",
  srcs = ["credits_dump.cc"],
  deps = [
    "//:credits",
    "//:rom",
  ],
  linkopts = select({
    "//:windows": [
      "-ldbghelp",
    ],
    "//conditions:default": [],
  }),
)

platform_target(
  name = "credits_dump_windows",
  platform = "@crt//platforms/x86_64:win64",
  target = ":credits_dump",
)

cc_binary(
  name = "modder",
  srcs = ["modder.cc"],
  deps = [
    "@absl//absl/flags:flag",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:flags",
    "@absl//absl/log:log",
    "//:rom",
    "//:util",
  ],
  linkopts = select({
    "//:windows": [
      "-ldbghelp",
    ],
    "//conditions:default": [],
  }),
)

platform_target(
  name = "modder_windows",
  platform = "@crt//platforms/x86_64:win64",
  target = ":modder",
)

cc_binary(
  name = "music_dump",
  srcs = ["music_dump.cc"],
  deps = [
    "@absl//absl/flags:flag",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:flags",
    "@absl//absl/log:log",
    "//:rom",
    "//:util",
  ],
  linkopts = select({
    "//:windows": [
      "-ldbghelp",
    ],
    "//conditions:default": [],
  }),
)

platform_target(
  name = "music_dump_windows",
  platform = "@crt//platforms/x86_64:win64",
  target = ":music_dump",
)

cc_binary(
  name = "pitch_dump",
  srcs = ["pitch_dump.cc"],
  deps = [
    "@absl//absl/flags:flag",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:flags",
    "@absl//absl/log:log",
    "//:pitch",
    "//:rom",
  ],
  linkopts = select({
    "//:windows": [
      "-ldbghelp",
    ],
    "//conditions:default": [],
  }),
)

platform_target(
  name = "pitch_dump_windows",
  platform = "@crt//platforms/x86_64:win64",
  target = ":pitch_dump",
)
