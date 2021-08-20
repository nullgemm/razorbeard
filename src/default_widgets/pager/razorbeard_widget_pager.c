#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_pager.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// pager

struct rzb_widget*
	rzb_alloc_widget_pager(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal)
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
	widget->children_limit = 1;
	widget->render = true;
	widget->hide = false;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_pager;
	widget->callback_events = rzb_event_widget_pager;
	widget->callback_free = rzb_free_widget_pager;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_pager pager =
	{
		.context = context,
		.horizontal = horizontal,
		.size_page = 0,
		.position_page = 0,
	};

	// prepare persistent memory area
	struct rzb_widget_pager* data =
		malloc(sizeof (struct rzb_widget_pager));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = pager;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

bool rzb_event_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_pager* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;
#endif

	switch (event_code)
	{
		default:
		{
			return false;
		}
	}
}

void rzb_event_widget_pager_scroll_up(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_pager_scroll_down(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
