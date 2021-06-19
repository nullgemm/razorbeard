#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_default_widgets.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

bool rzb_default_widgets_init(
	struct rzb_default_widgets_context* context,
	void** events_data,
	int* events_table)
{
	context->events_table = events_table;
	context->events_data_typed_string = (char**) events_data;
	context->events_data_mouse_pos_x = 0;
	context->events_data_mouse_pos_y = 0;

	struct rzb_default_widgets_sizes sizes_density_low =
	{
		.size_shadow = 3,
		.size_edge_border = 1,
		.size_selected_border = 1,
		.size_font = 10,

		.size_shine_edge = 1,
		.size_shine_gradient = 15,
		.radius_edge_border = 3,

		.size_slider = 6,
		.size_separator = 1,
		.size_textarea_corner = 6,
		.size_arrow_base = 3,

		.size_textbox_scrollbar = 6,
		.padding_textbox_scrollbar = 1,

		.size_tab_separator = 1,
		.padding_tab_separator = 4,
		.padding_tab_button = 1,

		.size_handle = 1,
		.radius_handle = 2,
		.padding_handle = 2,

		.tab_default_width = 44,
		.tab_default_height = 16,

		.padding_bar = 1,
		.padding_textbox = 3,
		.padding_checkbox = 1,
		.padding_radiobutton = 2,
	};

	struct rzb_default_widgets_sizes sizes_density_medium =
	{
		.size_shadow = 5,
		.size_edge_border = 1,
		.size_selected_border = 2,
		.size_font = 12,

		.size_shine_edge = 1,
		.size_shine_gradient = 30,
		.radius_edge_border = 4,

		.size_slider = 8,
		.size_separator = 1,
		.size_textarea_corner = 12,
		.size_arrow_base = 7,

		.size_textbox_scrollbar = 8,
		.padding_textbox_scrollbar = 2,

		.size_tab_separator = 2,
		.padding_tab_separator = 7,
		.padding_tab_button = 4,

		.size_handle = 2,
		.radius_handle = 2,
		.padding_handle = 3,

		.tab_default_width = 106,
		.tab_default_height = 33,

		.padding_bar = 1,
		.padding_textbox = 6,
		.padding_checkbox = 3,
		.padding_radiobutton = 5,
	};

	struct rzb_default_widgets_sizes sizes_density_high =
	{
		.size_shadow = 8,
		.size_edge_border = 2,
		.size_selected_border = 3,
		.size_font = 18,

		.size_shine_edge = 2,
		.size_shine_gradient = 45,
		.radius_edge_border = 7,

		.size_slider = 12,
		.size_separator = 2,
		.size_textarea_corner = 18,
		.size_arrow_base = 14,

		.size_textbox_scrollbar = 12,
		.padding_textbox_scrollbar = 3,

		.size_tab_separator = 3,
		.padding_tab_separator = 11,
		.padding_tab_button = 8,

		.size_handle = 3,
		.radius_handle = 3,
		.padding_handle = 5,

		.tab_default_width = 159,
		.tab_default_height = 49,

		.padding_bar = 2,
		.padding_textbox = 9,
		.padding_checkbox = 5,
		.padding_radiobutton = 7,
	};

	context->sizes_current = &(context->sizes_density_medium);
	context->sizes_density_low = sizes_density_low;
	context->sizes_density_medium = sizes_density_medium;
	context->sizes_density_high = sizes_density_high;

	context->color_shadow = 0xcc000000;
	context->color_edge = 0xff0e0e0e;
	context->color_selected = 0xff2b527b;
	context->color_text = 0xffeaeaea;
	context->color_background = 0xff292929;
	context->color_foreground = 0xff191919;
	context->color_background_box = 0xff232323;
	context->color_foreground_shine = 0xff1e1e1e;

	return true;
}

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context)
{
	// only here as a template in case of developer modifications,
	// it is not actually needed by default as we don't make
	// dynamic memory allocations in the widgets context

	return true;
}

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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = sections_count;

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

	// fill the widget's background

	for (int y = cropping->y; y < cropping->y + cropping->height; ++y)
	{
		for (int x = cropping->x; x < cropping->x + cropping->width; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_background;
		}
	}

	// render handles

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
		circle_y = handle_pos;

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

void rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

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

void rzb_event_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

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
		&& (widget->width < (2 * context->sizes_current->tab_default_height)))
	{
		return;
	}

	int pos_x = widget->x;
	int width = widget->width;
	int pos_y = widget->y + context->sizes_current->tab_default_height;
	int height = widget->height - context->sizes_current->tab_default_height;

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

void rzb_event_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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

// popup

struct rzb_widget*
	rzb_alloc_widget_popup(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_popup;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_popup;
	widget->data_widget = context;

	return widget;
}

void rzb_free_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_default_widgets_context* context = widget->data_widget;

	int thickness = context->sizes_current->size_shadow;

	rzb_helper_render_shadow_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x - thickness,
		widget->y - thickness,
		widget->width + (2 * thickness),
		widget->height + (2 * thickness),
		context->sizes_current->radius_edge_border + thickness + 1,
		thickness + 1,
		context->color_shadow);

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
		context->color_foreground_shine);
}

// dropmenu

struct rzb_widget*
	rzb_alloc_widget_dropmenu(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_dropmenu;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_dropmenu;
	widget->data_widget = context;

	return widget;
}

void rzb_free_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

// separator

struct rzb_widget*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_separator;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_separator;
	widget->data_widget = context;

	return widget;
}

void rzb_free_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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

// image

struct rzb_widget*
	rzb_alloc_widget_image(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool alpha_premult,
		uint32_t gamma,
		uint32_t* rgba)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_image image =
	{
		.context = context,
		.alpha_premult = alpha_premult,
		.gamma = gamma,
		.rgba = rgba,
	};

	struct rzb_widget_image* data =
		malloc(sizeof (struct rzb_widget_image));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = image;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}

void rzb_event_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}

void rzb_event_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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

// textbox

struct rzb_widget*
	rzb_alloc_widget_textbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*text_changed)(struct rzb*, struct rzb_widget*),
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_textbox;
	widget->callback_events = rzb_event_widget_textbox;
	widget->callback_free = rzb_free_widget_textbox;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_textbox textbox =
	{
		.context = context,
		.text_changed = text_changed,
		.cursor_beg = NULL,
		.cursor_end = NULL,
		.text = strdup(text),
	};

	if (textbox.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_textbox* data =
		malloc(sizeof (struct rzb_widget_textbox));

	if (data == NULL)
	{
		free(textbox.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = textbox;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_textbox* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}

void rzb_event_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}

void rzb_event_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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

// checkbox

struct rzb_widget*
	rzb_alloc_widget_checkbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*checkbox_changed)(struct rzb*, struct rzb_widget*),
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_checkbox;
	widget->callback_events = rzb_event_widget_checkbox;
	widget->callback_free = rzb_free_widget_checkbox;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_checkbox checkbox =
	{
		.context = context,
		.checkbox_changed = checkbox_changed,
		.checked = checked,
	};

	// prepare persistent memory area
	struct rzb_widget_checkbox* data =
		malloc(sizeof (struct rzb_widget_checkbox));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = checkbox;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_checkbox_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_checkbox_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

// scrollbar

struct rzb_widget*
	rzb_alloc_widget_scrollbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*scrollbar_moved)(struct rzb*, struct rzb_widget*))
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_scrollbar;
	widget->callback_events = rzb_event_widget_scrollbar;
	widget->callback_free = rzb_free_widget_scrollbar;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_scrollbar scrollbar =
	{
		.context = context,
		.scrollbar_moved = scrollbar_moved,
		.pushed = false,
		.size_scrollbar = 0,
		.position_scrollbar = 0,
	};

	// prepare persistent memory area
	struct rzb_widget_scrollbar* data =
		malloc(sizeof (struct rzb_widget_scrollbar));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = scrollbar;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_scrollbar_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_scrollbar_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}

void rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

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
	// TODO
}
