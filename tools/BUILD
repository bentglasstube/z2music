cc_binary(
  name = "credits",
  srcs = ["credits.cc"],
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
  ]
)
