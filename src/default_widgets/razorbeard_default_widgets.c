#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

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

	context->sizes_current = &(context->sizes_density_medium);
	context->sizes_density_low = sizes_density_low;
	context->sizes_density_medium = sizes_density_medium;
	context->sizes_density_high = sizes_density_high;

	context->color_shadow = 0x66000000;
	context->color_frame = 0xff1b1b1b;
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

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context)
{
	// only here as a template in case of developer modifications,
	// it is not actually needed by default as we don't make
	// dynamic memory allocations in the widgets context

	return true;
}

// frame

struct rzb_widget*
	rzb_alloc_widget_frame(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* title,
		void (*callback_interactive)(void*, enum rzb_widget_frame_status),
		void* callback_data)
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
	widget->callback_render = rzb_render_widget_frame;
	widget->callback_events = rzb_event_widget_frame;
	widget->callback_free = rzb_free_widget_frame;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_frame frame =
	{
		.context = context,
		.title = strdup(title),
		.callback_interactive = callback_interactive,
		.callback_data = callback_data,
		.status = RZB_WIDGET_FRAME_IDLE,
	};

	if (frame.title == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_frame* data =
		malloc(sizeof (struct rzb_widget_frame));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = frame;
	widget->data_widget = data;

	return widget;
}

void rzb_free_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	struct rzb_widget_frame* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int min_size = 
		context->sizes_current->frame_default_height
		+ context->sizes_current->frame_border_size;

	if ((widget->height <= min_size)
		|| (widget->width <= (2 * context->sizes_current->frame_border_size)))
	{
		return;
	}

	int pos_x =
		widget->x;
	int width =
		widget->width;

	int pos_y =
		widget->y;
	int height =
		context->sizes_current->frame_default_height;

	int pos_y_bottom =
		widget->y
		+ widget->height
		- context->sizes_current->frame_border_size;
	int height_bottom =
		context->sizes_current->frame_border_size;

	int pos_y_sides =
		widget->y
		+ context->sizes_current->frame_default_height;
	int height_sides =
		widget->height
		- context->sizes_current->frame_default_height
		- context->sizes_current->frame_border_size;

	int pos_x_right =
		widget->x
		+ widget->width
		- context->sizes_current->frame_border_size;
	int width_right =
		context->sizes_current->frame_border_size;

	int pos_x_left =
		widget->x;
	int width_left =
		context->sizes_current->frame_border_size;

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
		pos_y_bottom,
		height_bottom,
		cropping->y,
		cropping->height,
		&pos_y_bottom,
		&height_bottom);

	rzb_helper_crop_rectangle(
		pos_y_sides,
		height_sides,
		cropping->y,
		cropping->height,
		&pos_y_sides,
		&height_sides);

	rzb_helper_crop_rectangle(
		pos_x_right,
		width_right,
		cropping->x,
		cropping->width,
		&pos_x_right,
		&width_right);

	rzb_helper_crop_rectangle(
		pos_x_left,
		width_left,
		cropping->x,
		cropping->width,
		&pos_x_left,
		&width_left);

	for (int y = pos_y; y < (pos_y + height); ++y)
	{
		for (int x = pos_x; x < pos_x + width; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_frame;
		}
	}

	for (int y = pos_y_bottom; y < (pos_y_bottom + height_bottom); ++y)
	{
		for (int x = pos_x; x < pos_x + width; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_frame;
		}
	}

	for (int y = pos_y_sides; y < (pos_y_sides + height_sides); ++y)
	{
		for (int x = pos_x_left; x < pos_x_left + width_left; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_frame;
		}
	}

	for (int y = pos_y_sides; y < (pos_y_sides + height_sides); ++y)
	{
		for (int x = pos_x_right; x < pos_x_right + width_right; ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_frame;
		}
	}

	int frame_height = context->sizes_current->frame_default_height;
	int frame_border = context->sizes_current->frame_border_size;
	int frame_button = context->sizes_current->frame_button_size;

	// hover rectangle
	int button_hover;

	switch (data->status)
	{
		case RZB_WIDGET_FRAME_PRESS_MIN:
		case RZB_WIDGET_FRAME_HOVER_MIN:
		{
			button_hover = 3;
			break;
		}
		case RZB_WIDGET_FRAME_PRESS_MAX:
		case RZB_WIDGET_FRAME_HOVER_MAX:
		{
			button_hover = 2;
			break;
		}
		case RZB_WIDGET_FRAME_PRESS_CLOSE:
		case RZB_WIDGET_FRAME_HOVER_CLOSE:
		{
			button_hover = 1;
			break;
		}
		default:
		{
			button_hover = 0;
			break;
		}
	}

	if (button_hover > 0)
	{
		int pos_y_hover = widget->y + frame_border;
		int height_hover = frame_height - (2 * frame_border);
		int pos_x_hover = widget->x + widget->width - frame_border - (button_hover * frame_button);
		int width_hover = frame_button;

		rzb_helper_crop_rectangle(
			pos_x_hover,
			width_hover,
			cropping->x,
			cropping->width,
			&pos_x_hover,
			&width_hover);

		rzb_helper_crop_rectangle(
			pos_y_hover,
			height_hover,
			cropping->y,
			cropping->height,
			&pos_y_hover,
			&height_hover);

		for (int y = pos_y_hover; y < (pos_y_hover + height_hover); ++y)
		{
			for (int x = pos_x_hover; x < pos_x_hover + width_hover; ++x)
			{
				rzb->argb[(y * rzb->argb_width) + x] = context->color_background;
			}
		}
	}

	// min
	int width_minimize =
		frame_height / 4;
	int pos_x_minimize =
		widget->x
		+ widget->width
		- frame_border
		- (5 * frame_button / 2)
		- (width_minimize / 2);

	int height_minimize =
		2;
	int pos_y_minimize =
		widget->y
		+ (frame_height / 2)
		- (height_minimize / 2);

	rzb_helper_crop_rectangle(
		pos_x_minimize,
		width_minimize,
		cropping->x,
		cropping->width,
		&pos_x_minimize,
		&width_minimize);

	rzb_helper_crop_rectangle(
		pos_y_minimize,
		height_minimize,
		cropping->y,
		cropping->height,
		&pos_y_minimize,
		&height_minimize);

	for (int y = pos_y_minimize; y < (pos_y_minimize + height_minimize); ++y)
	{
		for (int x = pos_x_minimize; x < (pos_x_minimize + width_minimize); ++x)
		{
			rzb->argb[(y * rzb->argb_width) + x] = context->color_text;
		}
	}

	// max
	int width_maximize =
		frame_height * 3 / 8;
	int pos_x_maximize =
		widget->x
		+ widget->width
		- frame_border
		- (3 * frame_button / 2)
		- (width_maximize / 2);

	int height_maximize =
		(frame_height * 3) / 8;
	int pos_y_maximize =
		widget->y
		+ (frame_height / 2)
		- (height_maximize / 2);

	rzb_helper_render_maximized(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x_maximize,
		pos_y_maximize,
		width_maximize,
		height_maximize,
		2,
		1,
		context->color_text);

	// cross
	rzb_helper_render_cross(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width - frame_border - (frame_button / 2) - (1 - (frame_button % 2)),
		widget->y + (frame_height / 2) - (1 - (frame_height % 2)),
		(frame_height * 3) / 7,
		(frame_height * 3) % 7,
		1,
		context->color_text);
}

