#!/usr/bin/env bash
set -e
cd "$(dirname "$(dirname "$(readlink -f "$0")")")"

# a large heap/stack size to avoid malloc failure
# modify include path by your machine
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectCreate -ccs.device TMS320C64XX.TMS320DM6467 -ccs.name x264-dsp -ccs.setCompilerOptions --gcc -ccs.setCompilerOptions -O3 @configurations Release -ccs.setCompilerOptions '-I${PROJECT_ROOT}/build' -ccs.setCompilerOptions '-I${PROJECT_ROOT}/build/.gens/x264-dsp/linux/x86_64/release/rules/utils/bin2c' -ccs.setCompilerOptions --program_level_compile @configurations Release -ccs.setCompilerOptions --call_assumptions=3 @configurations Release -ccs.setLinkerOptions -heap=0x1000000 -ccs.setLinkerOptions -stack=0x1000000
cp -r .git ~/workspace_v12/x264-dsp
git -C ~/workspace_v12/x264-dsp reset --hard
