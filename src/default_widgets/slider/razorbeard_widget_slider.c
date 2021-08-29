#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_slider.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// fsm update callbacks

static bool update_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_slider* slider = widget->data_widget;
	struct rzb_default_widgets_context* context = slider->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			int x;
			int y; 
			int width;
			int height;
			int pos_x_circle;
			int pos_y_circle;
			int radius_circle;
			int offset_circle;

			if (slider->vertical == true)
			{
				pos_x_circle =
					widget->x
						+ (widget->width / 2);
				pos_y_circle =
					widget->y
						+ widget->height
						- context->sizes_current->radius_edge_border
						- (widget->height - (2 * context->sizes_current->radius_edge_border))
							* slider->progress / 100;

				radius_circle = widget->width / 2;
				offset_circle = widget->width % 2;

				x = pos_x_circle - radius_circle - offset_circle;
				y = pos_y_circle - radius_circle - offset_circle;
				width = (2 * radius_circle) + offset_circle;
				height = (2 * radius_circle) + offset_circle;
			}
			else
			{
				pos_x_circle =
					widget->x
						+ context->sizes_current->radius_edge_border
						+ (widget->width - (2 * context->sizes_current->radius_edge_border))
							* slider->progress / 100;
				pos_y_circle =
					widget->y
						+ (widget->height / 2);

				radius_circle = widget->height / 2;
				offset_circle = widget->height % 2;

				x = pos_x_circle - radius_circle - offset_circle;
				y = pos_y_circle - radius_circle - offset_circle;
				width = (2 * radius_circle) + offset_circle;
				height = (2 * radius_circle) + offset_circle;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					x,
					y,
					width,
					height);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_HOVERING);

				rzb_helper_transition_callback(slider->button_on_area, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_KEY_SPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				slider->progress += 1;

				if (slider->progress > 100)
				{
					slider->progress = 100;
				}

				handled = true;
			}

			break;
		}
		case RZB_KEY_BACKSPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				slider->progress -= 1;

				if (slider->progress < 0)
				{
					slider->progress = 0;
				}

				handled = true;
			}

			break;
		}
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

				handled = true;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

static bool update_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_slider* slider = widget->data_widget;
	struct rzb_default_widgets_context* context = slider->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				if (slider->vertical == true)
				{
					slider->selection_offset =
						context->events_data.mouse_pos_y
						- (widget->y
							+ widget->height
							- context->sizes_current->radius_edge_border
							- (widget->height - (2 * context->sizes_current->radius_edge_border))
								* slider->progress / 100);
				}
				else
				{
					slider->selection_offset =
						context->events_data.mouse_pos_x
						- (widget->x
							+ context->sizes_current->radius_edge_border
							+ (widget->width - (2 * context->sizes_current->radius_edge_border))
								* slider->progress / 100);
				}

				rzb_helper_transition_callback(slider->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			int x;
			int y; 
			int width;
			int height;
			int pos_x_circle;
			int pos_y_circle;
			int radius_circle;
			int offset_circle;

			if (slider->vertical == true)
			{
				pos_x_circle =
					widget->x
						+ (widget->width / 2);
				pos_y_circle =
					widget->y
						+ widget->height
						- context->sizes_current->radius_edge_border
						- (widget->height - (2 * context->sizes_current->radius_edge_border))
							* slider->progress / 100;

				radius_circle = widget->width / 2;
				offset_circle = widget->width % 2;

				x = pos_x_circle - radius_circle - offset_circle;
				y = pos_y_circle - radius_circle - offset_circle;
				width = (2 * radius_circle) + offset_circle;
				height = (2 * radius_circle) + offset_circle;
			}
			else
			{
				pos_x_circle =
					widget->x
						+ context->sizes_current->radius_edge_border
						+ (widget->width - (2 * context->sizes_current->radius_edge_border))
							* slider->progress / 100;
				pos_y_circle =
					widget->y
						+ (widget->height / 2);

				radius_circle = widget->height / 2;
				offset_circle = widget->height % 2;

				x = pos_x_circle - radius_circle - offset_circle;
				y = pos_y_circle - radius_circle - offset_circle;
				width = (2 * radius_circle) + offset_circle;
				height = (2 * radius_circle) + offset_circle;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					x,
					y,
					width,
					height);

			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(slider->button_off_area, rzb, widget);
			}

			handled = true;

			break;
		}
		case RZB_KEY_SPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				slider->progress += 1;

				if (slider->progress > 100)
				{
					slider->progress = 100;
				}

				handled = true;
			}

			break;
		}
		case RZB_KEY_BACKSPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				slider->progress -= 1;

				if (slider->progress < 0)
				{
					slider->progress = 0;
				}

				handled = true;
			}

			break;
		}
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

				handled = true;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

