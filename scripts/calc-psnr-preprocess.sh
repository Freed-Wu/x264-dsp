#!/usr/bin/env bash
#? v0.0.1
##? usage: calc-psnr.sh [options]
##?
##? options:
##?   -h, --help            Show this screen.
##?   -V, --version         Show version.
help=$(grep "^##?" "$0" | cut -c 5-)
version=$(grep "^#?" "$0" | cut -c 4-)
eval "$(docopts -h "$help" -V "$version" : "$@")"
dirs=(logs/{mpeg4,h264}/{3,6}00k)
for dir in "${dirs[@]}"; do
  [[ -d "tmp/$dir" ]] || mkdir -p "tmp/$dir"
done
parallel 'cat {} | tr -d a-z_: > tmp/{}' ::: logs/**/*.log
