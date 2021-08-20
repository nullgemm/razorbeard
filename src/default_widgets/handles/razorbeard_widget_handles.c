#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_handles.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// handles

struct rzb_widget*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal,
		int sections_count)
{
	if (sections_count == 0)
	{
		return NULL;
	}

	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->children_count = 0;
	widget->children_limit = sections_count;
	widget->render = true;
	widget->hide = false;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = rzb_event_widget_handles;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_handles handles =
	{
		.context = context,
		.horizontal = horizontal,
		.sections_count = sections_count,
		.section_lengths = malloc(sections_count * sizeof (int)),
		.section_dragging = NULL,
	};

	if (handles.section_lengths == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_handles* data =
		malloc(sizeof (struct rzb_widget_handles));

	if (data == NULL)
	{
		free(handles.section_lengths);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = handles;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data = widget->data_widget;

	free(data->section_lengths);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_handles* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int handle_pos = 0;
	int handle_pos_min = 0;
	int handle_pos_max = 0;
	int handle_rect_1_pos = 0;
	int handle_rect_2_pos = 0;
	int handle_rect_1_size = 0;
	int handle_rect_2_size = 0;
	int handle_dots_size = 0;

	int widget_thickness_pos;
	int widget_thickness_size;
	int crop_thickness_min;
	int crop_thickness_max;
	int widget_length_pos;
	int widget_length_size;
	int crop_length_pos;
	int crop_length_size;

	int center_offset = context->sizes_current->size_handle % 2;
	int circle_x;
	int circle_y;
	int radius;

	if (data->horizontal)
	{
		widget_thickness_pos = widget->x;
		widget_thickness_size = widget->width;
		crop_thickness_min = cropping->x;
		crop_thickness_max = cropping->x + cropping->width;
		widget_length_pos = widget->y;
		widget_length_size = widget->height;
		crop_length_pos = cropping->y;
		crop_length_size = cropping->height;
	}
	else
	{
		widget_thickness_pos = widget->y;
		widget_thickness_size = widget->height;
		crop_thickness_min = cropping->y;
		crop_thickness_max = cropping->y + cropping->height;
		widget_length_pos = widget->x;
		widget_length_size = widget->width;
		crop_length_pos = cropping->x;
		crop_length_size = cropping->width;
	}

	// repeat for each handle
	for (int i = 0; i < (data->sections_count - 1); ++i)
	{
		handle_pos += data->section_lengths[i];

		// get handle bounds
		if (handle_pos
			< (context->sizes_current->size_handle / 2))
		{
			handle_pos_min = 0;
		}
		else
		{
			handle_pos_min =
				handle_pos - (context->sizes_current->size_handle / 2);
		}

		if (handle_pos_min
			> (widget_thickness_size - context->sizes_current->size_handle))
		{
			handle_pos_max = widget_thickness_size;
		}
		else
		{
			handle_pos_max =
				handle_pos_min + context->sizes_current->size_handle;
		}

		// get handle bounds positions
		handle_pos_min += widget_thickness_pos;
		handle_pos_max += widget_thickness_pos;

		// crop handle bounds positions
		if (handle_pos_min < crop_thickness_min)
		{
			handle_pos_min = crop_thickness_min;
		}

		if (handle_pos_max > crop_thickness_max)
		{
			handle_pos_max = crop_thickness_max;
		}

		// get handle rectangles bounds
		handle_dots_size =
			(4 * (2 * context->sizes_current->radius_handle + center_offset))
			+ (3 * context->sizes_current->padding_handle);

		handle_rect_1_pos =
			widget_length_pos 
			+ context->sizes_current->padding_handle;

		handle_rect_1_size =
			((widget_length_size - handle_dots_size) / 2)
			- (2 * context->sizes_current->padding_handle);

		if (handle_rect_1_size < 0)
		{
			handle_rect_1_size = 0;
		}

		handle_rect_2_pos =
			widget_length_pos
			+ handle_rect_1_size
			+ handle_dots_size
			+ (3 * context->sizes_current->padding_handle);

		handle_rect_2_size =
			handle_rect_1_size;

		// crop handle bounds positions, first rectangle
		rzb_helper_crop_rectangle(
			handle_rect_1_pos,
			handle_rect_1_size,
			crop_length_pos,
			crop_length_size,
			&handle_rect_1_pos,
			&handle_rect_1_size);

		// crop handle bounds positions, second rectangle
		rzb_helper_crop_rectangle(
			handle_rect_2_pos,
			handle_rect_2_size,
			crop_length_pos,
			crop_length_size,
			&handle_rect_2_pos,
			&handle_rect_2_size);

		// render handle bars
		if (data->horizontal)
		{
			for (int x = handle_pos_min; x < handle_pos_max; ++x)
			{
				// first rectangle
				for (int y = 0; y < handle_rect_1_size; ++y)
				{
					rzb->argb[(y + handle_rect_1_pos) * rzb->argb_width + x] =
						context->color_foreground;
				}

				// second rectangle
				for (int y = 0; y < handle_rect_2_size; ++y)
				{
					rzb->argb[(y + handle_rect_2_pos) * rzb->argb_width + x] =
						context->color_foreground;
				}
			}
		}
		else
		{
			for (int y = handle_pos_min; y < handle_pos_max; ++y)
			{
				// first rectangle
				for (int x = 0; x < handle_rect_1_size; ++x)
				{
					rzb->argb[y * rzb->argb_width + x + handle_rect_1_pos] =
						context->color_foreground;
				}

				// second rectangle
				for (int x = 0; x < handle_rect_2_size; ++x)
				{
					rzb->argb[y * rzb->argb_width + x + handle_rect_2_pos] =
						context->color_foreground;
				}
			}
		}

		// render handle dots
		radius = context->sizes_current->radius_handle;
		circle_y = handle_pos + widget_thickness_pos;

		for (int i = 0; i < 4; ++i)
		{
			circle_x =
				widget_length_pos
				+ ((widget_length_size - handle_dots_size) / 2)
				+ i * (2 * radius
					+ center_offset
					+ context->sizes_current->padding_handle)
				+ radius;

			if (data->horizontal)
			{
				rzb_helper_render_circle(
					rzb->argb,
					rzb->argb_width,
					cropping,
					circle_y,
					circle_x,
					radius,
					center_offset,
					context->color_foreground);
			}
			else
			{
				rzb_helper_render_circle(
					rzb->argb,
					rzb->argb_width,
					cropping,
					circle_x,
					circle_y,
					radius,
					center_offset,
					context->color_foreground);
			}
		}
	}
}

bool rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_handles* data = widget->data_widget;
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

void rzb_event_widget_handles_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_handles_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}
