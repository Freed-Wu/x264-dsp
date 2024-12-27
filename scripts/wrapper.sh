#!/usr/bin/env bash
set -e
cd "$(dirname "$(dirname "$(readlink -f "$0")")")"

exec scripts/burn.js -- "$@"
