#!/usr/bin/env bash
#? v0.0.1
##? usage: get-psnr.sh [options]
##?
##? options:
##?   -h, --help            Show this screen.
##?   -V, --version         Show version.
help=$(grep "^##?" "$0" | cut -c 5-)
version=$(grep "^#?" "$0" | cut -c 4-)
eval "$(docopts -h "$help" -V "$version" : "$@")"
dirs=(mpeg4/{3,6}00k h264/{3,6}00k logs/{mpeg4,h264}/{3,6}00k)
for dir in "${dirs[@]}"; do
  [[ -d $dir ]] || mkdir -p "$dir"
done
# average bitrate = 300k
parallel --link "ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -c:v libxvid -preset slow -bf 0 -b:v {2}k mpeg4/300k/{1/.}.mp4 \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -i mpeg4/300k/{1/.}.mp4 -lavfi psnr=stats_file=logs/mpeg4/300k/{1/.}.log -f null - \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -c:v libx264 -preset slow -bf 0 -b:v {3}k h264/300k/{1/.}.mp4 \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -i h264/300k/{1/.}.mp4 -lavfi psnr=stats_file=logs/h264/300k/{1/.}.log -f null -" \
  ::: yuv/crew_cif.yuv yuv/deadline_cif_300.yuv yuv/harbour_cif.yuv yuv/pamphlet_cif_300.yuv yuv/paris_cif_300.yuv yuv/silent_cif_300.yuv yuv/students_cif_300.yuv \
  ::: 245 235 229 245 230 239 239 \
  ::: 286 316 304 294 318 303 315
# average bitrate = 600k
parallel --link "ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -c:v libxvid -preset slow -bf 0 -b:v {2}k mpeg4/600k/{1/.}.mp4 \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -i mpeg4/600k/{1/.}.mp4 -lavfi psnr=stats_file=logs/mpeg4/600k/{1/.}.log -f null - \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -c:v libx264 -preset slow -bf 0 -b:v {3}k h264/600k/{1/.}.mp4 \
  && ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i yuv/{1/.}.yuv -i h264/600k/{1/.}.mp4 -lavfi psnr=stats_file=logs/h264/600k/{1/.}.log -f null -" \
  ::: yuv/crew_cif.yuv yuv/deadline_cif_300.yuv yuv/harbour_cif.yuv yuv/pamphlet_cif_300.yuv yuv/paris_cif_300.yuv yuv/silent_cif_300.yuv yuv/students_cif_300.yuv \
  ::: 540 500 480 545 476 510 515 \
  ::: 578 628 605 593 632 609 617
# check average bitrate
exiftool mpeg4/*/*.mp4 h264/*/*.mp4 | rg '========|Avg Bitrate' | sed ':a;N;$!ba;s/\nAvg Bitrate\s*//g' | cut -d' ' -f2- | xsv table -d:
