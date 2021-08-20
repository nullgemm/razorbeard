#ifndef H_RAZORBEARD_WIDGET_IMAGE
#define H_RAZORBEARD_WIDGET_IMAGE

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// image

struct rzb_widget*
	rzb_alloc_widget_image(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool alpha_premult,
		uint32_t gamma,
		uint32_t* rgba);

void rzb_free_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_image
{
	struct rzb_default_widgets_context* context;

	bool alpha_premult;
	uint32_t gamma; // 16.16 fixed-point
	uint32_t* rgba;
};

#endif
