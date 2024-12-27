# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

Add an optional downsample module.

## Dependencies

### Build Systems

- [meson](https://mesonbuild.com)
- [one backend of meson](https://mesonbuild.com/Builtin-options.html#core-options)

### Toolchains

- [TI-CGT 7.4.24](https://dr-download.ti.com/secure/software-development/ide-configuration-compiler-or-debugger/MD-vqU2jj6ibH/7.4.24/ti_cgt_c6000_7.4.24_linux_installer_x86.bin):
  8.0.0 doesn't support TI DSP DM6467. Refer
  [x264-for-TI-CGT-8.0.0](https://github.com/ustc-ivclab/x264)
- [ccstudio](https://aur.archlinux.org/packages/ccstudio): for burn.
  Default install path is `/opt/ccstudio/ccs`

### Optional Dependencies

- [check](https://github.com/libcheck/check): for unit test
  - `meson test -Cbuild`

## Build

```sh
meson setup --cross-file meson/ti-c6000.txt build
meson compile -Cbuild
```

See `meson configure build` to know how to configure.

## Burn

Default debugger is XDS100v3. See [targetConfigs](targetConfigs) to modify it.

```sh
# load /the/path/of/352x288.yuv to DDR2 and run:
#   build/x264.out --input=352x288.yuv --frames=10
scripts/burn.js /the/path/of/352x288.yuv -- build/x264.out --input=352x288.yuv --frames=10
ffplay build/out.264
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
