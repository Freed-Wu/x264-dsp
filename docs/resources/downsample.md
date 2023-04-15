# Downsample

<https://github.com/Freed-Wu/x264-dsp/tree/main/scripts> provides `matlab`
(`octave`), `opencv`, `pytorch`'s results of bilinear and bicubic. The bilinear
results are same. The bicubic results are not same. Because bicubic depends on
two factors: `a` and padding method.

## a

![cubic](https://wikimedia.org/api/rest_v1/media/math/render/svg/7835668875c7c0284063c9f2eafc3b1022692e03)

- `matlab` (`octave`)'s `a` is -0.5.
- `opencv` and `pytorch`'s `a` is -0.75.
- We use `-0.5` because it has a fast algorithm:

![a](https://wikimedia.org/api/rest_v1/media/math/render/svg/32d289bca511910631e1bc21aad22088fdb62283)

## Padding Method

[Padding method document](https://pytorch.org/vision/stable/generated/torchvision.transforms.Pad.html).

- `matlab` (`octave`) use symmetric padding.
- `opencv` and `pytorch` use reflect padding.
- We use symmetric padding by default. You can change it by
  `./configure --padding=X`.

## Unit Test

Install [`check`](https://github.com/libcheck/check), then:

```shell
$ make check
...
PASS: check_downsample
============================================================================
Testsuite summary for x264 UNKNOWN
============================================================================
# TOTAL: 1
# PASS:  1
# SKIP:  0
# XFAIL: 0
# FAIL:  0
# XPASS: 0
# ERROR: 0
============================================================================
...
$ tests/check_downsample
Running suite(s): Core
100%: Checks: 5, Failures: 0, Errors: 0
```

## Related Projects

- [rrepka10/imgResample](https://github.com/rrepka10/imgResample):
  A `C` implementation of bicubic.
  [Some bug](https://github.com/rrepka10/imgResample/issues/2) exists.
- [sanghyun-son/bicubic_pytorch](https://github.com/sanghyun-son/bicubic_pytorch):
  A `pytorch` implementation which is compatible with `matlab`'s `imresize()`'s
  API.
