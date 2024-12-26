#!/usr/bin/env bash
yuvname=${1-Capture00006-downsample-352x288.yuv}
preset=(ultrafast veryslow)
bitrates=(300 600)
profiles=(baseline main)
x264name="${yuvname%%.yuv}-{3}-{2}-{1}.mp4"
mpegname="${yuvname%%.yuv}-mpeg4-{1}.mp4"

cmd="x264 --tune psnr --psnr --bitrate {1} --preset {2} --profile {3} --input-res=352x288 '$yuvname' -o '$x264name'"
parallel "hr; hyperfine --command-name='$x264name' '$cmd'; $cmd 2>&1|rg -N PSNR" ::: "${bitrates[@]}" ::: "${preset[@]}" ::: "${profiles[@]}"
cmd1="ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i '$yuvname' -c:v libxvid -bf 0 -b:v {1}k -bf 0 '$mpegname'"
cmd2="echo -n PSNR Mean:; ffmpeg -y -v 0 -s cif -pix_fmt yuv420p -i $yuvname -i '$mpegname' -lavfi psnr=stats_file=- -f null -|cut -d' ' -f6|cut -d: -f2|average"
parallel "hr; hyperfine --command-name='$mpegname' '$cmd1' && $cmd2" ::: "${bitrates[@]}"
