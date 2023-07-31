load("@crt//rules:transition.bzl", "platform_target")

cc_binary(
  name = "credits_dump",
  srcs = ["credits_dump.cc"],
  deps = ["//:music"],
)

cc_binary(
  name = "modder",
  srcs = ["modder.cc"],
  deps = [
    "//:music",
    "@absl//absl/flags:flag",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:log",
    "@absl//absl/log:flags",
  ],
  linkopts = select({
    "//:windows": [
      "-ldbghelp",
    ],
    "//conditions:default": [],
  }),
)

platform_target(
  name = "modder-windows",
  platform = "@crt//platforms/x86_64:win64",
  target = ":modder",
)

cc_binary(
  name = "music_dump",
  srcs = ["music_dump.cc"],
  deps = [
    "//:music",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:flags",
  ],
)

cc_binary(
  name = "pitch_dump",
  srcs = ["pitch_dump.cc"],
  deps = [
    "//:music",
    "@absl//absl/flags:parse",
    "@absl//absl/flags:usage",
    "@absl//absl/log:flags",
  ],
)
