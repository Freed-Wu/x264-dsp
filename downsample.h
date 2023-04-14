#ifndef DOWNSAMPLE_H
#define DOWNSAMPLE_H 1
#include <stddef.h>
#include <stdint.h>
#include "config.h"

#define DOWNSAMPLE_BILINEAR 0
#define DOWNSAMPLE_BICUBIC 1
// https://pytorch.org/vision/stable/generated/torchvision.transforms.Pad.html
#define PADDING_EDGE 1
#define PADDING_REFLECT 2
// matlab's imresize()
#define PADDING_SYMMETRIC 3

#if PADDING == PADDING_SYMMETRIC
#define PAD SYMMETRIC
#elif PADDING == PADDING_REFLECT
#define PAD REFLECT
#elif PADDING == PADDING_EDGE
#define PAD CLAMP
#endif

#define SYMMETRIC(input, min, max)                      \
	do {                                            \
		__typeof__(input) _min = (min);         \
		__typeof__(input) _max = (max);         \
		__typeof__(input) _input = (input);     \
		if (_input < _min)                      \
			_input = 2 * _min - _input - 1; \
		if (_input > _max)                      \
			_input = 2 * _max - _input + 1; \
		(input) = _input;                       \
	} while (0)
#define REFLECT(input, min, max)                    \
	do {                                        \
		__typeof__(input) _min = (min);     \
		__typeof__(input) _max = (max);     \
		__typeof__(input) _input = (input); \
		if (_input < _min)                  \
			_input = 2 * _min - _input; \
		if (_input > _max)                  \
			_input = 2 * _max - _input; \
		(input) = _input;                   \
	} while (0)
#define CLAMP(input, min, max)                      \
	do {                                        \
		__typeof__(input) _min = (min);     \
		__typeof__(input) _max = (max);     \
		__typeof__(input) _input = (input); \
		if (_input < _min)                  \
			_input = _min;              \
		if (_input > _max)                  \
			_input = _max;              \
		(input) = _input;                   \
	} while (0)
double cubic_hermite(double f_1, double f0, double f1, double f2, double t);
uint8_t sample_bicubic(uint8_t *src, size_t src_width, size_t src_height, double u, double v);
void resize(void *dst, const void *src, size_t dst_width, size_t dst_height);
void resize2(void *dst, const void *src, size_t dst_width, size_t dst_height);

#endif /* downsample.h */
