#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_button.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// button

struct rzb_widget*
	rzb_alloc_widget_button(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_activated)(struct rzb*, struct rzb_widget*),
		bool toggle,
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
	widget->callback_render = rzb_render_widget_button;
	widget->callback_events = rzb_event_widget_button;
	widget->callback_free = rzb_free_widget_button;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_button button =
	{
		.context = context,

		.button_pressed = button_pressed,
		.button_released = button_released,
		.button_activated = button_activated,

		.pushed = false,
		.toggle = toggle,
		.active = false,
		.text = strdup(text),
	};

	if (button.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_button* data =
		malloc(sizeof (struct rzb_widget_button));

	if (data == NULL)
	{
		free(button.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = button;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_button* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_button* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	// render bottom infill with gradient
	rzb_helper_render_gradient_solid(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->size_edge_border,
		widget->y + widget->height
			- context->sizes_current->size_edge_border
			- context->sizes_current->size_shine_gradient,
		widget->width
			- (2 * context->sizes_current->size_edge_border),
		context->sizes_current->size_shine_gradient,
		context->color_foreground,
		context->color_foreground_shine_gradient);

	// render bottom-left and bottom-right border corners
	rzb_helper_render_bottom_corners(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y
			+ widget->height
			- context->sizes_current->radius_edge_border,
		widget->width,
		context->sizes_current->radius_edge_border,
		context->sizes_current->radius_edge_border,
		context->sizes_current->size_edge_border,
		false,
		context->color_edge,
		context->color_background);

	// render bottom border
	int border_pos_x =
		widget->x
		+ context->sizes_current->radius_edge_border;

	int border_pos_y =
		widget->y
		+ widget->height
		- context->sizes_current->size_edge_border;

	int border_width =
		widget->width
		- (2 * context->sizes_current->radius_edge_border);

	int border_height =
		context->sizes_current->size_edge_border;

	rzb_helper_crop_rectangle(
		border_pos_x,
		border_width,
		cropping->x,
		cropping->width,
		&border_pos_x,
		&border_width);

	rzb_helper_crop_rectangle(
		border_pos_y,
		border_height,
		cropping->y,
		cropping->height,
		&border_pos_y,
		&border_height);

	for (int x = border_pos_x; x < (border_pos_x + border_width); ++x)
	{
		for (int y = border_pos_y; y < (border_pos_y + border_height); ++y)
		{
			rzb->argb[y * rzb->argb_width + x] = context->color_edge;
		}
	}

	// render top and size borders with up-left and up-right corners
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
		true,
		false,
		context->color_edge);

	// render shine border
	rzb_helper_render_hollow_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		widget->width - (2 * context->sizes_current->size_edge_border),
		2 * (context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border)
			+ context->sizes_current->size_shine_edge,
		context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border,
		context->sizes_current->size_shine_edge,
		true,
		false,
		context->color_foreground_shine);

	// render top infill
	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border
			+ context->sizes_current->size_shine_edge,
		widget->width
			- (2 * context->sizes_current->size_edge_border),
		widget->height
			- (2 * context->sizes_current->size_edge_border)
			- context->sizes_current->size_shine_edge
			- context->sizes_current->size_shine_gradient,
		context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border,
		true,
		context->color_foreground);

	if (rzb->events_grabber == widget)
	{
		rzb_helper_render_hollow_rectangle(
			rzb->argb,
			rzb->argb_width,
			cropping,
			widget->x
				- 2 * context->sizes_current->size_edge_border,
			widget->y
				- 2 * context->sizes_current->size_edge_border,
			widget->width
				+ 4 * context->sizes_current->size_edge_border,
			widget->height
				+ 4 * context->sizes_current->size_edge_border,
			context->sizes_current->radius_edge_border
				+ 2 * context->sizes_current->size_edge_border,
			context->sizes_current->size_edge_border,
			false,
			true,
			context->color_selected);
	}
}

bool rzb_event_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_button* data = widget->data_widget;
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

void rzb_event_widget_button_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_button_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
