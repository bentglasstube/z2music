#!/bin/bash

set -e

main() {
  local rom="$1"

  if [ -z "$rom" ]; then
    echo "Usage: $0 <romfile>" >&2
    exit 1
  fi

  local name=$(basename "$rom" .nes)

  local project_file="$(mktemp "$name-XXXXXX-first.z2music")"
  local rom_file="$(mktemp "$name-XXXXXX.nes")"
  local second_file="$(mktemp "$name-XXXXXX-second.z2music")"

  bazel build -c opt tools:modder tools:music_dump
  bazel-bin/tools/music_dump --minloglevel 1 --rom "$rom" > "$project_file"
  bazel-bin/tools/modder "$project_file" --minloglevel 1 --rom "$rom" --output "$rom_file"
  bazel-bin/tools/music_dump --minloglevel 1 --rom "$rom_file" > "$second_file"

  vimdiff "$project_file" "$second_file"

  rm -f "$project_file" "$rom_file" "$second_file"
}

main "$@"
