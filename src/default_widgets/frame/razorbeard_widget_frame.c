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

// fsms

bool update_frame_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit;

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y + frame->frame_border_size,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_title_size - frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_MOVE;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y + frame->frame_border_size,
					frame->frame_border_size,
					widget->height - (2 * frame->frame_border_size));
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_E;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_NE;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_N;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_NW;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + frame->frame_border_size,
					frame->frame_border_size,
					widget->height - (2 * frame->frame_border_size));
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_W;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + widget->height - frame->frame_border_size,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_SW;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y + widget->height - frame->frame_border_size,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_S;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y + widget->height - frame->frame_border_size,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_SIZE_SE;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

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

bool update_frame_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(frame->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			bool hit;

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y + frame->frame_border_size,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_title_size - frame->frame_border_size);
			
			if (hit == true)
			{
				if (frame->status != RZB_WIDGET_FRAME_MOVE)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_MOVE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y + frame->frame_border_size,
					frame->frame_border_size,
					widget->height - (2 * frame->frame_border_size));
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_E)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_E;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_NE)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_NE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_N)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_N;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_NW)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_NW;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + frame->frame_border_size,
					frame->frame_border_size,
					widget->height - (2 * frame->frame_border_size));
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_W)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_W;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x,
					widget->y + widget->height - frame->frame_border_size,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_SW)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_SW;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + frame->frame_border_size,
					widget->y + widget->height - frame->frame_border_size,
					widget->width - (2 * frame->frame_border_size),
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_S)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_S;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			hit =
				rzb_helper_event_mouse_in_rect(
					context,
					widget->x + widget->width - frame->frame_border_size,
					widget->y + widget->height - frame->frame_border_size,
					frame->frame_border_size,
					frame->frame_border_size);
			
			if (hit == true)
			{
				//if (frame->status != RZB_WIDGET_FRAME_SIZE_SE)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_off_area, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_SE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);
				}

				handled = true;
				break;
			}

			frame->status = RZB_WIDGET_FRAME_IDLE;

			rzb_helper_transition_callback(frame->button_off_area, rzb, widget);

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

bool update_frame_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			rzb_helper_transition_callback(frame->button_dragged, rzb, widget);

			handled = true;

			break;
		}
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				bool hit;
				handled = true;

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + frame->frame_border_size,
						widget->y + frame->frame_border_size,
						widget->width - (2 * frame->frame_border_size),
						frame->frame_title_size - frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_MOVE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + widget->width - frame->frame_border_size,
						widget->y + frame->frame_border_size,
						frame->frame_border_size,
						widget->height - (2 * frame->frame_border_size));
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_E;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + widget->width - frame->frame_border_size,
						widget->y,
						frame->frame_border_size,
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_NE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + frame->frame_border_size,
						widget->y,
						widget->width - (2 * frame->frame_border_size),
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_N;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x,
						widget->y,
						frame->frame_border_size,
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_NW;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x,
						widget->y + frame->frame_border_size,
						frame->frame_border_size,
						widget->height - (2 * frame->frame_border_size));
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_W;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x,
						widget->y + widget->height - frame->frame_border_size,
						frame->frame_border_size,
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_SW;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + frame->frame_border_size,
						widget->y + widget->height - frame->frame_border_size,
						widget->width - (2 * frame->frame_border_size),
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_S;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						widget->x + widget->width - frame->frame_border_size,
						widget->y + widget->height - frame->frame_border_size,
						frame->frame_border_size,
						frame->frame_border_size);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_frame_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
					frame->status = RZB_WIDGET_FRAME_SIZE_SE;
					rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

					break;
				}

				rzb_fsm_button_set_state(
					&(frame->fsm_frame_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				frame->status = RZB_WIDGET_FRAME_IDLE;

				rzb_helper_transition_callback(frame->button_released, rzb, widget);
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

bool update_min_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_min_x,
					frame->button_min_y,
					frame->button_min_width,
					frame->button_min_height);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_min_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_MINIMIZE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

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

bool update_min_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_min_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(frame->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_min_x,
					frame->button_min_y,
					frame->button_min_width,
					frame->button_min_height);
			
			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_min_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				frame->status = RZB_WIDGET_FRAME_IDLE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_off_area, rzb, widget);

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

bool update_min_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				bool hit =
					rzb_helper_event_mouse_in_rect(
						context,
						frame->button_min_x,
						frame->button_min_y,
						frame->button_min_width,
						frame->button_min_height);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_min_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}
				else
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_min_button),
						RZB_FSM_BUTTON_STATE_IDLING);

					frame->status = RZB_WIDGET_FRAME_IDLE;
					widget->render = true;

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}

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

bool update_max_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_max_x,
					frame->button_max_y,
					frame->button_max_width,
					frame->button_max_height);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_max_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_MAXIMIZE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

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

bool update_max_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_max_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(frame->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_max_x,
					frame->button_max_y,
					frame->button_max_width,
					frame->button_max_height);
			
			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_max_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				frame->status = RZB_WIDGET_FRAME_IDLE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_off_area, rzb, widget);

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

bool update_max_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				bool hit =
					rzb_helper_event_mouse_in_rect(
						context,
						frame->button_max_x,
						frame->button_max_y,
						frame->button_max_width,
						frame->button_max_height);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_max_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}
				else
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_max_button),
						RZB_FSM_BUTTON_STATE_IDLING);

					frame->status = RZB_WIDGET_FRAME_IDLE;
					widget->render = true;

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}

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

