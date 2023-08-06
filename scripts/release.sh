#!/bin/bash

set -e

make_release() {
  local version=$(git describe --tags --long)
  local short=$(git describe --tags --abbrev=0)

  echo "Building release for version $version"

  bazel test :all
  bazel build -c opt :release

  local rpath="/mnt/build/z2music/$short"
  ssh build@build.eatabrick.org mkdir -p "$rpath"
  scp "bazel-bin/release.zip" build@build.eatabrick.org:"$rpath/z2music-$version.zip"
}

make_release "$@"
