# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

Add an optional downsample module.

## Dependencies

## Build Systems

You have 2 methods to build this project:

For `autotools`:

Download
[a source distribution](https://github.com/Freed-Wu/x264-dsp/releases), then
install:

- [bash](https://www.gnu.org/software/bash)
- [make](https://www.gnu.org/software/make) (ccstudio contains a builtin
  `/opt/ccstudio/ccs/utils/bin/gmake`)

For `cmake`:

- [cmake](https://github.com/Kitware/CMake)
- [one generator of cmake](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)

### Toolchains

### Host Builds

One of the following:

- [gcc](https://gcc.gnu.org)
- [clang](https://clang.llvm.org/)
- [MSVC](https://visualstudio.microsoft.com/vs/features/cplusplus)

### Cross Compiling

- [mingw-w64](https://archlinux.org/packages/community/x86_64/mingw-w64-gcc):
  for windows
- [android-ndk](https://aur.archlinux.org/packages/android-ndk): for android
- [ccstudio](https://aur.archlinux.org/packages/ccstudio): for TI DSP
- [TI C6000 toolchain \< 8.0.0](https://www.ti.com/tool/C6000-CGT): for TI DSP
  DM6467. Go to ccstudio's App center to install TI-CGT 7.4.24

For TI C6000 toolchain > 8.0.0, refer <https://github.com/Freed-Wu/x264>.

### Optional Dependencies

- [check](https://github.com/libcheck/check): for unit test
  - `make check`
  - `ctest`
- [bin2c](https://github.com/adobe/bin2c): use bin2c to convert a yuv to
  a c array
  - `./configure --with-bin2c=/the/path/of/WxH.yuv`
  - `cmake -DBIN2C=ON -DINPUT_FILENAME=/the/path/of/WxH.yuv`

## Build

### autotools

For OSs:

```shell
mkdir build
cd build
# host build
../configure
# or cross compiling for windows
../configure --build=x86_64-pc-linux-gnu --host=x86_64-w64-mingw32
# or cross compiling for android with API 21
../configure --build=x86_64-pc-linux-gnu --host=aarch64-linux-android21
make -j$(nproc)
```

See `--help` to know how to configure:

```shell
$ ./configure --help
...
  --enable-debug          enable debug. default=no

  --enable-asm            enable TI C6X asm. default=no

  --enable-dry-run        enable dry run, do not write any file. default=no
...
  --with-x264-bit-depth[=8|10]
                          bit depth. default=8

  --with-x264-chroma-format[=0..3]
                          chroma format: 400, 420, 422, 444. default=1

  --with-x264-log-level[=0..3]
                          log level: error, warning, info, debug. debug will
                          decrease fps. default=2

  --with-bin2c[=/the/path/of/WxH.yuv]
                          use bin2c to convert a yuv to yuv.h

  --with-downsample[=1|2] downsample from 720p to 360p, 1, 2 means bilinear,
                          bicubic. default=1

  --with-downsample-scale[=X]
                          downsample scale. default=2

  --with-padding[=1..3]   padding method, edge, reflect, symmetric. default=3
...
```

`autotools` doesn't support TI-CGT.

### cmake

```shell
# host build
cmake -Bbuild
# or cross compiling for windows
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/mingw.cmake
# or cross compiling for windows on x86
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/mingw.cmake -DCMAKE_SYSTEM_PROCESSOR=i686
# or cross compiling for android with highest API
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/android-ndk.cmake
# or cross compiling for TI DSP
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=cmake/ti.cmake
cmake --build build
```

See `ccmake -Bbuild` to know how to configure.

### ccstudio

<!-- markdownlint-disable MD013 -->

```shell
# a large heap/stack size to avoid malloc failure
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectCreate -ccs.device TMS320C64XX.TMS320DM6467 -ccs.name x264-dsp -ccs.setCompilerOptions --gcc -ccs.setCompilerOptions -O3 @configurations Release -ccs.setCompilerOptions --program_level_compile @configurations Release -ccs.setCompilerOptions --call_assumptions=3 @configurations Release -ccs.setLinkerOptions -heap=0x1000000 -ccs.setLinkerOptions -stack=0x1000000
cd ~/workspace_v12/x264-dsp
git clone --bare --depth=1 https://github.com/Freed-Wu/x264-dsp .git
git config core.bare false
git reset --hard
# use autotools to generate config.h and yuv.h
autoreconf -vif
./configure --with-bin2c=/the/path/of/1280x720.yuv --with-downsample --with-downsample-scale=4
# or use cmake
cmake -B. -DBIN2C=ON -DINPUT_FILENAME=/the/path/of/1280x720.yuv -DDOWNSAMPLE=1 -DSCALE=4
cmake --build . --target yuv.h
rm -r CMakeFiles
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectBuild -ccs.projects x264-dsp -ccs.configuration Release
```

<!-- markdownlint-enable MD013 -->

You will get `Release/x264-dsp.out`.

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

```shell
head -c13824000 /720p/the/path/of/WxH.yuv > /the/path/of/WxH.yuv
```

## Burn

You must create an arm project to activate DSP core by [gel files](assets/gel).

<!-- markdownlint-disable MD013 -->

```shell
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

```shell
# For DSP
mv /the/path/yuv/1280x720.yuv ~/workspace_v12/x264-dsp/Release
# For PC
mv /the/path/yuv/1280x720.yuv .
```

Or tell the precious path:

```shell
the/path/of/x264 /the/path/yuv/1280x720.yuv
```

After running, `out.264` will occur in `~/workspace_v12/x264-dsp/Release` for
DSP and current directory for PC.

You can use `ffplay` to check the correctness of 264 format.

```shell
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
