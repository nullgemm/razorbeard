#ifndef H_RAZORBEARD_WIDGET_DROPMENU
#define H_RAZORBEARD_WIDGET_DROPMENU

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// dropmenu

struct rzb_widget*
	rzb_alloc_widget_dropmenu(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context);

void rzb_free_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

#endif
