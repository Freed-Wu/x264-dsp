# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

Add an optional downsample module.

## Dependencies

### Build Systems

For `meson`:

- [meson](https://mesonbuild.com)
- [one backend of meson](https://mesonbuild.com/Builtin-options.html#core-options)

For `xmake`:

- [xmake](https://xmake.io)

### Toolchains

- [TI-CGT 7.4.24](https://dr-download.ti.com/secure/software-development/ide-configuration-compiler-or-debugger/MD-vqU2jj6ibH/7.4.24/ti_cgt_c6000_7.4.24_linux_installer_x86.bin):
  8.0.0 doesn't support TI DSP DM6467. Refer
  [x264-for-TI-CGT-8.0.0](https://github.com/ustc-ivclab/x264)
- [ccstudio](https://aur.archlinux.org/packages/ccstudio): for burn.
  Default install path is `/opt/ccstudio/ccs`

### Optional Dependencies

- [check](https://github.com/libcheck/check): for unit test
  - `meson test -Cbuild`
- [bin2c](https://github.com/adobe/bin2c): use bin2c to convert a yuv to
  a c array. Or use xmake's builtin
  [bin2c](https://xmake.io/#/manual/custom_rule?id=utilsbin2c).
  - `meson setup build -Dbin2c=true -Dinput_filename=/the/path/of/WxH.yuv`
  - `xmake f --bin2c=y`

## Build

### meson

```sh
meson setup --cross-file meson/ti-c6000.txt build
meson compile -Cbuild
meson install -Cbuild
```

See `meson configure build` to know how to configure.

### xmake

```sh
xmake
xmake install
```

See `xmake f --menu` to know how to configure.

## Burn

Default debugger is XDS100v3. See [targetConfigs](targetConfigs) to modify it.

```sh
scripts/burn.js /the/path/of/WxH.yuv
ffplay out.264
```

## Tips

```sh
xmake f --bin2c=yes --input\ filename=/the/path/of/1280x720.yuv
```

**Note**: TI-CGT cannot support too large `/the/path/of/WxH.yuv`:

```text
terminate called after throwing an instance of 'std::bad_alloc'
  what():  St9bad_alloc

INTERNAL ERROR: /opt/ccstudio/ccs/tools/compiler/c6000_7.4.24/bin/ilk6x aborted while
                processing file /tmp/TIXXXXXXXXX

This is a serious problem.  Please contact Customer
Support with this message and a copy of the input file
and help us to continue to make the tools more robust.
```

Try to reduce the size of YUV file:

```sh
head -c13824000 /720p/the/path/of/WxH.yuv > /the/path/of/WxH.yuv
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
