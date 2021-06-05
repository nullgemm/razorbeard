#ifndef H_RAZORBEARD_HELPERS
#define H_RAZORBEARD_HELPERS

#include "razorbeard.h"

#include <stdint.h>

void pixel_set(
	uint32_t* argb,
	int argb_width,
	int x,
	int y,
	uint8_t r,
	uint8_t g,
	uint8_t b,
	uint8_t a);

int rzb_helper_crop_circle(
	int radius,
	int circle_x,
	int circle_y,
	struct rzb_cropping* cropping,
	int* intersections);

void rzb_helper_cricle_pixel_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r);

void rzb_helper_cricle_cross_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r);

void rzb_helper_crop_rectangle(
	int pos,
	int size,
	int pos_crop,
	int size_crop,
	int* pos_out,
	int* size_out);

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
	uint32_t color);

void rzb_helper_render_circle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int circle_x,
	int circle_y,
	int radius,
	int offset,
	uint32_t color);

#endif
