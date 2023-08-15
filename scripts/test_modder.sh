#!/bin/sh

set -e

PROJECT=amida
CORE=nestopia

function test() {
  local input="$1"
  local output="$(mktemp)"

  bazel run -c opt tools:modder -- $(pwd)/projects/$PROJECT.z2music --rom "$1" --output "$output" --minloglevel 1
  retroarch -L /usr/lib/libretro/${CORE}_libretro.so "$output"
}

test "$@"
