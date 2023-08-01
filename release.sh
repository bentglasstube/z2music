#!/bin/bash

set -e

make_release() {
  local version=$(git describe --tags --long)
  bazel test :all
  bazel build -c opt tools:modder_windows tools:music_dump_windows

  mkdir z2music
  cp bazel-bin/tools/modder.exe z2music
  cp bazel-bin/tools/music_dump.exe z2music
  cp projects/amida.z2music z2music

  local zipfile="z2music-$version.zip"
  zip "$zipfile" z2music/*
  rm -rf z2music

  local rpath="/mnt/build/z2music/$(git describe --tags --abbrev=0)"
  ssh build@build.eatabrick.org mkdir -p "$rpath"
  scp "$zipfile" build@build.eatabrick.org:"$rpath"/.
}

make_release "$@"
