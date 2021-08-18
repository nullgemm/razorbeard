#ifndef H_RAZORBEARD_HELPERS
#define H_RAZORBEARD_HELPERS

#include "razorbeard.h"

#include <stdbool.h>
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

int rzb_helper_crop_rounded_rectangle(
	int x,
	int y,
	int width,
	int height,
	int radius,
	struct rzb_cropping* cropping,
	int* intersections);

void rzb_helper_circle_pixel_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r);

void rzb_helper_circle_cross_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r);

void rzb_helper_circle_inverted(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int r);

void rzb_helper_ring_pixel_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int ro,
	int ri);

void rzb_helper_ring_cross_centered(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int ro,
	int ri);

void rzb_helper_ring_cross_centered_inverted(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int ro,
	int ri);

void rzb_helper_ring_cross_centered_full(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int ro,
	int ri);

void rzb_helper_render_gradient_radial(
	uint32_t* argb,
	int argb_width,
	uint32_t color,
	int ox,
	int oy,
	int ro,
	int ri);

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

void rzb_helper_render_maximized(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int size_bar,
	int size_border,
	uint32_t color);

void rzb_helper_render_cross(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int center_x,
	int center_y,
	int size,
	int offset,
	int thickness,
	uint32_t color);

void rzb_helper_render_rectangle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	uint32_t color);

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
	uint32_t color);

void rzb_helper_render_rounded_rectangle_side(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int radius,
	bool left,
	uint32_t color);

void rzb_helper_render_bottom_corners(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int radius,
	int size,
	bool tab,
	uint32_t color,
	uint32_t background);

void rzb_helper_render_gradient_solid(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	uint32_t color_top,
	uint32_t color_bottom);

void rzb_helper_render_hollow_rectangle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int radius,
	int thickness,
	bool tab,
	bool full,
	uint32_t color);

void rzb_helper_render_shadow_rectangle(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x,
	int y,
	int width,
	int height,
	int radius,
	int thickness,
	uint32_t color);

void rzb_helper_render_arrow_horizontal(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int x1,
	int x2,
	int y,
	int size,
	uint32_t color);

void rzb_helper_render_arrow_vertical(
	uint32_t* argb,
	int argb_width,
	struct rzb_cropping* cropping,
	int y1,
	int y2,
	int x,
	int size,
	uint32_t color);

// button fsm

enum rzb_fsm_button_state
{
	RZB_FSM_BUTTON_STATE_IDLING = 0,
	RZB_FSM_BUTTON_STATE_HOVERING,
	RZB_FSM_BUTTON_STATE_DRAGGING,
	RZB_FSM_BUTTON_STATE_COUNT,
};

struct rzb_fsm_button
{
	void* data;
	enum rzb_fsm_button_state state;
	enum rzb_fsm_button_state (*update[RZB_FSM_BUTTON_STATE_COUNT])(
		int event,
		void* data);
};

void rzb_fsm_button_init(
	struct rzb_fsm_button* fsm,
	void* data,
	enum rzb_fsm_button_state (**update)(
		int event,
		void* data));

void rzb_fsm_button_update(
	struct rzb_fsm_button* fsm,
	int event);

// text fsm

enum rzb_fsm_text_state
{
	RZB_FSM_TEXT_STATE_IDLING = 0,
	RZB_FSM_TEXT_STATE_EDITING,
	RZB_FSM_TEXT_STATE_SELECTING,
	RZB_FSM_TEXT_STATE_COUNT,
};

struct rzb_fsm_text
{
	void* data;
	enum rzb_fsm_text_state state;
	enum rzb_fsm_text_state (*update[RZB_FSM_TEXT_STATE_COUNT])(
		int event,
		void* data);
};

void rzb_fsm_text_init(
	struct rzb_fsm_text* fsm,
	void* data,
	enum rzb_fsm_text_state (**update)(
		int event,
		void* data));

void rzb_fsm_text_update(
	struct rzb_fsm_text* fsm,
	int event);

#endif
