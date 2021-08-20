#ifndef H_RAZORBEARD_WIDGET_HANDLES
#define H_RAZORBEARD_WIDGET_HANDLES

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// handles

struct rzb_widget*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal,
		int sections_count);

void rzb_free_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_handles_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_handles_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_handles
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	int sections_count;
	int* section_lengths;
	int* section_dragging;
};

#endif
