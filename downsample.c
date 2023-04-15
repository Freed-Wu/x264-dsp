/**
 * https://github.com/rrepka10/imgResample
 * https://github.com/sanghyun-son/bicubic_pytorch
 */
#include <stdlib.h>
#include <math.h>
#include "downsample.h"

double cubic(double x, double a) {
	x = fabs(x);
	double weight = 0;
	if (x < 1)
		weight = (a + 2) * x * x * x - (a + 3) * x * x + 1;
	else if (x < 2)
		weight = a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a;
	return weight;
}

double sample_cubic(double *src, size_t len, double pos) {
	int i;
	double sum = 0;
	double weights = 0;
	// left of pos: -1..0, right of pos: 1..2
	for (i = -1; i < 3; i++) {
		ssize_t base = (size_t)pos + i;
		double dist = pos - base;
		double weight = cubic(dist, -0.5);
		PAD(base, 0, len);
		sum += weight * (*(src + base));
		weights += weight;
	}
	sum /= weights;
	return sum;
}

/**
 * \param u the coordination of unknown pixel
 * \param v the coordination of unknown pixel
 */
uint8_t sample_bicubic2(uint8_t *_src, size_t src_width, size_t src_height, double u, double v) {
	double *src = (double *)malloc(sizeof(double) * src_width * src_height);
	double *src2 = src;
	int i;
	// convert uint8_t to double
	for (i = 0; i < src_width * src_height; i++)
		*(src2++) = (double)*(_src++);
	double *cols = (double *)malloc(sizeof(double) * src_height);
	double *cols2 = cols;
	for (i = 0; i < src_height; i++) {
		*(cols2++) = sample_cubic(src, src_width, u);
		src += src_width;
	}
	double result = sample_cubic(cols, src_width, v);
	return (uint8_t)rint(result);
}

/**
 * fast algorithm for bicubic when a = -0.5
 *
 * https://en.wikipedia.org/wiki/Bicubic_interpolation#Bicubic_convolution_algorithm
 */
double cubic_hermite(double f_1, double f0, double f1, double f2, double t) {
	double a0 = 2 * f0;
	double a1 = -f_1 + f1;
	double a2 = 2 * f_1 - 5 * f0 + 4 * f1 - f2;
	double a3 = -f_1 + 3 * f0 - 3 * f1 + f2;
	return (((a3 * t + a2) * t + a1) * t + a0) / 2;
}

uint8_t sample_bicubic(uint8_t *src, size_t src_width, size_t src_height, double x, double y) {
	double xfract = x - (int)x;
	double yfract = y - (int)y;
	int yint = (int)y - 1, xint;
	uint8_t p[4][4];
	int i, j;
	// C-order is row-major order
	for (j = 0; j < 4; j++) {
		int yy = yint;
		PAD(yy, 0, src_height - 1);
		// reset xint
		xint = (int)x - 1;
		for (i = 0; i < 4; i++) {
			int xx = xint;
			PAD(xx, 0, src_width - 1);
			p[j][i] = *(src + xx + yy * src_width);
			xint++;
		}
		yint++;
	}
	double col[] = {
	    cubic_hermite(p[0][0], p[0][1], p[0][2], p[0][3], xfract),
	    cubic_hermite(p[1][0], p[1][1], p[1][2], p[1][3], xfract),
	    cubic_hermite(p[2][0], p[2][1], p[2][2], p[2][3], xfract),
	    cubic_hermite(p[3][0], p[3][1], p[3][2], p[3][3], xfract),
	};
	double value = cubic_hermite(col[0], col[1], col[2], col[3], yfract);
	CLAMP(value, 0, 255);
	return (uint8_t)(value + 0.5);
}

void resize(void *_dst, const void *_src, size_t dst_width, size_t dst_height) {
	size_t x, y;
	uint8_t *src = (uint8_t *)_src;
	uint8_t *dst = (uint8_t *)_dst;
	size_t src_width = 2 * dst_width, src_height = 2 * dst_height;
	for (y = 0; y < dst_height; y++) {
		// don't align corner
		double v = (y + 0.5) / dst_height * src_height - 0.5;
		for (x = 0; x < dst_width; x++) {
			double u = (x + 0.5) / dst_width * src_width - 0.5;
			*(dst++) = sample_bicubic(src, src_width, src_height, u, v);
		}
	}
}

/**
 * fast algorithm for bilinear when scale is 0.5
 */
void resize2(void *_dst, const void *_src, size_t dst_width, size_t dst_height) {
	size_t x, y;
	uint8_t *src = (uint8_t *)_src;
	uint8_t *dst = (uint8_t *)_dst;
	size_t src_width = 2 * dst_width;
	for (y = 0; y < dst_height; y++) {
		for (x = 0; x < dst_width; x++) {
			*(dst++) = (*(src) + *(src + 1) + *(src + src_width) + *(src + src_width + 1) + 2) / 4;
			src += 2;
		}
		src += src_width;
	}
}
