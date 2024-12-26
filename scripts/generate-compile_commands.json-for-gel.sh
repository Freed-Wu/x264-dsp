#!/usr/bin/env bash
set -e
cd "$(dirname "$(dirname "$(readlink -f "$0")")")"

mkdir -p assets/gel/build
find "$PWD" -wholename "$PWD/assets/gel/*.gel" |
  scripts/generate-compile_commands.json-for-gel.jq >assets/gel/build/compile_commands.json
