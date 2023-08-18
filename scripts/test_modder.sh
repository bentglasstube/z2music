#!/bin/sh

set -e

PROJECT=amida
CORE=nestopia

function test() {
  local input="$1"
  local output="$(mktemp)"

  if [ -z "$input" ]; then
    echo "Usage: $0 <romfile>" >&2
    exit 1
  fi

  bazel run -c opt tools:modder -- $(pwd)/projects/$PROJECT.z2music --rom "$1" --output "$output" --minloglevel 1
  fceux "$output"
}

test "$@"
