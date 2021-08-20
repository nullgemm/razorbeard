#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_numberbox.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// numberbox

struct rzb_widget*
	rzb_alloc_widget_numberbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*number_changed)(struct rzb*, struct rzb_widget*),
		char* value)
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
	widget->callback_render = rzb_render_widget_numberbox;
	widget->callback_events = rzb_event_widget_numberbox;
	widget->callback_free = rzb_free_widget_numberbox;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_numberbox numberbox =
	{
		.context = context,
		.number_changed = number_changed,
		.pushed_up = false,
		.pushed_down = false,
		.value = strdup(value),
	};

	if (numberbox.value == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_numberbox* data =
		malloc(sizeof (struct rzb_widget_numberbox));

	if (data == NULL)
	{
		free(numberbox.value);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = numberbox;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_numberbox* data =
		widget->data_widget;

	free(data->value);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_numberbox* numberbox = widget->data_widget;
	struct rzb_default_widgets_context* context = numberbox->context;
	int buttons_size = (widget->height - (3 * context->sizes_current->size_edge_border)) / 2;

	// outer border
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

	// text input
	rzb_helper_render_rounded_rectangle_side(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		widget->width - buttons_size - (3 * context->sizes_current->size_edge_border),
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border,
		true,
		context->color_background_box);

	// buttons
	rzb_helper_render_rounded_rectangle_side(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width - buttons_size - context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		buttons_size,
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border,
		false,
		context->color_foreground_shine);

	// text/buttons border
	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width - buttons_size - (2 * context->sizes_current->size_edge_border),
		widget->y + context->sizes_current->size_edge_border,
		context->sizes_current->size_edge_border,
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->color_edge);

	// buttons border
	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width - buttons_size - context->sizes_current->size_edge_border,
		widget->y + (widget->height / 2),
		buttons_size,
		context->sizes_current->size_edge_border,
		context->color_edge);

	// arrows
	rzb_helper_render_arrow_vertical(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->y
			+ (widget->height - (2 * context->sizes_current->size_edge_border)) / 4,
		widget->y
			+ 3 * (widget->height - (2 * context->sizes_current->size_edge_border)) / 4,
		widget->x
			+ widget->width - (buttons_size / 2) - context->sizes_current->size_edge_border,
		context->sizes_current->size_arrow_base,
		context->color_text);

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

bool rzb_event_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_numberbox* data = widget->data_widget;
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

void rzb_event_widget_numberbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_increase(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_decrease(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
