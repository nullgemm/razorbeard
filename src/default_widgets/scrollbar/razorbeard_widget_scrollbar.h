#ifndef H_RAZORBEARD_WIDGET_SCROLLBAR
#define H_RAZORBEARD_WIDGET_SCROLLBAR

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// scrollbar

struct rzb_widget*
	rzb_alloc_widget_scrollbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*scrollbar_moved)(struct rzb*, struct rzb_widget*));

void rzb_free_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_scrollbar_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_scrollbar_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_scrollbar
{
	struct rzb_default_widgets_context* context;

	void (*scrollbar_moved)(struct rzb*, struct rzb_widget*);

	bool pushed;
	int size_scrollbar;
	int position_scrollbar;
};

#endif
