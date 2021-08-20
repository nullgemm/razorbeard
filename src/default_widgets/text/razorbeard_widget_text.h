#ifndef H_RAZORBEARD_WIDGET_TEXT
#define H_RAZORBEARD_WIDGET_TEXT

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// text

struct rzb_widget*
	rzb_alloc_widget_text(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* text);

void rzb_free_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_text
{
	struct rzb_default_widgets_context* context;

	char* text;
};

#endif
