#!/bin/sh
set -e
set -x

function dump_song() {
  local file="$1"
  local song="$2"
  local rom="$HOME/$file.nes"
  local project="projects/$file.z2music"
  bazel-bin/tools/music_dump --rom $rom --song $song >>$project
}

function dump() {
  local file="$1"
  local project="projects/$file.z2music"

  local songs=$(grep ^song $project |cut -c6-)
  rm $project
  for song in $songs; do
    dump_song $file $song
  done
}

bazel build -c opt tools:music_dump
dump 'amida'
dump 'rog'
dump 'benthic_king'
dump 'solstice'
