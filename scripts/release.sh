#!/bin/bash

set -e

make_release() {
  local version=$(git describe --tags --long)

  echo "Building release for version $version"

  bazel test :all
  bazel build -c opt tools:credits_dump_windows tools:modder_windows tools:music_dump_windows tools:pitch_dump_windows

  mkdir z2music
  cp bazel-bin/tools/*.exe z2music
  cp projects/amida.z2music z2music
  cp bazel-z2music/external/gcc_mxe_mingw64_files/x86_64-w64-mingw32.shared/bin/*.dll z2music

  local zipfile="z2music-$version.zip"
  zip "$zipfile" z2music/*
  rm -rf z2music

  local rpath="/mnt/build/z2music/$(git describe --tags --abbrev=0)"
  ssh build@build.eatabrick.org mkdir -p "$rpath"
  scp "$zipfile" build@build.eatabrick.org:"$rpath"/.
}

make_release "$@"
