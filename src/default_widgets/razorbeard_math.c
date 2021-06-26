#include "razorbeard_math.h"

#include <stdint.h>

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
