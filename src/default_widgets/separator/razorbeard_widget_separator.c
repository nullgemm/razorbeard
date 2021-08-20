#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_separator.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// separator

struct rzb_widget*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
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
	widget->callback_render = rzb_render_widget_separator;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_separator;
	widget->data_widget = context;

	return widget;
}

void rzb_free_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_default_widgets_context* context = widget->data_widget;

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->padding_separator,
		widget->y + ((widget->height - context->sizes_current->size_separator) / 2),
		widget->width - (2 * context->sizes_current->padding_separator),
		context->sizes_current->size_separator,
		context->color_foreground);
}
