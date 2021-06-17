// TODO only implement rings without inner anti-aliasing
// TODO always render everything from back to front
// TODO render the buttons by first rendering an opaque rectangular gradient
// TODO render the circularly cropped gradients as a post-process step
//      using a dedicated function that provides its own tmp buffer

#include "razorbeard_helpers.h"
#include "razorbeard_math.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// gamma-aware color blending with the existing pixel values
void pixel_set(
	uint32_t* argb,
	int argb_width,
	int x,
	int y,
	uint8_t r,
	uint8_t g,
	uint8_t b,
	uint8_t a)
{
	uint8_t* p = ((uint8_t*) argb) + (y * argb_width + x) * 4;
	uint8_t a_dst = p[3];
	uint8_t a_out = (((int) a_dst) * (0xFF - a) / 0xFF) + a;

	if (a_out > 0)
	{
		p[0] = r_gamma_22(
			(((int) gamma_22(b) * a)
			+ (((int) gamma_22(p[0]) * a_dst * (0xFF - a)) / 0xFF))
			/ a_out);

		p[1] = r_gamma_22(
			(((int) gamma_22(g) * a)
			+ (((int) gamma_22(p[1]) * a_dst * (0xFF - a)) / 0xFF))
			/ a_out);

		p[2] = r_gamma_22(
			(((int) gamma_22(r) * a)
			+ (((int) gamma_22(p[2]) * a_dst * (0xFF - a)) / 0xFF))
			/ a_out);
	}

	p[3] = a_out;
}

int rzb_helper_crop_rounded_rectangle(
	int x,
	int y,
	int width,
	int height,
	int radius,
	struct rzb_cropping* cropping,
	int* intersections)
{
	int intersections_count = 0;
	int cropping_circle_y1 = cropping->y - y - radius;
	int cropping_circle_y2 = cropping->y + cropping->height - height - y + radius;

	int tmp = 0;
	int cropping_circle_x1 = cropping->x - x - radius;
	int cropping_circle_x2 = cropping->x + cropping->width - width - x + radius;

	bool crop_top_right = false;
	bool crop_top_left = false;
	bool crop_bottom_left = false;
	bool crop_bottom_right = false;

	if ((cropping->x + cropping->width < x + width)
		&& (cropping->x + cropping->width > x)
		&& (cropping->y < y + height)
		&& (cropping->y + cropping->height > y))
	{
		tmp = isqrt(
			(radius * radius)
			- (cropping_circle_x2 * cropping_circle_x2));

		if (crop_top_right == false)
		{
			intersections[intersections_count] = cropping_circle_x2 + width - radius;
			++intersections_count;
			intersections[intersections_count] = radius - tmp;
			++intersections_count;
			crop_top_right = true;
		}
	}

	if ((cropping->y < y + height)
		&& (cropping->y > y)
		&& (cropping->x < x + width)
		&& (cropping->x + cropping->width > x))
	{
		tmp = isqrt(
			(radius * radius)
			- (cropping_circle_y1 * cropping_circle_y1));

		if (crop_top_right == false)
		{
			intersections[intersections_count] = tmp + width - radius;
			++intersections_count;
			intersections[intersections_count] = radius + cropping_circle_y1;
			++intersections_count;
			crop_top_right = true;
		}

		if (crop_top_left == false)
		{
			intersections[intersections_count] = width - tmp - radius;
			++intersections_count;
			intersections[intersections_count] = radius + cropping_circle_y1;
			++intersections_count;
			crop_top_left = true;
		}
	}

	if ((cropping->x < x + width)
		&& (cropping->x > x)
		&& (cropping->y < y + height)
		&& (cropping->y + cropping->height > y))
	{
		tmp = isqrt(
			(radius * radius)
			- (cropping_circle_x1 * cropping_circle_x1));

		if (crop_top_left == false)
		{
			intersections[intersections_count] = radius + cropping_circle_x1;
			++intersections_count;
			intersections[intersections_count] = height - tmp - radius;
			++intersections_count;
			crop_top_left = true;
		}

		if (crop_bottom_left == false)
		{
			intersections[intersections_count] = radius + cropping_circle_x1;
			++intersections_count;
			intersections[intersections_count] = tmp + height - radius;
			++intersections_count;
			crop_bottom_left = true;
		}
	}

	if ((cropping->y + cropping->height < y + height)
		&& (cropping->y + cropping->height > y)
		&& (cropping->x < x + width)
		&& (cropping->x + cropping->width > x))
	{
		tmp = isqrt(
			(radius * radius)
			- (cropping_circle_y2 * cropping_circle_y2));

		if (crop_bottom_left == false)
		{
			intersections[intersections_count] = radius - tmp;
			++intersections_count;
			intersections[intersections_count] = cropping_circle_y2 + height - radius;
			++intersections_count;
			crop_bottom_left = true;
		}

		if (crop_bottom_right == false)
		{
			intersections[intersections_count] = radius + tmp;
			++intersections_count;
			intersections[intersections_count] = cropping_circle_y2 + height - radius;
			++intersections_count;
			crop_bottom_right = true;
		}
	}

	if ((cropping->x + cropping->width < x + width)
		&& (cropping->x + cropping->width > x)
		&& (cropping->y < y + height)
		&& (cropping->y + cropping->height > y))
	{
		tmp = isqrt(
			(radius * radius)
			- (cropping_circle_x2 * cropping_circle_x2));

		if (crop_bottom_right == false)
		{
			intersections[intersections_count] = cropping_circle_x2 + width - radius;
			++intersections_count;
			intersections[intersections_count] = tmp + height - radius;
			++intersections_count;
			crop_bottom_right = true;
		}
	}

	return intersections_count;
}

