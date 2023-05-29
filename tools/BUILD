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
      "@com_google_absl//absl/flags:flag",
      "@com_google_absl//absl/flags:parse",
      "@com_google_absl//absl/flags:usage",
      "@com_google_absl//absl/log:log",
      "@com_google_absl//absl/log:flags",
    ]
)