bool rzb_helper_event_mouse_in_rect(
	struct rzb_default_widgets_context* context,
	int pos_x_minimize,
	int pos_y_minimize,
	int width_minimize,
	int height_minimize)
{
	int x = context->events_data.mouse_pos_x;
	int y = context->events_data.mouse_pos_y;

	if (x < pos_x_minimize)
	{
		return false;
	}

	if (y < pos_y_minimize)
	{
		return false;
	}

	if (x > (pos_x_minimize + width_minimize))
	{
		return false;
	}

	if (y > (pos_y_minimize + height_minimize))
	{
		return false;
	}

	return true;
}

bool rzb_event_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
	struct rzb_widget_frame* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			int frame_height = context->sizes_current->frame_default_height;
			int frame_border = context->sizes_current->frame_border_size;

			bool hit;
			enum rzb_widget_frame_status status;

			// click release

			if (event_state == RZB_STATE_RELEASE)
			{
				data->status = RZB_WIDGET_FRAME_IDLE;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			// corners
			
			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					2 * frame_border,
					2 * frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_NW;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - (2 * frame_border),
					widget->y,
					2 * frame_border,
					2 * frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_NE;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - (2 * frame_border),
					widget->y + widget->height - (2 * frame_border),
					2 * frame_border,
					2 * frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_SE;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + widget->height - (2 * frame_border),
					2 * frame_border,
					2 * frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_SW;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			// borders

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					widget->width,
					frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_N;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame_border,
					widget->y + frame_border,
					frame_border,
					widget->height - (2 * frame_border));

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_E;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + frame_border,
					frame_border,
					widget->height - (2 * frame_border));

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_W;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + widget->height - frame_border,
					widget->width,
					frame_border);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_SIZE_S;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			// buttons

			switch (data->status)
			{
				case RZB_WIDGET_FRAME_HOVER_CLOSE:
				{
					status = RZB_WIDGET_FRAME_PRESS_CLOSE;
					break;
				}
				case RZB_WIDGET_FRAME_HOVER_MAX:
				{
					status = RZB_WIDGET_FRAME_PRESS_MAX;
					break;
				}
				case RZB_WIDGET_FRAME_HOVER_MIN:
				{
					status = RZB_WIDGET_FRAME_PRESS_MIN;
					break;
				}
				default:
				{
					status = RZB_WIDGET_FRAME_IDLE;
					break;
				}
			}

			if (data->status != status)
			{
				data->status = status;
				widget->render = true;
				return true;
			}

			// move

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					widget->width,
					frame_height);

			if (hit == true)
			{
				data->status = RZB_WIDGET_FRAME_MOVE;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			return false;
		}
		case RZB_MOUSE_MOTION:
		{
			switch (data->status)
			{
				case RZB_WIDGET_FRAME_SIZE_E:
				case RZB_WIDGET_FRAME_SIZE_NE:
				case RZB_WIDGET_FRAME_SIZE_N:
				case RZB_WIDGET_FRAME_SIZE_NW:
				case RZB_WIDGET_FRAME_SIZE_W:
				case RZB_WIDGET_FRAME_SIZE_SW:
				case RZB_WIDGET_FRAME_SIZE_S:
				case RZB_WIDGET_FRAME_SIZE_SE:
				case RZB_WIDGET_FRAME_MOVE:
				{
					return true;
				}
				default:
				{
					break;
				}
			}

			int frame_height = context->sizes_current->frame_default_height;
			int frame_border = context->sizes_current->frame_border_size;
			int frame_button = context->sizes_current->frame_button_size;

			enum rzb_widget_frame_status status = RZB_WIDGET_FRAME_IDLE;
			int pos_y_hover = widget->y + frame_border;
			int height_hover = frame_height - (2 * frame_border);
			int pos_x_hover = widget->x + widget->width - frame_border - frame_button;
			int width_hover = frame_button;

			int table[3] =
			{
				RZB_WIDGET_FRAME_HOVER_CLOSE,
				RZB_WIDGET_FRAME_HOVER_MAX,
				RZB_WIDGET_FRAME_HOVER_MIN
			};

			bool hit;
			int i = 0;

			while (i < 3)
			{
				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						pos_x_hover,
						pos_y_hover,
						width_hover,
						height_hover);

				if (hit == true)
				{
					status = table[i];
					widget->render = true;
					break;
				}

				pos_x_hover -= frame_button;
				++i;
			}

			if  (data->status != status)
			{
				data->status = status;
				widget->render = true;
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

void rzb_event_widget_frame_size_move(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_frame_close(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_frame_maximize(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_frame_minimize(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
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
	widget->children_count = 0;
	widget->children_limit = 1;
	widget->render = true;
	widget->hide = false;

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
	widget->children_count = 0;
	widget->children_limit = 1;
	widget->render = true;
	widget->hide = false;

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
		thickness + 1,
		thickness + 1,
		context->color_shadow);

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width - context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		context->sizes_current->size_edge_border,
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->color_edge);

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y,
		widget->width,
		context->sizes_current->size_edge_border,
		context->color_edge);

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y + context->sizes_current->size_edge_border,
		context->sizes_current->size_edge_border,
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->color_edge);

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y + widget->height - context->sizes_current->size_edge_border,
		widget->width,
		context->sizes_current->size_edge_border,
		context->color_edge);

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->size_edge_border,
		widget->y + context->sizes_current->size_edge_border,
		widget->width - (2 * context->sizes_current->size_edge_border),
		widget->height - (2 * context->sizes_current->size_edge_border),
		context->color_foreground_shine);
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	struct rzb_default_widgets_context* context = widget->data_widget;

	rzb_helper_render_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + context->sizes_current->padding_separator,
		widget->y + ((widget->height - context->sizes_current->size_separator) / 2),
		widget->width - (2 * context->sizes_current->padding_separator),
		context->sizes_current->size_separator,
		context->color_foreground);
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_image;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_image;
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	struct rzb_widget_textbox* data = widget->data_widget;
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