// midpoint circle algorithm with very fast anti-aliasing
// (thanks to integer math and Xiaolin Wu's amazing ideas)
void rzb_helper_circle_pixel_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r)
{
	int x = 0;
	int y = r;
	int p = (5 - r * 4) / 4;

	int r2 = r * r;
	uint8_t a;
	int i;

	int color_a = color >> 24;
	int color_r = (color >> 16) & 0xFF;
	int color_g = (color >> 8) & 0xFF;
	int color_b = color & 0xFF;

	// fill center, top, bottom, left, right pixels
	pixel_set(argb, argb_width, ox,     oy, color_r, color_g, color_b, color_a);
	pixel_set(argb, argb_width, ox, oy + y, color_r, color_g, color_b, color_a * 0x80 / 0xFF);
	pixel_set(argb, argb_width, ox, oy - y, color_r, color_g, color_b, color_a * 0x80 / 0xFF);
	pixel_set(argb, argb_width, ox + y, oy, color_r, color_g, color_b, color_a * 0x80 / 0xFF);
	pixel_set(argb, argb_width, ox - y, oy, color_r, color_g, color_b, color_a * 0x80 / 0xFF);

	// loop offset by one to skip center pixel
	for (x = 1; x < y; ++x)
	{
		if (p < 0)
		{
			p += 2 * x + 1;

			// update alpha value
			a = isqrt((r2 - x * x) << 16) + 0x80;
		}
		else
		{
			y--;
			p += 2 * (x - y) + 1;

			// fix opacity rounding
			a = isqrt((r2 - x * x - 1) << 16) + 0x80;

			// fill vertical and horizontal diameters (from top)
			pixel_set(argb, argb_width, ox, oy + y, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox, oy - y, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + y, oy, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - y, oy, color_r, color_g, color_b, color_a);
		}

		// fill vertical and horizontal diameters (from bottom)
		pixel_set(argb, argb_width, ox, oy + x, color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox, oy - x, color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox + x, oy, color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox - x, oy, color_r, color_g, color_b, color_a);

		// fill diagonal diameters
		if (x != y)
		{
			pixel_set(argb, argb_width, ox + x, oy + x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + x, oy - x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x, oy + x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x, oy - x, color_r, color_g, color_b, color_a);
		}

		// draw anti-aliased border
		pixel_set(argb, argb_width, ox + x, oy + y, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - x, oy + y, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox + x, oy - y, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - x, oy - y, color_r, color_g, color_b, color_a * a / 0xFF);

		if (x != y)
		{
			pixel_set(argb, argb_width, ox + y, oy + x, color_r, color_g, color_b, color_a * a / 0xFF);
			pixel_set(argb, argb_width, ox - y, oy + x, color_r, color_g, color_b, color_a * a / 0xFF);
			pixel_set(argb, argb_width, ox + y, oy - x, color_r, color_g, color_b, color_a * a / 0xFF);
			pixel_set(argb, argb_width, ox - y, oy - x, color_r, color_g, color_b, color_a * a / 0xFF);
		}

		// fill circle
		for (i = x + 1; i < y; ++i)
		{
			pixel_set(argb, argb_width, ox + x, oy + i, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x, oy - i, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + x, oy - i, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x, oy + i, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + i, oy + x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - i, oy - x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + i, oy - x, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - i, oy + x, color_r, color_g, color_b, color_a);
		}
	}
}

// same diy circle routine but moving the origin
// to the bottom-right corner of the target pixel
void rzb_helper_circle_cross_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r)
{
	r += 1;

	int i;
	int a;
	int x = 1;
	int y = r - 1;
	int r2 = r * r;

	int color_a = color >> 24;
	int color_r = (color >> 16) & 0xFF;
	int color_g = (color >> 8) & 0xFF;
	int color_b = color & 0xFF;

	while (x < y)
	{
		a = (0xFF * r) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

		if (a < 0)
		{
			a = -a;
		}

		// set antialiased NNE, NNW, SSE, SSW octants
		pixel_set(argb, argb_width, ox + x - 1, oy + y - 1, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - x    , oy + y - 1, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox + x - 1, oy - y    , color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - x    , oy - y    , color_r, color_g, color_b, color_a * a / 0xFF);

		// set antialiased ENE, ESE, WNW, WSW octants
		pixel_set(argb, argb_width, ox + y - 1, oy + x - 1, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - y    , oy + x - 1, color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox + y - 1, oy - x    , color_r, color_g, color_b, color_a * a / 0xFF);
		pixel_set(argb, argb_width, ox - y    , oy - x    , color_r, color_g, color_b, color_a * a / 0xFF);

		// fill diagonals
		pixel_set(argb, argb_width, ox + x - 1, oy + x - 1, color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox + x - 1, oy - x    , color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox - x    , oy + x - 1, color_r, color_g, color_b, color_a);
		pixel_set(argb, argb_width, ox - x    , oy - x    , color_r, color_g, color_b, color_a);

		// fill the reset of the circle
		for (i = x + 1; i < y; ++i)
		{
			pixel_set(argb, argb_width, ox + x - 1, oy + i - 1, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x    , oy - i    , color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + x - 1, oy - i    , color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - x    , oy + i - 1, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + i - 1, oy + x - 1, color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - i    , oy - x    , color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox + i - 1, oy - x    , color_r, color_g, color_b, color_a);
			pixel_set(argb, argb_width, ox - i    , oy + x - 1, color_r, color_g, color_b, color_a);
		}

		if (((x + 1) * (x + 1) + (y * y)) > r2)
		{
			--y;
			++x;
		}
		else
		{
			++x;
		}
	}

	// set antialiased diagonals pixels
	a = (0xFF * r) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

	if (a < 0)
	{
		a = -a;
	}

	pixel_set(argb, argb_width, ox + x - 1, oy + y - 1, color_r, color_g, color_b, color_a * a / 0xFF);
	pixel_set(argb, argb_width, ox - x    , oy + y - 1, color_r, color_g, color_b, color_a * a / 0xFF);
	pixel_set(argb, argb_width, ox + x - 1, oy - y    , color_r, color_g, color_b, color_a * a / 0xFF);
	pixel_set(argb, argb_width, ox - x    , oy - y    , color_r, color_g, color_b, color_a * a / 0xFF);
}

