#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_textarea.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// textarea

struct rzb_widget*
	rzb_alloc_widget_textarea(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*text_changed)(struct rzb*, struct rzb_widget*),
		bool resizable,
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
	widget->callback_render = rzb_render_widget_textarea;
	widget->callback_events = rzb_event_widget_textarea;
	widget->callback_free = rzb_free_widget_textarea;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_textarea textarea =
	{
		.context = context,
		.text_changed = text_changed,
		.resizable = resizable,
		.resizing = false,
		.cursor_beg = NULL,
		.cursor_end = NULL,
		text = strdup(text),
	};

	if (textarea.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_textarea* data =
		malloc(sizeof (struct rzb_widget_textarea));

	if (data == NULL)
	{
		free(textarea.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = textarea;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_textarea* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_textarea* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

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
		widget->x + context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		widget->width - (2 * context->sizes_current->size_edge_border),
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->sizes_current->radius_edge_border - context->sizes_current->size_edge_border,
		false,
		context->color_background_box);

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

bool rzb_event_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_textarea* data = widget->data_widget;
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

void rzb_event_widget_textarea_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_resize_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_resize_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