bool update_close_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_close_x,
					frame->button_close_y,
					frame->button_close_width,
					frame->button_close_height);
			
			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_close_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				frame->status = RZB_WIDGET_FRAME_CLOSE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_on_area, rzb, widget);

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

bool update_close_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_close_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(frame->button_pressed, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					frame->button_close_x,
					frame->button_close_y,
					frame->button_close_width,
					frame->button_close_height);
			
			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(frame->fsm_close_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				frame->status = RZB_WIDGET_FRAME_IDLE;
				widget->render = true;

				rzb_helper_transition_callback(frame->button_off_area, rzb, widget);

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

bool update_close_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_frame* frame = widget->data_widget;
	struct rzb_default_widgets_context* context = frame->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_RELEASE)
			{
				bool hit =
					rzb_helper_event_mouse_in_rect(
						context,
						frame->button_close_x,
						frame->button_close_y,
						frame->button_close_width,
						frame->button_close_height);
				
				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_close_button),
						RZB_FSM_BUTTON_STATE_HOVERING);

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}
				else
				{
					rzb_fsm_button_set_state(
						&(frame->fsm_close_button),
						RZB_FSM_BUTTON_STATE_IDLING);

					frame->status = RZB_WIDGET_FRAME_IDLE;
					widget->render = true;

					rzb_helper_transition_callback(frame->button_released, rzb, widget);
				}

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

// frame

struct rzb_widget*
	rzb_alloc_widget_frame(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* title,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_dragged)(struct rzb*, struct rzb_widget*),
		void* button_data)
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
		.button_on_area = button_on_area,
		.button_off_area = button_off_area,
		.button_pressed = button_pressed,
		.button_released = button_released,
		.button_dragged = button_dragged,
		.button_data = button_data,
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

	// fsm
	bool (*update_frame[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_frame_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_frame_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_frame_dragging,
	};

	bool (*update_min[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_min_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_min_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_min_dragging,
	};

	bool (*update_max[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_max_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_max_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_max_dragging,
	};

	bool (*update_close[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_close_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_close_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_close_dragging,
	};

	rzb_fsm_button_init(
		&(data->fsm_frame_button),
		widget,
		update_frame);

	rzb_fsm_button_init(
		&(data->fsm_min_button),
		widget,
		update_min);

	rzb_fsm_button_init(
		&(data->fsm_max_button),
		widget,
		update_max);

	rzb_fsm_button_init(
		&(data->fsm_close_button),
		widget,
		update_close);

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
	int button_hover = 0;
	enum rzb_fsm_button_state state;

	switch (data->status)
	{
		case RZB_WIDGET_FRAME_MINIMIZE:
		{
			state = rzb_fsm_button_get_state(&(data->fsm_min_button));

			if (state != RZB_FSM_BUTTON_STATE_IDLING)
			{
				button_hover = 3;
			}

			break;
		}
		case RZB_WIDGET_FRAME_MAXIMIZE:
		{
			state = rzb_fsm_button_get_state(&(data->fsm_max_button));

			if (state != RZB_FSM_BUTTON_STATE_IDLING)
			{
				button_hover = 2;
			}

			break;
		}
		case RZB_WIDGET_FRAME_CLOSE:
		{
			state = rzb_fsm_button_get_state(&(data->fsm_close_button));

			if (state != RZB_FSM_BUTTON_STATE_IDLING)
			{
				button_hover = 1;
			}

			break;
		}
		default:
		{
			button_hover = 0;
			break;
		}
	}

	data->frame_title_size = context->sizes_current->frame_default_height;
	data->frame_border_size = context->sizes_current->frame_border_size;

	data->button_min_x = widget->x + widget->width - frame_border - (3 * frame_button);
	data->button_min_y = widget->y + frame_border;
	data->button_min_width = frame_button;
	data->button_min_height = frame_height - (2 * frame_border);

	data->button_max_x = widget->x + widget->width - frame_border - (2 * frame_button);
	data->button_max_y = widget->y + frame_border;
	data->button_max_width = frame_button;
	data->button_max_height = frame_height - (2 * frame_border);

	data->button_close_x = widget->x + widget->width - frame_border - (1 * frame_button);
	data->button_close_y = widget->y + frame_border;
	data->button_close_width = frame_button;
	data->button_close_height = frame_height - (2 * frame_border);

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

	// if there is a fsm is not idling, only try its handler

	enum rzb_fsm_button_state state;

	state = rzb_fsm_button_get_state(&(data->fsm_min_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_min_button),
			event_code,
			event_state);
	}

	state = rzb_fsm_button_get_state(&(data->fsm_max_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_max_button),
			event_code,
			event_state);
	}

	state = rzb_fsm_button_get_state(&(data->fsm_close_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_close_button),
			event_code,
			event_state);
	}

	state = rzb_fsm_button_get_state(&(data->fsm_frame_button));

	if (state == RZB_FSM_BUTTON_STATE_DRAGGING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_frame_button),
			event_code,
			event_state);
	}

	// otherwise try all fsm handlers

	bool handled;

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_min_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_max_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_close_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_frame_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	// return false if no fsm handler accepted the event

	return handled;
}