#if 0
void ras_rounded_rectangle(
	struct ras_buf ras,
	uint16_t x1,
	uint16_t x2,
	uint16_t y1,
	uint16_t y2,
	uint16_t r)
{
	int16_t x = 0;
	int16_t y = r;
	int16_t p = (5 - r * 4) / 4;

	uint32_t r2 = r * r;
	uint8_t a;
	int16_t i;

	for (x = 1; x < y; ++x)
	{
		if (p < 0)
		{
			p += 2 * x + 1;
			a = isqrt((r2 - x * x) << 16) + 0x80;
		}
		else
		{
			y--;
			p += 2 * (x - y) + 1;
			a = isqrt((r2 - x * x - 1) << 16) + 0x80;
		}

		if (x != y)
		{
			pixel_set(ras, x2 + x, y2 + x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x2 + x, y1 - x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - x, y2 + x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - x, y1 - x, 0x00, 0x00, 0x00, 0xFF);
		}

		pixel_set(ras, x2 + x, y2 + y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, x1 - x, y2 + y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, x2 + x, y1 - y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, x1 - x, y1 - y, 0x00, 0x00, 0x00, a);

		if (x != y)
		{
			pixel_set(ras, x2 + y, y2 + x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, x1 - y, y2 + x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, x2 + y, y1 - x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, x1 - y, y1 - x, 0x00, 0x00, 0x00, a);
		}

		for (i = x + 1; i < y; ++i)
		{
			pixel_set(ras, x2 + x, y2 + i, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - x, y1 - i, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x2 + x, y1 - i, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - x, y2 + i, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x2 + i, y2 + x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - i, y1 - x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x2 + i, y1 - x, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, x1 - i, y2 + x, 0x00, 0x00, 0x00, 0xFF);
		}
	}

	int16_t k;

	for (k = y1; k < (y2 + 1); ++k)
	{
		for (i = (x1 - r + 1); i < (x2 + r); ++i)
		{
			pixel_set(ras, i, k, 0x00, 0x00, 0x00, 0xFF);
		}

		// this code performs an accurate extension of rounded
		// corners by adding lines of semi-transparent pixels on the
		// sides of the rectangle (which actually doesn't look good)
		pixel_set(ras, x1 - r, k, 0x00, 0x00, 0x00, 0x80);
		pixel_set(ras, x2 + r, k, 0x00, 0x00, 0x00, 0x80);
	}

	for (i = x1; i < (x2 + 1); ++i)
	{
		for (k = 0; k < r; ++k)
		{
			pixel_set(ras, i, y1 - k, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, i, y2 + k, 0x00, 0x00, 0x00, 0xFF);
		}

		pixel_set(ras, i, y1 - r, 0x00, 0x00, 0x00, 0x80);
		pixel_set(ras, i, y2 + r, 0x00, 0x00, 0x00, 0x80);
	}
}

