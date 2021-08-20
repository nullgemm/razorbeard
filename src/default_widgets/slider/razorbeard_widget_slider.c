#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_slider.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*slider_moved)(struct rzb*, struct rzb_widget*),
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
	widget->callback_render = rzb_render_widget_slider;
	widget->callback_events = rzb_event_widget_slider;
	widget->callback_free = rzb_free_widget_slider;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_slider slider =
	{
		.context = context,
		.slider_moved = slider_moved,
		.pushed = false,
		.vertical = vertical,
		.progress = progress,
	};

	// prepare persistent memory area
	struct rzb_widget_slider* data =
		malloc(sizeof (struct rzb_widget_slider));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = slider;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_slider* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int size_bar =
		context->sizes_current->radius_edge_border
		- (2 * context->sizes_current->size_edge_border);

	int size_inner =
		context->sizes_current->radius_edge_border
		- context->sizes_current->size_edge_border;

	int pos_x_edge;
	int pos_y_edge;
	int size_width_edge;
	int size_height_edge;
	int pos_x_foreground;
	int pos_y_foreground;
	int size_width_foreground;
	int size_height_foreground;
	int pos_x_bar;
	int pos_y_bar;
	int size_width_bar;
	int size_height_bar;
	int pos_x_circle;
	int pos_y_circle;
	int radius_circle;
	int offset_circle;

	if (data->vertical)
	{
		size_width_edge =
			2 * context->sizes_current->radius_edge_border;
		size_height_edge =
			widget->height;

		pos_x_edge =
			widget->x
				+ (widget->width / 2)
				- context->sizes_current->radius_edge_border;
		pos_y_edge =
			widget->y;

		size_width_foreground =
			2 * size_inner;
		size_height_foreground =
			(widget->height - (2 * context->sizes_current->size_edge_border));

		pos_x_foreground =
			widget->x
				+ (widget->width / 2)
				- size_inner;
		pos_y_foreground =
			widget->y
				+ context->sizes_current->size_edge_border;

		size_width_bar =
			(2 * size_bar);
		size_height_bar =
			(2 * size_bar)
				+ (widget->height - (2 * context->sizes_current->radius_edge_border))
					* data->progress / 100;

		pos_x_bar =
			widget->x
				+ (widget->width / 2)
				- size_bar;
		pos_y_bar =
			widget->y
				+ widget->height
				- size_height_bar
				- (2 * context->sizes_current->size_edge_border);

		radius_circle = widget->width / 2;
		offset_circle = widget->width % 2;

		pos_x_circle =
			widget->x
				+ (widget->width / 2);
		pos_y_circle =
			widget->y
				+ widget->height
				- context->sizes_current->radius_edge_border
				- (widget->height - (2 * context->sizes_current->radius_edge_border))
					* data->progress / 100;
	}
	else
	{
		pos_x_edge =
			widget->x;
		pos_y_edge =
			widget->y
				+ (widget->height / 2)
				- context->sizes_current->radius_edge_border;

		size_width_edge =
			widget->width;
		size_height_edge =
			2 * context->sizes_current->radius_edge_border;

		pos_x_foreground =
			widget->x
				+ context->sizes_current->size_edge_border;
		pos_y_foreground =
			widget->y
				+ (widget->height / 2)
				- size_inner;

		size_width_foreground =
			(widget->width - (2 * context->sizes_current->size_edge_border));
		size_height_foreground =
			2 * size_inner;

		pos_x_bar =
			widget->x
				+ (2 * context->sizes_current->size_edge_border);
		pos_y_bar =
			widget->y
				+ (widget->height / 2)
				- size_bar;

		size_width_bar =
			(2 * size_bar)
				+ (widget->width - (2 * context->sizes_current->radius_edge_border))
					* data->progress / 100;
		size_height_bar =
			2 * size_bar;

		pos_x_circle =
			widget->x
				+ context->sizes_current->radius_edge_border
				+ (widget->width - (2 * context->sizes_current->radius_edge_border))
					* data->progress / 100;
		pos_y_circle =
			widget->y
				+ (widget->height / 2);
		radius_circle = widget->height / 2;
		offset_circle = widget->height % 2;
	}

	rzb_helper_render_hollow_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_edge,
		pos_y_edge,
		size_width_edge,
		size_height_edge,
		context->sizes_current->radius_edge_border,
		context->sizes_current->size_edge_border,
		false,
		false,
		context->color_edge);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_foreground,
		pos_y_foreground,
		size_width_foreground,
		size_height_foreground,
		context->sizes_current->size_edge_border,
		false,
		context->color_foreground);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_bar,
		pos_y_bar,
		size_width_bar,
		size_height_bar,
		context->sizes_current->size_edge_border,
		false,
		context->color_accent);

	if (rzb->events_grabber == widget)
	{
		rzb_helper_render_hollow_rectangle(
			rzb->argb,
			rzb->argb_width,
			cropping,
			pos_x_edge
				- 2 * context->sizes_current->size_edge_border,
			pos_y_edge
				- 2 * context->sizes_current->size_edge_border,
			size_width_edge
				+ 4 * context->sizes_current->size_edge_border,
			size_height_edge
				+ 4 * context->sizes_current->size_edge_border,
			context->sizes_current->radius_edge_border
				+ 2 * context->sizes_current->size_edge_border,
			context->sizes_current->size_edge_border,
			false,
			true,
			context->color_selected);
	}

	rzb_helper_render_circle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_circle,
		pos_y_circle,
		radius_circle,
		offset_circle,
		context->color_edge);

	rzb_helper_render_circle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_circle,
		pos_y_circle,
		radius_circle
			- context->sizes_current->size_edge_border,
		offset_circle,
		context->color_foreground_shine);
}

bool rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_slider* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;
#endif

	switch (event_code)
	{
		case RZB_KEY_UP:
		case RZB_KEY_DOWN:
		case RZB_KEY_LEFT:
		case RZB_KEY_RIGHT:
		{
			if ((event_state == RZB_STATE_PRESS)
				&& (rzb->events_grabber == widget))
			{
				rzb_nearest_widget(
					rzb,
					widget,
					event_code);

				return true;
			}

			return false;
		}
		default:
		{
			return false;
		}
	}
}

void rzb_event_widget_slider_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_slider_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
