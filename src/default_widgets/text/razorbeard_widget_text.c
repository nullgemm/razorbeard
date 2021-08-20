#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_text.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// text

struct rzb_widget*
	rzb_alloc_widget_text(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* text)
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
	widget->callback_render = rzb_render_widget_text;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_text;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_text text_data =
	{
		.context = context,
		.text = strdup(text),
	};

	if (text_data.text == NULL)
	{
		free(widget);
	}

	// prepare persistent memory area
	struct rzb_widget_text* data =
		malloc(sizeof (struct rzb_widget_text));

	if (data == NULL)
	{
		free(text_data.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = text_data;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_text* data = widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}