void ras_ring(
	struct ras_buf ras,
	uint16_t ox,
	uint16_t oy,
	uint16_t ro,
	uint16_t ri)
{
	int16_t x = 0;

	int16_t y = ro;
	int16_t p = (5 - ro * 4) / 4;

	int16_t yi = ri;
	int16_t pi = (5 - ri * 4) / 4;

	uint8_t a;
	uint16_t i;

	uint32_t r2 = ro * ro;
	uint32_t ri2 = ri * ri;

	pixel_set(ras, ox, oy + y, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox, oy - y, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox + y, oy, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox - y, oy, 0x00, 0x00, 0x00, 0x80);

	pixel_set(ras, ox, oy + yi, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox, oy - yi, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox + yi, oy, 0x00, 0x00, 0x00, 0x80);
	pixel_set(ras, ox - yi, oy, 0x00, 0x00, 0x00, 0x80);

	for (x = 1; x < y; ++x)
	{
		if (p < 0)
		{
			p += 2 * x + 1;
			a = isqrt((r2 - x * x) << 16) + 0x80;
		}
		else
		{
			y--;
			p += 2 * (x - y) + 1;
			a = isqrt((r2 - x * x - 1) << 16) + 0x80;
		}

		pixel_set(ras, ox + x, oy + y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x, oy + y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox + x, oy - y, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x, oy - y, 0x00, 0x00, 0x00, a);

		if (x != y)
		{
			pixel_set(ras, ox + y, oy + x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - y, oy + x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox + y, oy - x, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - y, oy - x, 0x00, 0x00, 0x00, a);
		}

		if (x < yi)
		{
			if (pi < 0)
			{
				pi += 2 * x + 1;
				a = 0xFF - (isqrt((ri2 - x * x) << 16) + 0x80);
			}
			else
			{
				yi--;
				pi += 2 * (x - yi) + 1;
				a = 0xFF - (isqrt((ri2 - x * x - 1) << 16) + 0x80);
			}

			pixel_set(ras, ox + x, oy + yi, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - x, oy + yi, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox + x, oy - yi, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - x, oy - yi, 0x00, 0x00, 0x00, a);

			if (x != yi)
			{
				pixel_set(ras, ox + yi, oy + x, 0x00, 0x00, 0x00, a);
				pixel_set(ras, ox - yi, oy + x, 0x00, 0x00, 0x00, a);
				pixel_set(ras, ox + yi, oy - x, 0x00, 0x00, 0x00, a);
				pixel_set(ras, ox - yi, oy - x, 0x00, 0x00, 0x00, a);
			}

			for (i = yi + 1; i < y; ++i)
			{
				pixel_set(ras, ox + x, oy + i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x, oy + i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + x, oy - i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x, oy - i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i, oy + x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i, oy + x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i, oy - x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i, oy - x, 0x00, 0x00, 0x00, 0xFF);
			}
		}
		else
		{
			for (i = x; i < y; ++i)
			{
				pixel_set(ras, ox + x, oy + i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x, oy + i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + x, oy - i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x, oy - i, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i, oy + x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i, oy + x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i, oy - x, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i, oy - x, 0x00, 0x00, 0x00, 0xFF);
			}
		}
	}

	for (i = ri + 1; i < ro; ++i)
	{
		pixel_set(ras, ox + i, oy, 0x00, 0x00, 0x00, 0xFF);
		pixel_set(ras, ox - i, oy, 0x00, 0x00, 0x00, 0xFF);
		pixel_set(ras, ox, oy + i, 0x00, 0x00, 0x00, 0xFF);
		pixel_set(ras, ox, oy - i, 0x00, 0x00, 0x00, 0xFF);
	}
}

