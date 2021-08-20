#ifndef H_RAZORBEARD_WIDGET_PAGER
#define H_RAZORBEARD_WIDGET_PAGER

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// pager

struct rzb_widget*
	rzb_alloc_widget_pager(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal);

void rzb_free_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_pager_scroll_up(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_pager_scroll_down(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_pager
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	int size_page;
	int position_page;
};

#endif
