#!/bin/bash

set -e

main() {
  local rom="$1"

  local project_file="$(mktemp)"
  local rom_file="$(mktemp)"
  local second_file="$(mktemp)"

  bazel build -c opt tools:modder tools:music_dump
  bazel-bin/tools/music_dump --minloglevel 1 --rom "$rom" > "$project_file"
  bazel-bin/tools/modder "$project_file" --minloglevel 1 --rom "$rom" --output "$rom_file"
  bazel-bin/tools/music_dump --minloglevel 1 --rom "$rom_file" > "$second_file"

  vimdiff "$project_file" "$second_file"
}

main "$@"
