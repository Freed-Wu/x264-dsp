#!/usr/bin/env bash
set -e
cd "$(dirname "$(dirname "$(readlink -f "$0")")")"

mkdir -p targetConfigs/gel/build
find "$PWD" -wholename "$PWD/targetConfigs/gel/*.gel" |
  scripts/generate-compile_commands.json-for-gel.jq >targetConfigs/gel/build/compile_commands.json
