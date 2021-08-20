#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_progressbar.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// progressbar

struct rzb_widget*
	rzb_alloc_widget_progressbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool vertical,
		uint32_t progress)
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
	widget->callback_render = rzb_render_widget_progressbar;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_progressbar;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_progressbar progressbar =
	{
		.context = context,
		.vertical = vertical,
		.progress = progress,
	};

	// prepare persistent memory area
	struct rzb_widget_progressbar* data =
		malloc(sizeof (struct rzb_widget_progressbar));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = progressbar;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_progressbar* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int size_bar =
		context->sizes_current->radius_edge_border
		- (2 * context->sizes_current->size_edge_border);

	int size_inner =
		context->sizes_current->radius_edge_border
		- context->sizes_current->size_edge_border;

	int width;
	int height;
	int y;

	if (data->vertical)
	{
		width = (widget->width - (4 * context->sizes_current->size_edge_border));
		height = (2 * size_bar) + (widget->height - (2 * context->sizes_current->radius_edge_border)) * data->progress / 100;
		y = widget->y + widget->height - height - (2 * context->sizes_current->size_edge_border);
	}
	else
	{
		height = (widget->height - (4 * context->sizes_current->size_edge_border));
		width = (2 * size_bar) + (widget->width - (2 * context->sizes_current->radius_edge_border)) * data->progress / 100;
		y = widget->y + (2 * context->sizes_current->size_edge_border);
	}

	rzb_helper_render_hollow_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y,
		widget->width,
		widget->height,
		context->sizes_current->radius_edge_border,
		context->sizes_current->size_edge_border,
		false,
		false,
		context->color_edge);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x
			+ context->sizes_current->size_edge_border,
		widget->y
			+ context->sizes_current->size_edge_border,
		(widget->width - (2 * context->sizes_current->size_edge_border)),
		(widget->height - (2 * context->sizes_current->size_edge_border)),
		size_inner,
		false,
		context->color_foreground);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x
			+ (2 * context->sizes_current->size_edge_border),
		y,
		width,
		height,
		size_bar,
		false,
		context->color_accent);
}
