#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_checkbox.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

// fsm update callbacks

static bool update_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_checkbox* checkbox = widget->data_widget;
	struct rzb_default_widgets_context* context = checkbox->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					widget->width,
					widget->height);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_HOVERING);

				rzb_helper_transition_callback(checkbox->button_on_area, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_KEY_ENTER:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_DRAGGING);

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
	struct rzb_widget_checkbox* checkbox = widget->data_widget;
	struct rzb_default_widgets_context* context = checkbox->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(checkbox->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					widget->width,
					widget->height);
			
			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(checkbox->button_off_area, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_KEY_ENTER:
		{
			if ((event_state == RZB_STATE_PRESS)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_DRAGGING);

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
	struct rzb_widget_checkbox* checkbox = widget->data_widget;
	struct rzb_default_widgets_context* context = checkbox->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				bool hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x,
						widget->y,
						widget->width,
						widget->height);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(checkbox->fsm_checkbox),
						RZB_FSM_BUTTON_STATE_HOVERING);

					checkbox->checked = !checkbox->checked;
					widget->render = true;

					rzb_helper_transition_callback(checkbox->button_released_on_area, rzb, widget);
				}
				else
				{
					rzb_fsm_button_set_state(
						&(checkbox->fsm_checkbox),
						RZB_FSM_BUTTON_STATE_IDLING);

					rzb_helper_transition_callback(checkbox->button_released_off_area, rzb, widget);
				}

				handled = true;
			}

			break;
		}
		case RZB_KEY_ENTER:
		{
			if ((event_state == RZB_STATE_RELEASE)
			&& (rzb->events_grabber == widget))
			{
				rzb_fsm_button_set_state(
					&(checkbox->fsm_checkbox),
					RZB_FSM_BUTTON_STATE_IDLING);

				checkbox->checked = !checkbox->checked;
				widget->render = true;

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

// checkbox

struct rzb_widget*
	rzb_alloc_widget_checkbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_released_off_area)(struct rzb*, struct rzb_widget*),
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
		.button_on_area = button_on_area,
		.button_off_area = button_off_area,
		.button_pressed = button_pressed,
		.button_released_on_area = button_released_on_area,
		.button_released_off_area = button_released_off_area,
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

	// prepare the fsm
	bool (*update[RZB_FSM_BUTTON_STATE_COUNT])(struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_dragging,
	};

	rzb_fsm_button_init(
		&(data->fsm_checkbox),
		widget,
		update);

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

	if (data->checked == true)
	{
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
	}

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
	struct rzb_widget_checkbox* data = widget->data_widget;

	return rzb_fsm_button_update(rzb, &(data->fsm_checkbox), event_code, event_state);
}
