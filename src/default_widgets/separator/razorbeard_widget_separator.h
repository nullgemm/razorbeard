#ifndef H_RAZORBEARD_WIDGET_SEPARATOR
#define H_RAZORBEARD_WIDGET_SEPARATOR

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// separator

struct rzb_widget*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context);

void rzb_free_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

#endif
