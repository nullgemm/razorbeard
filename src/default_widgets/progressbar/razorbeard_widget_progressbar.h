#ifndef H_RAZORBEARD_WIDGET_PROGRESSBAR
#define H_RAZORBEARD_WIDGET_PROGRESSBAR

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// progressbar

struct rzb_widget*
	rzb_alloc_widget_progressbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool vertical,
		uint32_t progress);

void rzb_free_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_progressbar
{
	struct rzb_default_widgets_context* context;

	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

#endif
