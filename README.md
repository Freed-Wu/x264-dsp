# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

Add an optional downsample module.

## Generate a Source Distribution

In order to generate a source distribution, install:

- [autoconf](https://www.gnu.org/software/autoconf)
- [automake](https://www.gnu.org/software/automake)

Then:

```shell
autoreconf -vif
```

## Build

Download
[a source distribution](https://github.com/Freed-Wu/x264-dsp/releases), then
install:

- [bash](https://www.gnu.org/software/bash)
- [make](https://www.gnu.org/software/make) (ccstudio contains a builtin
  `/opt/ccstudio/ccs/utils/bin/gmake`)

Optional dependencies:

- [check](https://github.com/libcheck/check): for unit test
- [bin2c](https://github.com/adobe/bin2c): for
  `./configure --with-bin2c=/the/path/of/WxH.yuv`

To compile this program for native platform or other platforms, install:

- [gcc](https://gcc.gnu.org)/[clang](https://clang.llvm.org/): for native
  building
- [mingw-w64](https://archlinux.org/packages/community/x86_64/mingw-w64-gcc):
  for windows
- [android-ndk](https://aur.archlinux.org/packages/android-ndk): for android
- [ccstudio](https://aur.archlinux.org/packages/ccstudio): for TI DSP
- [TI C6000 toolchain \< 8.0.0](https://www.ti.com/tool/C6000-CGT): for TI DSP
  DM6467

For OSs:

```shell
# native build
./configure
# or for windows
./configure --build=x86_64-pc-linux-gnu --host=x86_64-w64-mingw32
# or for android, API version is 32
./configure --build=x86_64-pc-linux-gnu --host=aarch64-linux-android32
make
```

For TI DSP DM6467: (Refer
[ccs_projects-command-line](https://software-dl.ti.com/ccs/esd/documents/ccs_projects-command-line.html))

<!-- markdownlint-disable MD013 -->

```shell
# a large heap/stack size to avoid malloc failure
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectCreate -ccs.device TMS320C64XX.TMS320DM6467 -ccs.name x264-dsp -ccs.setCompilerOptions --gcc -ccs.setCompilerOptions -O3 @configurations Release -ccs.setCompilerOptions --program_level_compile @configurations Release -ccs.setCompilerOptions --call_assumptions=3 @configurations Release -ccs.setLinkerOptions -heap=0x1000000 -ccs.setLinkerOptions -stack=0x1000000
cd ~/workspace_v12/x264-dsp
git clone --bare --depth=1 https://github.com/Freed-Wu/x264-dsp .git
git config core.bare false
git reset --hard
autoreconf -vif
./configure --with-bin2c=/the/path/of/1280x720.yuv
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectBuild -ccs.projects x264-dsp -ccs.configuration Release
```

<!-- markdownlint-enable MD013 -->

For TI C6000 toolchain > 8.0.0, refer <https://github.com/Freed-Wu/x264>.

## Configure

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

## Usage

Download a test YUV from
[release](https://github.com/Freed-Wu/x264-dsp/releases).
Note the file name must respect
[YUView filename rules](https://github.com/IENT/YUView/wiki/YUV-File-Names)
to contain resolution.

[Build](#build) a `x264`.

For OSs:

```shell
./x264 /the/path/yuv/1280x720.yuv
```

After running, `out.264` will occur in current directory.

For DSP:

```shell
mv /the/path/yuv/1280x720.yuv ~/workspace_v12/x264-dsp/Release
```

Then `Run -> Load -> Select Program to Load`, select
`~/workspace_v12/x264-dsp/Release/x264-dsp.out`.

After running, `out.264` will occur in `~/workspace_v12/x264-dsp/Release`.

```shell
ffplay /the/path/of/out.264
```
