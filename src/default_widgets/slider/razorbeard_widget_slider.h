#ifndef H_RAZORBEARD_WIDGET_SLIDER
#define H_RAZORBEARD_WIDGET_SLIDER

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*slider_moved)(struct rzb*, struct rzb_widget*),
		bool vertical,
		uint32_t progress);

void rzb_free_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_slider_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_slider_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_slider
{
	struct rzb_default_widgets_context* context;

	void (*slider_moved)(struct rzb*, struct rzb_widget*);

	bool pushed;
	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

#endif