void ras_ring2(
	struct ras_buf ras,
	uint32_t ox,
	uint32_t oy,
	uint32_t ro,
	uint32_t ri)
{
	ro += 1;
	ri += 1;

	int32_t i;
	int32_t a;

	int32_t x = 1;
	int32_t y = ro - 1;

	int32_t xi = 1;
	int32_t yi = ri - 1;

	int32_t ro2 = ro * ro;
	int32_t ri2 = ri * ri;

	while (x < y)
	{
		a = (0xFF * ro) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

		if (a < 0)
		{
			a = -a;
		}

		// set antialiased NNE, NNW, SSE, SSW octants
		pixel_set(ras, ox + x - 1, oy + y - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x    , oy + y - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox + x - 1, oy - y    , 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x    , oy - y    , 0x00, 0x00, 0x00, a);

		// set antialiased ENE, ESE, WNW, WSW octants
		pixel_set(ras, ox + y - 1, oy + x - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - y    , oy + x - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox + y - 1, oy - x    , 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - y    , oy - x    , 0x00, 0x00, 0x00, a);

		if (xi < yi)
		{
			a = 0xFF - (0xFF * ri) + ((0xFF * isqrt(((yi * yi) + (xi * xi)) << 16)) >> 8);

			if (a < 0)
			{
				a = -a;
			}

			// set antialiased NNE, NNW, SSE, SSW octants
			pixel_set(ras, ox + xi - 1, oy + yi - 1, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - xi    , oy + yi - 1, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox + xi - 1, oy - yi    , 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - xi    , oy - yi    , 0x00, 0x00, 0x00, a);

			// set antialiased ENE, ESE, WNW, WSW octants
			pixel_set(ras, ox + yi - 1, oy + xi - 1, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - yi    , oy + xi - 1, 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox + yi - 1, oy - xi    , 0x00, 0x00, 0x00, a);
			pixel_set(ras, ox - yi    , oy - xi    , 0x00, 0x00, 0x00, a);

			// fill the rest of the circle
			for (i = yi + 1; i < y; ++i)
			{
				pixel_set(ras, ox + xi - 1, oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - xi    , oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + xi - 1, oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - xi    , oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy + xi - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy - xi    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy - xi    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy + xi - 1, 0x00, 0x00, 0x00, 0xFF);
			}

			if (((xi + 1) * (xi + 1) + (yi * yi)) > ri2)
			{
				--yi;
				++xi;
			}
			else
			{
				++xi;
			}
		}
		else
		{
			// fill diagonals
			if (x > yi)
			{
				pixel_set(ras, ox + x - 1, oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + x - 1, oy - x    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy - x    , 0x00, 0x00, 0x00, 0xFF);
			}

			// fill the rest of the circle
			for (i = x + 1; i < y; ++i)
			{
				pixel_set(ras, ox + x - 1, oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + x - 1, oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy - x    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy - x    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
			}
		}

		if (((x + 1) * (x + 1) + (y * y)) > ro2)
		{
			--y;
			++x;
		}
		else
		{
			++x;
		}
	}

	// set antialiased diagonals pixels
	a = (0xFF * ro) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

	if (a < 0)
	{
		a = -a;
	}

	pixel_set(ras, ox + x - 1, oy + y - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - x    , oy + y - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox + x - 1, oy - y    , 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - x    , oy - y    , 0x00, 0x00, 0x00, a);

	// set antialiased diagonals pixels
	a = 0xFF - (0xFF * ri) + ((0xFF * isqrt(((yi * yi) + (xi * xi)) << 16)) >> 8);

	if (a < 0)
	{
		a = -a;
	}

	pixel_set(ras, ox + xi - 1, oy + yi - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - xi    , oy + yi - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox + xi - 1, oy - yi    , 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - xi    , oy - yi    , 0x00, 0x00, 0x00, a);
}

void ras_ring3(
	struct ras_buf ras,
	uint32_t ox,
	uint32_t oy,
	uint32_t ro,
	uint32_t ri)
{
	ro += 1;
	ri += 1;

	int32_t i;
	int32_t a;

	int32_t x = 1;
	int32_t y = ro - 1;

	int32_t xi = 1;
	int32_t yi = ri - 1;

	int32_t ro2 = ro * ro;
	int32_t ri2 = ri * ri;

	while (x < y)
	{
		a = (0xFF * ro) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

		if (a < 0)
		{
			a = -a;
		}

		// set antialiased NNE, NNW, SSE, SSW octants
		pixel_set(ras, ox + x - 1, oy + y - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x    , oy + y - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox + x - 1, oy - y    , 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - x    , oy - y    , 0x00, 0x00, 0x00, a);

		// set antialiased ENE, ESE, WNW, WSW octants
		pixel_set(ras, ox + y - 1, oy + x - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - y    , oy + x - 1, 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox + y - 1, oy - x    , 0x00, 0x00, 0x00, a);
		pixel_set(ras, ox - y    , oy - x    , 0x00, 0x00, 0x00, a);

		if (xi < yi)
		{
			// fill the ring
			for (i = yi; i < y; ++i)
			{
				pixel_set(ras, ox + xi - 1, oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - xi    , oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + xi - 1, oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - xi    , oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy + xi - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy - xi    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy - xi    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy + xi - 1, 0x00, 0x00, 0x00, 0xFF);
			}

			if (((xi + 1) * (xi + 1) + (yi * yi)) > ri2)
			{
				--yi;
				++xi;
			}
			else
			{
				++xi;
			}
		}
		else
		{
			// fill diagonals
			pixel_set(ras, ox + x - 1, oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, ox + x - 1, oy - x    , 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, ox - x    , oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
			pixel_set(ras, ox - x    , oy - x    , 0x00, 0x00, 0x00, 0xFF);

			// fill the rest of the circle
			for (i = x + 1; i < y; ++i)
			{
				pixel_set(ras, ox + x - 1, oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + x - 1, oy - i    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - x    , oy + i - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy - x    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox + i - 1, oy - x    , 0x00, 0x00, 0x00, 0xFF);
				pixel_set(ras, ox - i    , oy + x - 1, 0x00, 0x00, 0x00, 0xFF);
			}
		}

		if (((x + 1) * (x + 1) + (y * y)) > ro2)
		{
			--y;
			++x;
		}
		else
		{
			++x;
		}
	}

	// set antialiased diagonals pixels
	a = (0xFF * ro) - ((0xFF * isqrt(((y * y) + (x * x)) << 16)) >> 8);

	if (a < 0)
	{
		a = -a;
	}

	pixel_set(ras, ox + x - 1, oy + y - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - x    , oy + y - 1, 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox + x - 1, oy - y    , 0x00, 0x00, 0x00, a);
	pixel_set(ras, ox - x    , oy - y    , 0x00, 0x00, 0x00, a);
}
#endif

inline void rzb_helper_crop_rectangle(
	int pos,
	int size,
	int pos_crop,
	int size_crop,
	int* pos_out,
	int* size_out)
{
	if (pos < pos_crop)
	{
		if (size > (pos_crop - pos))
		{
			size -= (pos_crop - pos);
		}
		else
		{
			size = 0;
		}

		pos = pos_crop;
	}
	else if (pos > (pos_crop + size_crop))
	{
		size = 0;
		pos = (pos_crop + size_crop);
	}

	if ((pos + size)
		> (pos_crop + size_crop))
	{
		size =
			size_crop - (pos - pos_crop);
	}

	*pos_out = pos;
	*size_out = size;
}

void rzb_helper_render_cropped_circle(
	uint32_t* argb,
	int argb_width,
	int buffer_x1,
	int buffer_x2,
	int buffer_y1,
	int buffer_y2,
	int circle_x,
	int circle_y,
	int radius,
	int offset,
	uint32_t color)
{
	uint32_t* buffer =
		malloc((2 * radius + offset)
			* (2 * radius + offset)
			* (sizeof (uint32_t)));

	for (int y = buffer_y1; y < buffer_y2 + offset; ++y)
	{
		memcpy(
			buffer
				+ ((y + radius - circle_y) * (2 * radius + offset))
				+ buffer_x1 + radius - circle_x,
			argb
				+ y * argb_width
				+ buffer_x1,
			(buffer_x2 - buffer_x1 + offset)
				* (sizeof (uint32_t)));
	}

	if (offset > 0)
	{
		rzb_helper_circle_pixel_centered(
			buffer,
			2 * radius + offset,
			color,
			radius,
			radius,
			radius);
	}
	else
	{
		rzb_helper_circle_cross_centered(
			buffer,
			2 * radius,
			color,
			radius,
			radius,
			radius);
	}

	for (int y = buffer_y1; y < buffer_y2 + offset; ++y)
	{
		memcpy(
			argb
				+ y * argb_width
				+ buffer_x1,
			buffer
				+ ((y + radius - circle_y) * (2 * radius + offset))
				+ buffer_x1 + radius - circle_x,
			(buffer_x2 - buffer_x1 + offset)
				* (sizeof (uint32_t)));
	}

	free(buffer);
}

void rzb_helper_render_circle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int circle_x,
	int circle_y,
	int radius,
	int offset,
	uint32_t color)
{
	int intersections[24] = {0};

	int intersections_count =
		rzb_helper_crop_rounded_rectangle(
			circle_x - radius,
			circle_y - radius,
			radius * 2,
			radius * 2,
			radius,
			cropping,
			intersections);

	if (intersections_count == 0)
	{
		if ((circle_x > cropping->x)
		&& (circle_x < (cropping->x + cropping->width))
		&& (circle_y > cropping->y)
		&& (circle_y < (cropping->y + cropping->height)))
		{
			if (offset > 0)
			{
				rzb_helper_circle_pixel_centered(
					argb,
					argb_width,
					color,
					circle_x,
					circle_y,
					radius);
			}
			else
			{
				rzb_helper_circle_cross_centered(
					argb,
					argb_width,
					color,
					circle_x,
					circle_y,
					radius);
			}
		}
	}
	else
	{
		int buffer_x1 = circle_x - radius;
		int buffer_y1 = circle_y - radius;
		int buffer_x2 = circle_x + radius;
		int buffer_y2 = circle_y + radius;

		for (int i = 0; i < intersections_count; i += 2)
		{
			if ((circle_x + intersections[i] - radius)
				== cropping->x)
			{
				buffer_x1 = cropping->x;
			}
			else if ((circle_x + intersections[i] - radius)
				== (cropping->x + cropping->width))
			{
				buffer_x2 = cropping->x + cropping->width - offset;
			}

			if ((circle_y + intersections[i + 1] - radius)
				== cropping->y)
			{
				buffer_y1 = cropping->y;
			}
			else if ((circle_y + intersections[i + 1] - radius)
				== (cropping->y + cropping->height))
			{
				buffer_y2 = cropping->y + cropping->height - offset;
			}
		}

		rzb_helper_render_cropped_circle(
			argb,
			argb_width,
			buffer_x1,
			buffer_x2,
			buffer_y1,
			buffer_y2,
			circle_x,
			circle_y,
			radius,
			offset,
			color);
	}
}

void rzb_helper_render_cropped_rounded_rectangle(
	uint32_t* argb,
	int argb_width,
	int buffer_x1,
	int buffer_x2,
	int buffer_y1,
	int buffer_y2,
	int pos_x,
	int pos_y,
	int width,
	int height,
	int radius,
	bool tab,
	uint32_t color)
{
	uint32_t* buffer =
		malloc((2 * radius)
			* (2 * radius)
			* (sizeof (uint32_t)));

	int size_x1 = pos_x + radius - buffer_x1;

	if (size_x1 > (buffer_x2 - pos_x))
	{
		size_x1 = buffer_x2 - pos_x;
	}

	if (size_x1 < 0)
	{
		size_x1 = 0;
	}

	int size_x2 = buffer_x2 + radius - pos_x - width;
	int x1 = 0;

	if (size_x2 > pos_x + width - buffer_x1)
	{
		size_x2 = pos_x + width - buffer_x1;
		x1 = radius - size_x2;
	}

	if (size_x2 < 0)
	{
		size_x2 = 0;
	}

	for (int y = buffer_y1; y < pos_y + radius; ++y)
	{
		memcpy(
			buffer
				+ ((y - pos_y) * (2 * radius))
				+ buffer_x1 - pos_x,
			argb
				+ y * argb_width
				+ buffer_x1,
			size_x1
				* (sizeof (uint32_t)));

		memcpy(
			buffer
				+ ((y - pos_y) * (2 * radius))
				+ x1 + radius,
			argb
				+ y * argb_width
				+ pos_x + width - radius + x1,
			size_x2
				* (sizeof (uint32_t)));
	}

	for (int y = pos_y + height - radius; y < buffer_y2; ++y)
	{
		memcpy(
			buffer
				+ ((y + (2 * radius) - pos_y - height) * (2 * radius))
				+ buffer_x1 - pos_x,
			argb
				+ y * argb_width
				+ buffer_x1,
			size_x1
				* (sizeof (uint32_t)));

		memcpy(
			buffer
				+ ((y + (2 * radius) - pos_y - height) * (2 * radius))
				+ x1 + radius,
			argb
				+ y * argb_width
				+ pos_x + width - radius + x1,
			size_x2
				* (sizeof (uint32_t)));
	}

	rzb_helper_circle_cross_centered(
		buffer,
		2 * radius,
		color,
		radius,
		radius,
		radius);

	for (int y = buffer_y1; y < buffer_y2; ++y)
	{
		for (int x = buffer_x1; x < buffer_x2; ++x)
		{
			pixel_set(
				argb,
				argb_width,
				x,
				y,
				(color >> 16) & 0xff,
				(color >> 8) & 0xff,
				color & 0xff,
				color >> 24);
		}
	}

	int y1;

	if ((pos_y + radius) > buffer_y2)
	{
		y1 = buffer_y2;
	}
	else
	{
		y1 = pos_y + radius;
	}

	for (int y = buffer_y1; y < y1; ++y)
	{
		memcpy(
			argb
				+ y * argb_width
				+ buffer_x1,
			buffer
				+ ((y - pos_y) * (2 * radius))
				+ buffer_x1 - pos_x,
			size_x1
				* (sizeof (uint32_t)));

		memcpy(
			argb
				+ y * argb_width
				+ pos_x + width - radius + x1,
			buffer
				+ ((y - pos_y) * (2 * radius))
				+ x1 + radius,
			size_x2
				* (sizeof (uint32_t)));
	}

	if (tab == true)
	{
		free(buffer);
		return;
	}

	int y2;

	if ((pos_y + height - radius) < buffer_y1)
	{
		y2 = buffer_y1;
	}
	else
	{
		y2 = pos_y + height - radius;
	}

	for (int y = y2; y < buffer_y2; ++y)
	{
		memcpy(
			argb
				+ y * argb_width
				+ buffer_x1,
			buffer
				+ ((y + (2 * radius) - pos_y - height) * (2 * radius))
				+ buffer_x1 - pos_x,
			size_x1
				* (sizeof (uint32_t)));

		memcpy(
			argb
				+ y * argb_width
				+ pos_x + width - radius + x1,
			buffer
				+ ((y + (2 * radius) - pos_y - height) * (2 * radius))
				+ x1 + radius,
			size_x2
				* (sizeof (uint32_t)));
	}
}

void rzb_helper_render_rounded_rectangle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int radius,
	bool tab,
	uint32_t color)
{
	int intersections[24] = {0};

	int intersections_count =
		rzb_helper_crop_rounded_rectangle(
			x,
			y,
			width,
			height,
			radius,
			cropping,
			intersections);

	int buffer_x1 = x;
	int buffer_y1 = y;
	int buffer_x2 = x + width;
	int buffer_y2 = y + height;

	if (intersections_count == 0)
	{
		if ((x >= cropping->x)
		&& ((x + width) <= (cropping->x + cropping->width))
		&& (y >= cropping->y)
		&& ((y + height) <= (cropping->y + cropping->height)))
		{
			rzb_helper_render_cropped_rounded_rectangle(
				argb,
				argb_width,
				buffer_x1,
				buffer_x2,
				buffer_y1,
				buffer_y2,
				x,
				y,
				width,
				height,
				radius,
				tab,
				color);
		}
	}
	else
	{
		for (int i = 0; i < intersections_count; i += 2)
		{
			if ((x + intersections[i])
				== cropping->x)
			{
				buffer_x1 = cropping->x;
			}
			else if ((x + intersections[i])
				== (cropping->x + cropping->width))
			{
				buffer_x2 = cropping->x + cropping->width;
			}

			if ((y + intersections[i + 1])
				== cropping->y)
			{
				buffer_y1 = cropping->y;
			}
			else if ((y + intersections[i + 1])
				== (cropping->y + cropping->height))
			{
				buffer_y2 = cropping->y + cropping->height;
			}
		}

		rzb_helper_render_cropped_rounded_rectangle(
			argb,
			argb_width,
			buffer_x1,
			buffer_x2,
			buffer_y1,
			buffer_y2,
			x,
			y,
			width,
			height,
			radius,
			tab,
			color);
	}
}

