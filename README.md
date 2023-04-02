# x264-dsp

This is a complete implementation of H.264 codec ported for TI C6000 DSP.
By using linear assembly language, we can take full advantage of DSP
architecture.
We tried to optimize several core routines, including DCT, Intra-Predict,
Inter-Predict, CABAC, etc.

## Generate a Source Distribution

In order to generate a source distribution, install:

- autoconf
- automake

Then:

```shell
autoreconf -vif
```

## Build

Download
[a source distribution](https://github.com/Freed-Wu/x264-dsp/releases), then
install:

- bash
- make (ccstudio contains a builtin make)

To compile this program for native platform or other platforms, install:

- gcc/clang for native building
- [mingw-w64](https://archlinux.org/packages/community/x86_64/mingw-w64-gcc)
  for windows
- [android-ndk](https://aur.archlinux.org/packages/android-ndk) for android
- [ccstudio](https://aur.archlinux.org/packages/ccstudio) for TI DSP
- [TI C6000 toolchain \< 8.0.0](https://www.ti.com/tool/C6000-CGT) for TI DSP
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

For TI DSP DM6467:

<!-- markdownlint-disable MD013 -->

```shell
# generate config.h
./configure
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectCreate -ccs.device TMS320C64XX.TMS320DM6467 -ccs.name x264-dsp -ccs.setCompilerOptions --gcc
cp -r .git ~/workspace_v12/x264-dsp
git -C ~/workspace_v12/x264-dsp reset --hard
ccstudio -noSplash -data ~/workspace_v12 -application com.ti.ccstudio.apps.projectBuild -ccs.projects x264-dsp -ccs.configuration Debug
```

<!-- markdownlint-enable MD013 -->

For TI C6000 toolchain > 8.0.0, refer <https://github.com/Freed-Wu/x264>.
