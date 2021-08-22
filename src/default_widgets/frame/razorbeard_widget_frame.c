#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_frame.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// frame

struct rzb_widget*
	rzb_alloc_widget_frame(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* title,
		void (*callback_interactive)(void*, enum rzb_widget_frame_status),
		void (*callback_interactive_hover)(void*, enum rzb_widget_frame_status),
		void (*callback_frame_action)(void*, enum rzb_widget_frame_action),
		void* callback_data,
		void* callback_hover_data,
		void* callback_action_data)
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
		.callback_interactive_hover = callback_interactive_hover,
		.callback_frame_action = callback_frame_action,
		.callback_data = callback_data,
		.callback_hover_data = callback_hover_data,
		.callback_action_data = callback_action_data,
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
			enum rzb_widget_frame_status status;

			// click release

			if (event_state == RZB_STATE_RELEASE)
			{
				data->status = RZB_WIDGET_FRAME_IDLE;
				data->callback_interactive(data->callback_data, data->status);
				widget->render = true;
				return true;
			}

			// interactive move and resize

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
					data->callback_interactive(data->callback_data, data->status);
					widget->render = true;
					return true;
				}
				default:
				{
					break;
				}
			}

			// buttons

			switch (data->status)
			{
				case RZB_WIDGET_FRAME_HOVER_CLOSE:
				{
					status = RZB_WIDGET_FRAME_PRESS_CLOSE;
					data->callback_frame_action(data->callback_action_data, RZB_WIDGET_FRAME_CLOSE);
					break;
				}
				case RZB_WIDGET_FRAME_HOVER_MAX:
				{
					status = RZB_WIDGET_FRAME_PRESS_MAX;
					data->callback_frame_action(data->callback_action_data, RZB_WIDGET_FRAME_MAXIMIZE);
					break;
				}
				case RZB_WIDGET_FRAME_HOVER_MIN:
				{
					status = RZB_WIDGET_FRAME_PRESS_MIN;
					data->callback_frame_action(data->callback_action_data, RZB_WIDGET_FRAME_MINIMIZE);
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

			return false;
		}
		case RZB_MOUSE_MOTION:
		{
			int frame_height = context->sizes_current->frame_default_height;
			int frame_border = context->sizes_current->frame_border_size;
			int frame_button = context->sizes_current->frame_button_size;
			bool hit;

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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
				return true;
			}

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

			if (hit == true)
			{
				data->status = status;
				data->callback_interactive_hover(data->callback_hover_data, RZB_WIDGET_FRAME_MOVE);
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
				data->callback_interactive_hover(data->callback_hover_data, data->status);
				return true;
			}

			data->status = RZB_WIDGET_FRAME_IDLE;
			data->callback_interactive_hover(data->callback_hover_data, data->status);
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
