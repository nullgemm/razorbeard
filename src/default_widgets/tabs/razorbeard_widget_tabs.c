#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_tabs.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// tabs

struct rzb_widget*
	rzb_alloc_widget_tabs(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*tab_switched)(struct rzb*, struct rzb_widget*),
		char** tabs_names,
		int tabs_count,
		int tab_active)
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
	widget->callback_render = rzb_render_widget_tabs;
	widget->callback_events = rzb_event_widget_tabs;
	widget->callback_free = rzb_free_widget_tabs;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_tabs tabs =
	{
		.context = context,
		.tab_switched = tab_switched,
		.tabs_names = tabs_names,
		.tabs_count = tabs_count,
		.tab_active = tab_active,
		.tab_first_visible = 0,
	};

	// prepare persistent memory area
	struct rzb_widget_tabs* data =
		malloc(sizeof (struct rzb_widget_tabs));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = tabs;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_tabs* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	if ((widget->height <= context->sizes_current->tab_default_height)
		|| (widget->width < (2 * context->sizes_current->tab_default_height)))
	{
		return;
	}

	int pos_x = widget->x;
	int width = widget->width;
	int pos_y = widget->y + context->sizes_current->tab_default_height;
	int height = widget->height - context->sizes_current->tab_default_height;
	int pos_y_tabs = widget->y;
	int height_tabs = context->sizes_current->tab_default_height;

	rzb_helper_crop_rectangle(
		pos_x,
		width,
		cropping->x,
		cropping->width,
		&pos_x,
		&width);

	rzb_helper_crop_rectangle(
		pos_y,
		height,
		cropping->y,
		cropping->height,
		&pos_y,
		&height);

	rzb_helper_crop_rectangle(
		pos_y_tabs,
		height_tabs,
		cropping->y,
		cropping->height,
		&pos_y_tabs,
		&height_tabs);

	for (int y = pos_y_tabs; y < (pos_y_tabs + height_tabs); ++y)
	{
		for (int x = pos_x; x < pos_x + width; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_foreground_shine;
		}
	}

	for (int y = pos_y; y < (pos_y + height); ++y)
	{
		for (int x = pos_x; x < pos_x + width; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_background;
		}
	}

	// get the maximum number of tabs we can display
	int max_tabs;
	int pos_tabs;

	if ((widget->width / context->sizes_current->tab_default_width) < data->tabs_count)
	{
		max_tabs =
			(widget->width - (2 * context->sizes_current->tab_default_height))
			/ context->sizes_current->tab_default_width;

		pos_tabs =
			widget->x
			+ context->sizes_current->tab_default_height;

		// render the buttons
		if (max_tabs == 0)
		{
			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x
					+ context->sizes_current->padding_tab_button,
				widget->y
					+ context->sizes_current->padding_tab_button,
				(widget->width - 3 * context->sizes_current->padding_tab_button) / 2,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x
					+ (widget->width + context->sizes_current->padding_tab_button) / 2,
				widget->y
					+ context->sizes_current->padding_tab_button,
				(widget->width - 3 * context->sizes_current->padding_tab_button) / 2,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			rzb_helper_render_arrow_horizontal(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x + 1 * widget->width / 4,
				widget->x + 3 * widget->width / 4,
				widget->y
					+ (context->sizes_current->tab_default_height / 2),
				context->sizes_current->size_arrow_base,
				context->color_text);
		}
		else
		{
			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x
					+ context->sizes_current->padding_tab_button,
				widget->y
					+ context->sizes_current->padding_tab_button,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x
					+ widget->width
					- context->sizes_current->tab_default_height
					+ context->sizes_current->padding_tab_button,
				widget->y
					+ context->sizes_current->padding_tab_button,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->tab_default_height
					- 2 * context->sizes_current->padding_tab_button,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			rzb_helper_render_arrow_horizontal(
				rzb->argb,
				rzb->argb_width,
				cropping,
				widget->x
					+ (context->sizes_current->tab_default_height / 2),
				widget->x
					+ widget->width
					- (context->sizes_current->tab_default_height / 2),
				widget->y
					+ (context->sizes_current->tab_default_height / 2),
				context->sizes_current->size_arrow_base,
				context->color_text);
		}
	}
	else
	{
		max_tabs = data->tabs_count;
		pos_tabs = widget->x;
	}

	if (max_tabs > 0)
	{
		// render the active tab background
		if ((data->tab_active - data->tab_first_visible) < max_tabs)
		{
			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				pos_tabs + ((data->tab_active - data->tab_first_visible) * context->sizes_current->tab_default_width),
				widget->y,
				context->sizes_current->tab_default_width,
				context->sizes_current->tab_default_height,
				context->sizes_current->radius_edge_border,
				true,
				context->color_background);
		}

		// render the tabs text and separators
		for (int i = 0; i < max_tabs; ++i)
		{
			// render separators
			if ((i != (data->tab_active - data->tab_first_visible))
				&& ((i + 1) != (data->tab_active - data->tab_first_visible))
				&& ((i + 1) < max_tabs))
			{
				pos_x = pos_tabs + ((i + 1) * context->sizes_current->tab_default_width) - (context->sizes_current->size_tab_separator / 2);
				width = context->sizes_current->size_tab_separator;
				pos_y = widget->y + context->sizes_current->padding_tab_separator;
				height = context->sizes_current->tab_default_height - (2 * context->sizes_current->padding_tab_separator);

				rzb_helper_crop_rectangle(
					pos_x,
					width,
					cropping->x,
					cropping->width,
					&pos_x,
					&width);

				rzb_helper_crop_rectangle(
					pos_y,
					height,
					cropping->y,
					cropping->height,
					&pos_y,
					&height);

				for (int y = pos_y; y < (pos_y + height); ++y)
				{
					for (int x = pos_x; x < pos_x + width; ++x)
					{
						rzb->argb[(y * rzb->argb_width) + x] = context->color_background;
					}
				}

			}

			// TODO render text
		}
	}
}

bool rzb_event_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_tabs* data = widget->data_widget;
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

void rzb_event_widget_tabs_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_tabs_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