bool rzb_event_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_textbox* data = widget->data_widget;
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	struct rzb_widget_checkbox* data = widget->data_widget;
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
	int pos_x = widget->x + ((widget->width - min) / 2);
	int pos_y = widget->y + ((widget->height - min) / 2);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		pos_x,
		pos_y,
		min,
		min,
		context->sizes_current->radius_edge_border,
		false,
		context->color_edge);

	rzb_helper_render_cross(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x + widget->width / 2 - (1 - (widget->width % 2)),
		widget->y + widget->height / 2 - (1 - (widget->height % 2)),
		min - (2 * context->sizes_current->padding_checkbox),
		center_offset,
		1,
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

bool rzb_event_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_checkbox* data = widget->data_widget;
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	struct rzb_widget_scrollbar* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	// TODO stuff

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x,
		widget->y,
		widget->width,
		widget->height,
		context->sizes_current->radius_edge_border
			- context->sizes_current->size_edge_border,
		false,
		context->color_foreground_shine);

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

bool rzb_event_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state)
{
#if 0
	struct rzb_widget_scrollbar* data = widget->data_widget;
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
	widget->children_count = 0;
	widget->children_limit = 0;
	widget->render = true;
	widget->hide = false;

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
	struct rzb_widget_progressbar* data = widget->data_widget;
	struct rzb_default_widgets_context* context = data->context;

	int size_bar =
		context->sizes_current->radius_edge_border
		- (2 * context->sizes_current->size_edge_border);

	int size_inner =
		context->sizes_current->radius_edge_border
		- context->sizes_current->size_edge_border;

	int width;
	int height;
	int y;

	if (data->vertical)
	{
		width = (widget->width - (4 * context->sizes_current->size_edge_border));
		height = (2 * size_bar) + (widget->height - (2 * context->sizes_current->radius_edge_border)) * data->progress / 100;
		y = widget->y + widget->height - height - (2 * context->sizes_current->size_edge_border);
	}
	else
	{
		height = (widget->height - (4 * context->sizes_current->size_edge_border));
		width = (2 * size_bar) + (widget->width - (2 * context->sizes_current->radius_edge_border)) * data->progress / 100;
		y = widget->y + (2 * context->sizes_current->size_edge_border);
	}

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
		widget->x
			+ context->sizes_current->size_edge_border,
		widget->y
			+ context->sizes_current->size_edge_border,
		(widget->width - (2 * context->sizes_current->size_edge_border)),
		(widget->height - (2 * context->sizes_current->size_edge_border)),
		size_inner,
		false,
		context->color_foreground);

	rzb_helper_render_rounded_rectangle(
		rzb->argb,
		rzb->argb_width,
		cropping,
		widget->x
			+ (2 * context->sizes_current->size_edge_border),
		y,
		width,
		height,
		size_bar,
		false,
		context->color_accent);
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
