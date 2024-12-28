#!/usr/bin/env bash
set -e
cd "$(dirname "$(dirname "$(readlink -f "$0")")")"

mkdir -p targetConfigs/build
find "$PWD" -wholename "$PWD/targetConfigs/*.gel" |
  scripts/generate-compile_commands.json.jq >targetConfigs/build/compile_commands.json
