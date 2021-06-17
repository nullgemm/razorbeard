#include "razorbeard_math.h"

#include <stdint.h>

// log2(size of the log2 look-up table)
#define LOG_LOG2_LUT_LEN 6
// size of the log2 look-up table
#define LOG_LUT_LEN ((1 << LOG_LOG2_LUT_LEN) + 2)
// magic rounding number
#define LOG_MAGIC 0x10D

static const uint8_t zeros_lut[64] =
{
32, 20, 19, -1, -1, 18, -1,  7,
10, 17, -1, -1, 14, -1,  6, -1,
-1,  9, -1, 16, -1, -1,  1, 26,
-1, 13, -1, -1, 24,  5, -1, -1,
-1, 21, -1,  8, 11, -1, 15, -1,
-1, -1, -1,  2, 27,  0, 25, -1,
22, -1, 12, -1, -1,  3, 28, -1,
23, -1,  4, 29, -1, -1, 30, 31,
};

// for (int i = 0; i <= 65; ++i) {log2_lut[i] = log2(1 + (i / 64)) * (1 << 31);}
static const uint32_t log2_lut[LOG_LUT_LEN] =
{
0x00000000, 0x02dcf2d1, 0x05aeb4dd, 0x08759c50, 
0x0b31fb7d, 0x0de42120, 0x108c588d, 0x132ae9e2, 
0x15c01a3a, 0x184c2bd0, 0x1acf5e2e, 0x1d49ee4c, 
0x1fbc16b9, 0x22260fb6, 0x24880f56, 0x26e2499d, 
0x2934f098, 0x2b803474, 0x2dc4439b, 0x30014ac6, 
0x32377512, 0x3466ec15, 0x368fd7ee, 0x38b25f5a, 
0x3acea7c0, 0x3ce4d544, 0x3ef50ad2, 0x40ff6a2e, 
0x43041403, 0x450327eb, 0x46fcc47a, 0x48f10751, 
0x4ae00d1d, 0x4cc9f1ab, 0x4eaecfeb, 0x508ec1fa, 
0x5269e12f, 0x5440461c, 0x5612089a, 0x57df3fd0, 
0x59a80239, 0x5b6c65aa, 0x5d2c7f59, 0x5ee863e5, 
0x60a02757, 0x6253dd2c, 0x64039858, 0x65af6b4b, 
0x675767f5, 0x68fb9fce, 0x6a9c23d6, 0x6c39049b, 
0x6dd2523d, 0x6f681c73, 0x70fa728c, 0x72896373, 
0x7414fdb5, 0x759d4f81, 0x772266ad, 0x78a450b8, 
0x7a231ace, 0x7b9ed1c7, 0x7d17822f, 0x7e8d3846, 
0x80000000, 0x816fe50b,
};

// clz
int32_t zeros(uint32_t x)
{
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x & ~(x >> 16);

	x = (x << 9) - x;
	x = (x << 11) - x;
	x = (x << 14) - x;

	return zeros_lut[x >> 26];
}

// fixed-point log2(x) (Q15.16)
int32_t fixed_log2 (int32_t x)
{
	int32_t f1;
	int32_t f2;
	int32_t dx;
	int32_t a;
	int32_t b;
	int32_t approx;
	int32_t lz;
	int32_t i;
	int32_t idx;
	uint32_t t;

	// x = 2**i * (1 + f), 0 <= f < 1. Find i
	lz = zeros(x);
	i = 15 - lz;

	// normalize f
	t = (uint32_t)x << (lz + 1);

	// index table of log2 values using LOG2_TBL_SIZE msbs of fraction
	idx = t >> (32 - LOG_LOG2_LUT_LEN);

	// difference between argument and smallest sampling point
	dx = t - (idx << (32 - LOG_LOG2_LUT_LEN));

	// fit parabola through closest three sampling points; find coeffs a, b
	f1 = (log2_lut[idx+1] - log2_lut[idx]);
	f2 = (log2_lut[idx+2] - log2_lut[idx]);
	a = f2 - (f1 << 1);
	b = (f1 << 1) - a;

	// find function value for argument by computing ((a*dx+b)*dx)
	approx = (int32_t)((((int64_t)a)*dx) >> (32 - LOG_LOG2_LUT_LEN)) + b;
	approx = (int32_t)((((int64_t)approx)*dx) >> (32 - LOG_LOG2_LUT_LEN + 1));
	approx = log2_lut[idx] + approx;

	// round fractional part of result
	approx = (((uint32_t) approx) + ((1 << 15) / 2) + LOG_MAGIC) >> 15;

	// combine integer and fractional parts of result
	return (i << 16) + approx;
}

// fixed-point 2^x (Q15.16)
int32_t fixed_exp2 (int32_t x)
{
	int32_t i;
	int32_t f;
	int32_t s;
	int32_t r;

	// split a = i + f, such that f in [-0.5, 0.5]

	// 0.5
	i = (x + 0x8000) & ~((int32_t) 0xffff);
	f = x - i;   
	s = ((15 << 16) - i) >> 16;

	// minimax approximation for exp2(f) on [-0.5, 0.5]

	// 5.5171669058037949e-2
	r = 0x00000e20;
	// 2.4261112219321804e-1
	r = (r * f + 0x3e1cc333) >> 17;
	// 6.9326098546062365e-1
	r = (r * f + 0x58bd46a6) >> 16;
	// 9.9992807353939517e-1
	r = (r * f + 0x7ffde4a3);

	return ((uint32_t) r) >> s;
}

// approximate x^2.2
uint32_t gamma_22(uint32_t x)
{
	// (141 / 64) approximates 2.2: we use this technique because it's expensive
	// to divide the fixed-point log2(x) unless the denominator is a power of 2
	//
	// (141 / 64) was chosen specifically because it satisfies this critera
	// while giving a decent approximation of 2.2; here we simply right-shift
	// by 6 instead of dividing by 64
	int64_t t = (141 * (fixed_log2(x << 16) - fixed_log2(255 << 16))) >> 6;

	// everything here is magic: we use it to fix rounding errors,
	// leveraging the same technique as above
	uint64_t ret = (16386 * fixed_exp2(t)) >> 14;

	if (t <= -1154256)
	{
		return 0;
	}

	return ret; 
}

// approximate x^(1/2.2)
uint32_t r_gamma_22(uint32_t x)
{
	// (465 / 1024) approximates (1/2.2), see gamma_22() for more info
	int64_t t = (465 * fixed_log2(x)) >> 10;

	// same magic as in gamma_22() but the shifting is combined with
	// the conversion to integer (we basically shift by 8 + 14)
	uint64_t ret = ((16386 * fixed_exp2(t)) >> 22) - 1;

	return ret; 
}

// integer square root (we don't need fractional precision)
uint32_t isqrt(uint32_t x)
{
	uint32_t x1;
	int32_t s;
	int32_t g0;
	int32_t g1;

	if (x <= 1)
	{
		return x;
	}

	s = 1;
	x1 = x - 1;

	if (x1 > 65535)
	{
		s = s + 8;
		x1 = x1 >> 16;
	}

	if (x1 > 255)
	{
		s = s + 4;
		x1 = x1 >> 8;
	}

	if (x1 > 15)
	{
		s = s + 2;
		x1 = x1 >> 4;
	}

	if (x1 > 3)
	{
		s = s + 1;
	}

	g0 = 1 << s;
	g1 = (g0 + (x >> s)) >> 1;

	while (g1 < g0)
	{
		g0 = g1;
		g1 = (g0 + (x / g0)) >> 1;
	}

	return g0;
}