void rzb_helper_render_arrow_horizontal(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x1,
	int x2,
	int y,
	int size,
	uint32_t color)
{
	uint8_t color_r = (color >> 16) & 0xFF;
	uint8_t color_g = (color >> 8) & 0xFF;
	uint8_t color_b = color & 0xFF;

	// buffer info
	int buffer_size = size + 2;
	int center = size / 3;

	int half_size = size / 2;
	int odd = size % 2;

	uint32_t* buffer = malloc(buffer_size * buffer_size * (sizeof (uint32_t)));

	// cropping values
	int crop_y = 1;
	int crop_height = size;

	int crop_x1 = 0;
	int crop_width1 = half_size + (1 + odd);

	int crop_x2 = half_size + odd;
	int crop_width2 = half_size + (1 + odd);

	// cropping the first arrow horizontally
	if (cropping->x > (x1 - center + half_size + 1))
	{
		crop_width1 = 0;
	}
	else if (cropping->x > (x1 - center))
	{
		crop_x1 = cropping->x - x1 + center;
		crop_width1 = half_size + (1 + odd) - crop_x1;
	}

	if ((cropping->x + cropping->width) < (x1 - center))
	{
		crop_width1 = 0;
	}
	else if ((cropping->x + cropping->width) < (x1 - center + half_size + 1 + odd))
	{
		crop_width1 = cropping->x + cropping->width - (x1 + crop_x1 - center);
	}

	// cropping the second arrow horizontally
	if (cropping->x > (x2 + center + odd - 1))
	{
		crop_width2 = 0;
	}
	else if (cropping->x > (x2 + center - odd - 1 - half_size))
	{
		crop_x2 = cropping->x - x2 - center + 2 * (half_size + odd) + 1;
		crop_width2 = 2 * (half_size + odd) + 1 - crop_x2;
	}

	if ((cropping->x + cropping->width) < (x2 + center - half_size - odd - 1))
	{
		crop_width2 = 0;
	}
	else if ((cropping->x + cropping->width) < (x2 + center))
	{
		crop_width2 = cropping->x + cropping->width - (crop_x2 + x2 + center - 2 * (half_size + odd) - 1);
	}

	// cropping vertically
	if (cropping->y > (y + half_size + odd))
	{
		crop_height = 0;
	}
	else if (cropping->y > (y - half_size - odd))
	{
		crop_y = cropping->y - (y - half_size - odd);
		crop_height = size - crop_y + 1;
	}

	if ((cropping->y + cropping->height) < (y - half_size - odd))
	{
		crop_height = 0;
	}
	else if ((cropping->y + cropping->height) < (y + half_size + 1))
	{
		crop_height = cropping->y + cropping->height - (crop_y + y - half_size - odd);
	}

	// copy relevant background sections in temporary buffer
	for (int pos_y = crop_y; pos_y < (crop_y + crop_height); ++pos_y)
	{
		memcpy(
			buffer + (pos_y * buffer_size) + crop_x1,
			argb + (y + pos_y - half_size - odd) * argb_width + (x1 + crop_x1 - center),
			crop_width1 * (sizeof (uint32_t)));

		memcpy(
			buffer + (pos_y * buffer_size) + crop_x2 + (1 - odd),
			argb + (y + pos_y - half_size - odd) * argb_width + (x2 + crop_x2 + center - 2 * (half_size + odd) - 1),
			crop_width2 * (sizeof (uint32_t)));
	}

	// render arrows
	for (int pos_y = 0; pos_y < (half_size + (1 - odd)); ++pos_y)
	{
		// render all opaque pixels except the middle horizontal line for odd bases
		for (int pos_x = 0; pos_x < ((2 * pos_y) + odd); ++pos_x)
		{
			pixel_set(
				buffer,
				buffer_size,
				half_size - pos_y + pos_x + 1,
				pos_y + odd,
				color_r,
				color_g,
				color_b,
				0xFF);

			pixel_set(
				buffer,
				buffer_size,
				half_size - pos_y + pos_x + 1,
				size - pos_y + (1 - odd),
				color_r,
				color_g,
				color_b,
				0xFF);
		}

		// render all the anti-aliasing except that of the tips of odd bases
		pixel_set(
			buffer,
			buffer_size,
			half_size - pos_y,
			pos_y + odd,
			color_r,
			color_g,
			color_b,
			0x60);

		pixel_set(
			buffer,
			buffer_size,
			half_size + pos_y + (1 + odd),
			pos_y + odd,
			color_r,
			color_g,
			color_b,
			0x60);

		pixel_set(
			buffer,
			buffer_size,
			half_size - pos_y,
			size - pos_y + (1 - odd),
			color_r,
			color_g,
			color_b,
			0x60);

		pixel_set(
			buffer,
			buffer_size,
			half_size + pos_y + (1 + odd),
			size - pos_y + (1 - odd),
			color_r,
			color_g,
			color_b,
			0x60);
	}

	if (odd)
	{
		// render the middle horizontal line of odd bases
		for (int pos = 0; pos < size; ++pos)
		{
			pixel_set(
				buffer,
				buffer_size,
				pos + 1,
				half_size + 1,
				color_r,
				color_g,
				color_b,
				0xFF);
		}

		// render the tips of odd bases
		pixel_set(
			buffer,
			buffer_size,
			buffer_size - 1,
			half_size + 1,
			color_r,
			color_g,
			color_b,
			0x40);

		pixel_set(
			buffer,
			buffer_size,
			half_size + 1,
			0,
			color_r,
			color_g,
			color_b,
			0x40);

		pixel_set(
			buffer,
			buffer_size,
			0,
			half_size + 1,
			color_r,
			color_g,
			color_b,
			0x40);

		pixel_set(
			buffer,
			buffer_size,
			half_size + 1,
			buffer_size - 1,
			color_r,
			color_g,
			color_b,
			0x40);
	}

	// copy temporary buffer back to the main buffer respecting the cropping values
	for (int pos_y = crop_y; pos_y < (crop_y + crop_height); ++pos_y)
	{
		memcpy(
			argb + (y + pos_y - half_size - odd) * argb_width + (x1 + crop_x1 - center),
			buffer + (pos_y * buffer_size) + crop_x1,
			crop_width1 * (sizeof (uint32_t)));

		memcpy(
			argb + (y + pos_y - half_size - odd) * argb_width + (x2 + crop_x2 + center - 2 * (half_size + odd) - 1),
			buffer + (pos_y * buffer_size) + crop_x2 + (1 - odd),
			crop_width2 * (sizeof (uint32_t)));
	}

	free(buffer);
}
