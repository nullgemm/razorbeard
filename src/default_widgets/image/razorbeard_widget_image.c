#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_image.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// image

struct rzb_widget*
	rzb_alloc_widget_image(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool alpha_premult,
		uint32_t gamma,
		uint32_t* rgba)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_image;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_image;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_image image =
	{
		.context = context,
		.alpha_premult = alpha_premult,
		.gamma = gamma,
		.rgba = rgba,
	};

	struct rzb_widget_image* data =
		malloc(sizeof (struct rzb_widget_image));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = image;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}
