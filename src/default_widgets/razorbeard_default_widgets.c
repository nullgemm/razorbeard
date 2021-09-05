#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define RZB_IDEAL_BUTTON_HEIGHT 9

void rzb_default_widgets_set_events_data(void* context, void* data)
{
	struct rzb_default_widgets_context* kit = context;
	struct rzb_default_widgets_events_data* events_data = data;

	kit->events_data = *events_data;
}

bool rzb_default_widgets_init(
	struct rzb* rzb,
	struct rzb_default_widgets_context* context)
{
	context->events_data.typed_string = NULL;
	context->events_data.mouse_pos_x = 0;
	context->events_data.mouse_pos_y = 0;

	rzb_set_events_data_callback(
		rzb,
		rzb_default_widgets_set_events_data,
		context);

	struct rzb_default_widgets_sizes sizes_density_low =
	{
		.size_shadow = 3,
		.size_edge_border = 1,
		.size_selected_border = 1,
		.size_font = 10,

		.size_shine_edge = 1,
		.size_shine_gradient = 7,
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
		.padding_separator = 2,

		.size_handle = 1,
		.radius_handle = 2,
		.padding_handle = 2,

		.tab_default_width = 44,
		.tab_default_height = 16,

		.frame_default_height = 14,
		.frame_border_size = 1,
		.frame_button_size = 16,

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
		.size_shine_gradient = 14,
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
		.padding_separator = 4,

		.size_handle = 2,
		.radius_handle = 2,
		.padding_handle = 3,

		.tab_default_width = 106,
		.tab_default_height = 33,

		.frame_default_height = 28,
		.frame_border_size = 2,
		.frame_button_size = 32,

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
		.size_shine_gradient = 21,
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
		.padding_separator = 8,

		.size_handle = 3,
		.radius_handle = 3,
		.padding_handle = 5,

		.tab_default_width = 159,
		.tab_default_height = 49,

		.frame_default_height = 42,
		.frame_border_size = 3,
		.frame_button_size = 48,

		.padding_bar = 2,
		.padding_textbox = 9,
		.padding_checkbox = 5,
		.padding_radiobutton = 7,
	};

	context->sizes_scale = 1;
	context->sizes_current = &(context->sizes_density_medium);
	context->sizes_density_low = sizes_density_low;
	context->sizes_density_medium = sizes_density_medium;
	context->sizes_density_high = sizes_density_high;

	context->color_shadow = 0x66000000;
#if 0
	context->color_frame = 0xff1b1b1b;
#else
	context->color_frame = 0xff1e1e1e;
#endif
	context->color_edge = 0xff0e0e0e;
	context->color_accent= 0xff2b527b;
	context->color_selected = 0xff1d5289;
	context->color_text = 0xffeaeaea;
	context->color_background = 0xff292929;
	context->color_foreground = 0xff191919;
	context->color_background_box = 0xff232323;
	context->color_foreground_shine = 0xff1e1e1e;
	context->color_foreground_shine_gradient = 0xff1d1d1d;

	return true;
}

void rzb_default_widgets_density(
	struct rzb* rzb,
	struct rzb_default_widgets_context* context)
{
	int px_height;
	int mm_height;

	int diff_low;
	int diff_medium;
	int diff_high;
	int diff_ultra_floor;
	int diff_ultra_ceil;

	int ultra_scale_floor;
	int ultra_scale_ceil;

	int diff_min;

	// use low density
	px_height = context->sizes_density_low.tab_default_height;

	mm_height =
		px_height
		* rzb->display_info->mm_height
		/ rzb->display_info->px_height;

	diff_low = abs(mm_height - RZB_IDEAL_BUTTON_HEIGHT);

	// use medium density
	px_height = context->sizes_density_medium.tab_default_height;

	mm_height =
		px_height
		* rzb->display_info->mm_height
		/ rzb->display_info->px_height;

	diff_medium = abs(mm_height - RZB_IDEAL_BUTTON_HEIGHT);

	// use high density
	px_height = context->sizes_density_high.tab_default_height;

	mm_height =
		px_height
		* rzb->display_info->mm_height
		/ rzb->display_info->px_height;

	diff_high = abs(mm_height - RZB_IDEAL_BUTTON_HEIGHT);

	// automatically scale medium sizes for ultra-high densities
	// floor
	px_height =
		context->sizes_density_medium.tab_default_height;

	ultra_scale_floor =
		RZB_IDEAL_BUTTON_HEIGHT
		* rzb->display_info->px_height
		/ rzb->display_info->mm_height;

	mm_height =
		ultra_scale_floor
		* px_height
		* rzb->display_info->mm_height
		/ rzb->display_info->px_height;

