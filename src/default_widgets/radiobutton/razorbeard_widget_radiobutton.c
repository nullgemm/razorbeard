#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_radiobutton.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// radiobutton

struct rzb_widget*
	rzb_alloc_widget_radiobutton(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*radiobutton_changed)(struct rzb*, struct rzb_widget*),
		bool checked)
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
	widget->callback_render = rzb_render_widget_radiobutton;
	widget->callback_events = rzb_event_widget_radiobutton;
	widget->callback_free = rzb_free_widget_radiobutton;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_radiobutton radiobutton =
	{
		.context = context,
		.radiobutton_changed = radiobutton_changed,
		.checked = checked,
	};

	// prepare persistent memory area
	struct rzb_widget_radiobutton* data =
		malloc(sizeof (struct rzb_widget_radiobutton));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = radiobutton;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_radiobutton* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int min;

	if (widget->height < widget->width)
	{
		min = widget->height;
	}
	else
	{
		min = widget->width;
	}

	int center_offset = min % 2;
	int pos_x = widget->x + widget->width / 2;
	int pos_y = widget->y + widget->height / 2;

	rzb_helper_render_circle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x,
		pos_y,
		min / 2,
		center_offset,
		context->color_edge);

	rzb_helper_render_circle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x,
		pos_y,
		(min / 2) - context->sizes_current->padding_radiobutton,
		center_offset,
		context->color_text);

	if (rzb->events_grabber == widget)
	{
		rzb_helper_ring_cross_centered_full(
			rzb->argb,
			rzb->argb_width,
			context->color_selected,
			pos_x,
			pos_y,
			(min / 2) + (2 * context->sizes_current->size_edge_border),
			(min / 2) + context->sizes_current->size_edge_border);
	}
}

bool rzb_event_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_radiobutton* data = widget->data_widget;
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

void rzb_event_widget_radiobutton_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_radiobutton_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
