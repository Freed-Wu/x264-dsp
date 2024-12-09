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

- [ccstudio](https://aur.archlinux.org/packages/ccstudio)
- [TI C6000 toolchain \< 8.0.0](https://www.ti.com/tool/C6000-CGT): for TI DSP
  DM6467. Go to ccstudio's App center to install
  [TI-CGT 7.4.24](https://dr-download.ti.com/secure/software-development/ide-configuration-compiler-or-debugger/MD-vqU2jj6ibH/7.4.24/ti_cgt_c6000_7.4.24_linux_installer_x86.bin)

For TI C6000 toolchain > 8.0.0, refer <https://github.com/ustc-ivclab/x264>.

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
meson setup build
# or cross compiling for TI DSP. make sure *.cmd in project root
meson setup --cross-file meson/ti-c6000.txt build
meson compile -Cbuild
```

See `meson configure build` to know how to configure.

### xmake

```sh
xmake
```

See `xmake f --menu` to know how to configure.

For input file,

```sh
xmake f --bin2c=yes --input\ filename=/the/path/of/1280x720.yuv
```

**Note**: TI-CGT cannot support too large `/the/path/of/WxH.yuv`!
Otherwise, you will met the following error when `ccs.projectBuild`.

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

## Burn

You must create an arm project to activate DSP core by [gel files](assets/gel).

<!-- markdownlint-disable MD013 -->

```sh
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectCreate -ccs.device ARM9.TMS320DM6467 -ccs.name arm -ccs.template com.ti.common.project.core.emptyProjectWithMainTemplate
```

<!-- markdownlint-disable MD033 -->

1. select project `arm`
2. press <kbd>Alt</kbd> + <kbd>CR</kbd> to open properties

<!-- markdownlint-enable MD033 -->

![project explorer](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/6fe74391-a776-43e4-b2de-aa1434e881e7)

<!-- markdownlint-disable MD033 -->

1. select `Manage the project's target-configuration automatically`
2. change `Connection` according to your debug probe

<!-- markdownlint-enable MD033 -->

![Connection](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/8043be96-a98d-40a0-ab1d-a1cb5b7ef2d9)

<!-- markdownlint-enable MD013 -->

![ccxml](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/54b2e1e3-9e5c-4bb7-b20c-0c6f1954fe24)

1. double click `TMS320DM6467.ccxml`
2. press `Target Configuration`

![Target Configuration](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/361e264e-8e0b-4cfa-8fc9-9e20784a45eb)

<!-- markdownlint-disable MD033 -->

1. select `~/workspace_v12/x264-dsp/assets/gel/davincihd_arm.gel` for ARM926,
   ARM968_0, ARM968_1
2. select `~/workspace_v12/x264-dsp/assets/gel/davincihd_dsp.gel` for C64XP
3. press <kbd>Ctrl</kbd> + <kbd>S</kbd> to save

<!-- markdownlint-enable MD033 -->

![arm](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/b2602ac6-85cd-46ec-9b06-f073b52dc8c9)

![dsp](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/607cc3c1-7efd-4053-8f88-a982f530ef08)

<!-- markdownlint-disable MD033 -->

1. power on DSP
2. press <kbd>F11</kbd> to debug
3. press `OK`

<!-- markdownlint-enable MD033 -->

1. press `OK` to close `Launching Debug Session`

![Launching Debug Session](https://github.com/ustc-ivclab/x264-dsp/assets/32936898/847fcdf8-1313-481c-b9c4-a25f91f14b20)

<!-- markdownlint-disable MD033 -->

1. change project `x264-dsp` on `Project Explorer`
2. press <kbd>F11</kbd> to debug
3. press <kbd>F8</kbd> to run

<!-- markdownlint-enable MD033 -->

Refer [Code Composer Studio User’s Guide](http://software-dl.ti.com/ccs/esd/documents/users_guide/index.html)
to debug.

## Usage

Download a test YUV from
[release](https://github.com/Freed-Wu/x264-dsp/releases).
Note the file name must respect
[YUView filename rules](https://github.com/IENT/YUView/wiki/YUV-File-Names)
to contain resolution.

If you don't use `bin2c`, you must move YUV file to the path which x264 can find.

```sh
# For DSP
mv /the/path/yuv/1280x720.yuv ~/workspace_v12/x264-dsp/Release
# For PC
mv /the/path/yuv/1280x720.yuv .
```

Or tell the precious path:

```sh
the/path/of/x264 /the/path/yuv/1280x720.yuv
```

After running, `out.264` will occur in `~/workspace_v12/x264-dsp/Release` for
DSP and current directory for PC.

You can use `ffplay` to check the correctness of 264 format.

```sh
ffplay /the/path/of/out.264
```

## Documents

- codec
  - [information technology - coding of audio-visual objects: visual ISO/IEC 14496-2 Committee Draft](http://home.mit.bme.hu/~szanto/education/mpeg/14496-2.pdf)
  - [Overview of the H.264 / AVC Video Coding Standard](http://www.h264soft.com/downloads/h264_overview.pdf)
  - [White Paper: H.264 / AVC Intra Prediction](http://www.staroceans.org/e-book/vcodex/H264_intrapred_wp.pdf)
- TI DSP
  - [TMS320C6000 Assembly Language Tools](https://www.ti.com/lit/ug/sprui03e/sprui03e.pdf)
  - [TMS320C6000 Optimizing C/C++ Compile](https://www.ti.com/lit/ug/sprui04e/sprui04e.pdf)
  - [ccs_projects-command-line](https://software-dl.ti.com/ccs/esd/documents/ccs_projects-command-line.html)