	diff_ultra_floor = abs(mm_height - RZB_IDEAL_BUTTON_HEIGHT);

	// ceil
	px_height =
		context->sizes_density_medium.tab_default_height;

	ultra_scale_ceil =
		RZB_IDEAL_BUTTON_HEIGHT
		* rzb->display_info->px_height
		/ rzb->display_info->mm_height;

	mm_height =
		ultra_scale_ceil
		* px_height
		* rzb->display_info->mm_height
		/ rzb->display_info->px_height;

	diff_ultra_ceil = abs(mm_height - RZB_IDEAL_BUTTON_HEIGHT);

	// find the best size set but keep the medium set on equality
	context->sizes_current = &(context->sizes_density_medium);
	context->sizes_scale = rzb->display_info->scale;
	diff_min = diff_medium;

	if (diff_min > diff_low)
	{
		context->sizes_current = &(context->sizes_density_low);
		diff_min = diff_low;
	}

	if (diff_min > diff_high)
	{
		context->sizes_current = &(context->sizes_density_high);
		diff_min = diff_high;
	}

	if (diff_min > diff_ultra_floor)
	{
		context->sizes_current = &(context->sizes_density_medium);
		context->sizes_scale *= ultra_scale_floor;
		diff_min = diff_ultra_floor;
	}

	if (diff_min > diff_ultra_ceil)
	{
		context->sizes_current = &(context->sizes_density_medium);
		context->sizes_scale *= ultra_scale_ceil;
		diff_min = diff_ultra_ceil;
	}
}

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context)
{
	// only here as a template in case of developer modifications,
	// it is not actually needed by default as we don't make
	// dynamic memory allocations in the widgets context

	return true;
}

int norm(int x, int y)
{
	return isqrt((x * x) + (y * y));
}

void rzb_nearest_widget(
	struct rzb* rzb,
	struct rzb_widget* widget,
	enum rzb_default_widgets_events direction)
{
	struct rzb_widget* target = widget;
	struct rzb_widget* tmp = rzb->root_widget;
	struct rzb_widget* old;

	int ox = widget->x;
	int oy = widget->y;

	int x;
	int y;
	int distance;
	int min = norm(rzb->argb_width, rzb->argb_width);

	switch (direction)
	{
		case RZB_KEY_UP:
		{
			ox += widget->width / 2;
			break;
		}
		case RZB_KEY_DOWN:
		{
			ox += widget->width / 2;
			oy += widget->height;
			break;
		}
		case RZB_KEY_LEFT:
		{
			oy += widget->height / 2;
			break;
		}
		case RZB_KEY_RIGHT:
		{
			ox += widget->width;
			oy += widget->height / 2;
			break;
		}
		default:
		{
			break;
		}
	}

	do
	{
		do
		{
			old = tmp;
			tmp = tmp->children;
		}
		while (tmp != NULL);

		if ((old != widget)
			&& (old->callback_events != NULL)
			&& (old->children_limit == 0))
		{
			switch (direction)
			{
				case RZB_KEY_UP:
				{
					if ((old->y + old->height) > oy)
					{
						break;
					}

					x = (old->x + (old->width / 2)) - ox;
					y = (old->y + old->height) - oy;
					distance = norm(x, y);

					if (distance < min)
					{
						min = distance;
						target = old;
					}

					break;
				}
				case RZB_KEY_DOWN:
				{
					if (old->y < oy)
					{
						break;
					}

					x = (old->x + (old->width / 2)) - ox;
					y = old->y - oy;
					distance = norm(x, y);

					if (distance < min)
					{
						min = distance;
						target = old;
					}

					break;
				}
				case RZB_KEY_LEFT:
				{
					if ((old->x + old->width) > ox)
					{
						break;
					}

					x = (old->x + old->width) - ox;
					y = (old->y + (old->height / 2)) - oy;
					distance = norm(x, y);

					if (distance < min)
					{
						min = distance;
						target = old;
					}

					break;
				}
				case RZB_KEY_RIGHT:
				{
					if (old->x < ox)
					{
						break;
					}

					x = old->x - ox;
					y = (old->y + (old->height / 2)) - oy;
					distance = norm(x, y);

					if (distance < min)
					{
						min = distance;
						target = old;
					}

					break;
				}
				default:
				{
					break;
				}
			}
		}

		do
		{
			if (old == NULL)
			{
				if (target != NULL)
				{
					rzb_select_widget(rzb, target);
				}

				return;
			}

			tmp = old->siblings;
			old = old->parent;
		}
		while (tmp == NULL);
	}
	while (true);
}
