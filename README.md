# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

Add an optional downsample module.

## Dependencies

### Build Systems

- [meson](https://mesonbuild.com): version must be after
  [this PR](https://github.com/mesonbuild/meson/pull/13989)
- [one backend of meson](https://mesonbuild.com/Builtin-options.html#core-options)

### Toolchains

- [TI-CGT 7.4.24](https://dr-download.ti.com/secure/software-development/ide-configuration-compiler-or-debugger/MD-vqU2jj6ibH/7.4.24/ti_cgt_c6000_7.4.24_linux_installer_x86.bin):
  Download it needs a TI account to log in.
  If you don't want register, download
  [this backup](https://github.com/ustc-ivclab/x264-dsp/releases/download/0.0.1/c6000_7.4.24.7z).
  8.0.0 doesn't support TI DSP DM6467.
  Refer [x264-for-TI-CGT-8.0.0](https://github.com/ustc-ivclab/x264)
- [ccstudio](https://aur.archlinux.org/packages/ccstudio): for burn.
  Default install path is `/opt/ccstudio/ccs`

### Optional Dependencies

- [check](https://github.com/libcheck/check): for unit test: `meson test -Cbuild`

## Build

```sh
meson setup --cross-file meson/ti-c6000.txt build
meson compile -Cbuild
```

See `meson configure build` to know how to configure.

Default TI linker command file is `/opt/ccstudio/ccs/ccs_base/c6000/include/DM6467.cmd`.
You can download [it](https://github.com/ustc-ivclab/x264-dsp/releases/download/0.0.1/DM6467.cmd)
and modify for your requirements.

```sh
meson setup --cross-file meson/ti-c6000.txt build -Dcmd_file=/the/path/of/my/DM6467.cmd
```

## Burn and Run

Default debugger is XDS100v3. See [targetConfigs](targetConfigs) to modify it.

```sh
# load /the/path/of/352x288.yuv to DDR2 and run:
#   build/x264.out --input=352x288.yuv --frames=10
scripts/burn.js /the/path/of/352x288.yuv -- build/x264.out --input=352x288.yuv --frames=10
ffplay build/out.264
```

Some [test yuv files](https://github.com/ustc-ivclab/x264-dsp/releases).

## Usage

See `--help`:

```sh
sed -i -e"s/, 'linearasm'//g" meson.build
meson setup build/host
meson compile -Cbuild/host
build/host/x264 --help
```

Here are some details:

### --disable-ddr-input/--disable-ddr-output

There are two methods to pass input/output file to/from TI DSP:

- CIO. for only debug, allow TI DSP read/write host machine filesystem, we can
  get frame number from file size
- `loadRaw()/saveRaw()`. load/save a file from/to DDR2. We must tell frame
  number

By default, in TI DSP, it will enable DDR input/output. In PC, it always
read/write from itself's filesystem.

```sh
$ scripts/burn.js -- build/x264.out --input=../../352x288.yuv --disable-ddr-input
# ...
../../352x288.yuv [info]: 352x288p 0:0 @ 25/1 fps (cfr)
x264 [info]: profile Constrained Baseline, level 1.3
x264 [info]: frame I:1     Avg QP:29.00  size:  5403
encoded 1 frames, 0.02 fps, 1080.60 kb/s
$ scripts/burn.js ../352x288.yuv -- build/x264.out --input=../../352x288.yuv --frames=1
# ...
../../352x288.yuv [info]: 352x288p 0:0 @ 25/1 fps (cfr)
x264 [info]: profile Constrained Baseline, level 1.3
x264 [info]: frame I:1     Avg QP:29.00  size:  5403
encoded 1 frames, 0.50 fps, 1080.60 kb/s
```

### --input/--output

In PC, related path is based on `$PWD`. In TI DSP, related path is based on the
directory of `x264.out`.

```sh
$ build/host/x264 --input=../352x288.yuv
../352x288.yuv [info]: 352x288p 0:0 @ 25/1 fps (cfr)
x264 [info]: profile Constrained Baseline, level 1.3
x264 [info]: frame I:1     Avg QP:29.00  size:  5403
encoded 1 frames, inf fps, 1080.60 kb/s
# add an extra ../ because build/x264.out is in build/
$ scripts/burn.js -- build/x264.out --input=../../352x288.yuv --disable-ddr-input
# ...
```

If you use DDR input, it will ignore `--input`. however, `x264` get
width and height from `--input`'s `WxH`. So the following commands are same:

```sh
scripts/burn.js ../352x288.yuv -- build/x264.out --input=../../352x288.yuv --frames=1
scripts/burn.js ../352x288.yuv -- build/x264.out --input=foo352x288bar.yuv --frames=1
```

`scripts/burn.js ../352x288.yuv` will `loadRaw()` `../352x288.yuv` to DDR.
If the power is on, the step only need to be done once:

```sh
scripts/burn.js ../352x288.yuv
scripts/burn.js -- build/x264.out --input=../../352x288.yuv --frames=1
```

## Documents

- codec
  - [information technology - coding of audio-visual objects: visual ISO/IEC 14496-2 Committee Draft](http://home.mit.bme.hu/~szanto/education/mpeg/14496-2.pdf)
  - [Overview of the H.264 / AVC Video Coding Standard](http://www.h264soft.com/downloads/h264_overview.pdf)
  - [White Paper: H.264 / AVC Intra Prediction](http://www.staroceans.org/e-book/vcodex/H264_intrapred_wp.pdf)
- TI DSP
  - TMS320C6000 Assembly Language Tools:
    - [PDF](https://www.ti.com/lit/ug/sprui03e/sprui03e.pdf)
    - [HTML](https://downloads.ti.com/docs/esd/SPRUI03/)
  - TMS320C6000 Optimizing C/C++ Compile
    - [PDF](https://www.ti.com/lit/ug/sprui04e/sprui04e.pdf)
    - [HTML](https://downloads.ti.com/docs/esd/SPRUI04/)
  - [Code Composer Studio™ User's Guide](https://downloads.ti.com/ccs/esd/documents/users_guide/)
  - [TI Linker Command File Primer](https://software-dl.ti.com/ccs/esd/documents/sdto_cgt_Linker-Command-File-Primer.html)