static bool update_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_slider* slider = widget->data_widget;
	struct rzb_default_widgets_context* context = slider->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				int x;
				int y; 
				int width;
				int height;
				int pos_x_circle;
				int pos_y_circle;
				int radius_circle;
				int offset_circle;

				if (slider->vertical == true)
				{
					pos_x_circle =
						widget->x
							+ (widget->width / 2);
					pos_y_circle =
						widget->y
							+ widget->height
							- context->sizes_current->radius_edge_border
							- (widget->height - (2 * context->sizes_current->radius_edge_border))
								* slider->progress / 100;

					radius_circle = widget->width / 2;
					offset_circle = widget->width % 2;

					x = pos_x_circle - radius_circle - offset_circle;
					y = pos_y_circle - radius_circle - offset_circle;
					width = (2 * radius_circle) + offset_circle;
					height = (2 * radius_circle) + offset_circle;
				}
				else
				{
					pos_x_circle =
						widget->x
							+ context->sizes_current->radius_edge_border
							+ (widget->width - (2 * context->sizes_current->radius_edge_border))
								* slider->progress / 100;
					pos_y_circle =
						widget->y
							+ (widget->height / 2);

					radius_circle = widget->height / 2;
					offset_circle = widget->height % 2;

					x = pos_x_circle - radius_circle - offset_circle;
					y = pos_y_circle - radius_circle - offset_circle;
					width = (2 * radius_circle) + offset_circle;
					height = (2 * radius_circle) + offset_circle;
				}

				bool hit =
					rzb_helper_event_mouse_in_rect(
						context,
						x,
						y,
						width,
						height);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(slider->fsm_slider),
						RZB_FSM_BUTTON_STATE_HOVERING);
				}
				else
				{
					rzb_fsm_button_set_state(
						&(slider->fsm_slider),
						RZB_FSM_BUTTON_STATE_IDLING);
				}

				rzb_helper_transition_callback(slider->button_released, rzb, widget);
				handled = true;

				break;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{ 
			int mouse;
			int bar_min;
			int bar_size;

			int end_radius =
				context->sizes_current->radius_edge_border;

			if (slider->vertical == true)
			{
				mouse = context->events_data.mouse_pos_y;
				bar_min = widget->y + widget->height - end_radius;
				bar_size = widget->height - (2 * end_radius);
			}
			else
			{
				mouse = context->events_data.mouse_pos_x;
				bar_min = widget->x + end_radius;
				bar_size = widget->width - (2 * end_radius);
			}

			slider->progress =
				100
				* (mouse - slider->selection_offset - bar_min)
				/ bar_size;

			if (slider->progress > 100)
			{
				slider->progress = 100;
			}
			else if (slider->progress < 0)
			{
				slider->progress = 0;
			}

			rzb_helper_transition_callback(slider->button_dragged, rzb, widget);

			widget->render = true;

			break;
		}
		case RZB_KEY_SPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				slider->progress += 1;

				if (slider->progress > 100)
				{
					slider->progress = 100;
				}

				handled = true;
				break;
			}

			if ((event_state == RZB_STATE_RELEASE)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_IDLING);

				handled = true;
				break;
			}

			break;
		}
		case RZB_KEY_BACKSPACE:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				slider->progress -= 1;

				if (slider->progress < 0)
				{
					slider->progress = 0;
				}

				handled = true;
				break;
			}

			if ((event_state == RZB_STATE_RELEASE)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(slider->fsm_slider),
					RZB_FSM_BUTTON_STATE_IDLING);

				handled = true;
				break;
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_dragged)(struct rzb*, struct rzb_widget*),
		void* slider_data,
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

		.button_on_area = button_on_area,
		.button_off_area = button_off_area,
		.button_pressed = button_pressed,
		.button_released = button_released,
		.button_dragged = button_dragged,
		.slider_data = slider_data,

		.vertical = vertical,
		.progress = progress,
		.selection_offset = 0,
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

	// prepare the fsm
	bool (*update[RZB_FSM_BUTTON_STATE_COUNT])(struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_dragging,
	};

	rzb_fsm_button_init(
		&(data->fsm_slider),
		widget,
		update);

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
	struct rzb_widget_slider* data = widget->data_widget;

	return rzb_fsm_button_update(rzb, &(data->fsm_slider), event_code, event_state);
}
