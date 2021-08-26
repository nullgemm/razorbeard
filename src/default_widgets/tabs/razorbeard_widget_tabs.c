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

// TODO setters: keyboard shortcuts must implemented by the dev for this widget

// fsm updates

bool update_tab_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			if (tabs->tabs_total_visible == 0)
			{
				break;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->tabs_pos_x,
					tabs->tabs_pos_y,
					tabs->tabs_size_x * tabs->tabs_total_visible,
					tabs->tabs_size_y);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_tab_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				int x = context->events_data.mouse_pos_x - tabs->tabs_pos_x;
				tabs->tabs_hovered = x / tabs->tabs_size_x;

				rzb_helper_transition_callback(tabs->tab_on_area, rzb, widget);

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

bool update_tab_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			if (tabs->tabs_total_visible == 0)
			{
				break;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->tabs_pos_x,
					tabs->tabs_pos_y,
					tabs->tabs_size_x * tabs->tabs_total_visible,
					tabs->tabs_size_y);

			if (hit == true)
			{
				int x = context->events_data.mouse_pos_x - tabs->tabs_pos_x;
				int id = x / tabs->tabs_size_x;

				if (id != tabs->tabs_hovered)
				{
					tabs->tabs_hovered = id;
					rzb_helper_transition_callback(tabs->tab_on_area, rzb, widget);
					handled = true;
				}
			}
			else
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_tab_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_off_area, rzb, widget);
			}

			break;
		}
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (tabs->tabs_total_visible == 0)
			{
				break;
			}

			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_tab_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				rzb_helper_transition_callback(tabs->tab_pressed, rzb, widget);

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

bool update_tab_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (tabs->tabs_total_visible == 0)
			{
				break;
			}

			if (event_state != RZB_STATE_RELEASE)
			{
				break;
			}

			handled = true;

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->tabs_pos_x,
					tabs->tabs_pos_y,
					tabs->tabs_size_x * tabs->tabs_total_visible,
					tabs->tabs_size_y);

			if (hit == true)
			{
				int x = context->events_data.mouse_pos_x - tabs->tabs_pos_x;
				int id = x / tabs->tabs_size_x;

				rzb_fsm_button_set_state(
					&(tabs->fsm_tab_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				if (id != tabs->tabs_hovered)
				{
					rzb_helper_transition_callback(tabs->tab_released_off_area, rzb, widget);

					tabs->tabs_hovered = id;

					rzb_helper_transition_callback(tabs->tab_on_area, rzb, widget);
				}
				else
				{
					tabs->tabs_active = tabs->tabs_hovered;

					rzb_helper_transition_callback(tabs->tab_released_on_area, rzb, widget);
				}
			}
			else
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_tab_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_released_off_area, rzb, widget);
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

bool update_prev_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_prev_pos_x,
					tabs->nav_prev_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_prev_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				if (tabs->tabs_total_visible == 0)
				{
					tabs->tabs_hovered = tabs->tabs_active - 1;

					if (tabs->tabs_hovered < 0)
					{
						tabs->tabs_hovered = tabs->tabs_count - 1;
					}

					rzb_helper_transition_callback(tabs->tab_on_area, rzb, widget);
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

bool update_prev_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_prev_pos_x,
					tabs->nav_prev_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_prev_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_off_area, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_prev_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				if (tabs->tabs_total_visible == 0)
				{
					rzb_helper_transition_callback(tabs->tab_pressed, rzb, widget);
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

bool update_prev_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state != RZB_STATE_RELEASE)
			{
				break;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_prev_pos_x,
					tabs->nav_prev_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_prev_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				if (tabs->tabs_total_visible == 0)
				{
					tabs->tabs_active = tabs->tabs_hovered;
					tabs->tabs_hovered = tabs->tabs_active - 1;

					if (tabs->tabs_hovered < 0)
					{
						tabs->tabs_hovered = tabs->tabs_count - 1;
					}
				}
				else if (tabs->tabs_first_visible > 0)
				{
					--tabs->tabs_first_visible;
				}

				rzb_helper_transition_callback(tabs->tab_released_on_area, rzb, widget);
			}
			else
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_prev_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_released_off_area, rzb, widget);
			}

			handled = true;

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

bool update_next_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_next_pos_x,
					tabs->nav_next_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_next_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				if (tabs->tabs_total_visible == 0)
				{
					tabs->tabs_hovered = tabs->tabs_active + 1;

					if (tabs->tabs_hovered >= tabs->tabs_count)
					{
						tabs->tabs_hovered = 0;
					}

					rzb_helper_transition_callback(tabs->tab_on_area, rzb, widget);
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

bool update_next_hovering(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_next_pos_x,
					tabs->nav_next_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == false)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_next_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_off_area, rzb, widget);

				handled = true;
			}

			break;
		}
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_next_button),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				if (tabs->tabs_total_visible == 0)
				{
					rzb_helper_transition_callback(tabs->tab_pressed, rzb, widget);
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

bool update_next_dragging(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_tabs* tabs = widget->data_widget;
	struct rzb_default_widgets_context* context = tabs->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state != RZB_STATE_RELEASE)
			{
				break;
			}

			bool hit =
				rzb_helper_event_mouse_in_rect(
					context,
					tabs->nav_next_pos_x,
					tabs->nav_next_pos_y,
					tabs->nav_size_x,
					tabs->nav_size_y);

			if (hit == true)
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_next_button),
					RZB_FSM_BUTTON_STATE_HOVERING);

				if (tabs->tabs_total_visible == 0)
				{
					tabs->tabs_active = tabs->tabs_hovered;
					tabs->tabs_hovered = tabs->tabs_active + 1;

					if (tabs->tabs_hovered >= tabs->tabs_count)
					{
						tabs->tabs_hovered = 0;
					}
				}
				else if ((tabs->tabs_count - tabs->tabs_first_visible) > tabs->tabs_total_visible)
				{
					++tabs->tabs_first_visible;
				}

				rzb_helper_transition_callback(tabs->tab_released_on_area, rzb, widget);
			}
			else
			{
				rzb_fsm_button_set_state(
					&(tabs->fsm_next_button),
					RZB_FSM_BUTTON_STATE_IDLING);

				rzb_helper_transition_callback(tabs->tab_released_off_area, rzb, widget);
			}

			handled = true;

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

// tabs

struct rzb_widget*
	rzb_alloc_widget_tabs(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*tab_on_area)(struct rzb*, struct rzb_widget*),
		void (*tab_off_area)(struct rzb*, struct rzb_widget*),
		void (*tab_pressed)(struct rzb*, struct rzb_widget*),
		void (*tab_released_on_area)(struct rzb*, struct rzb_widget*),
		void (*tab_released_off_area)(struct rzb*, struct rzb_widget*),
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
		.tab_on_area = tab_on_area,
		.tab_off_area = tab_off_area,
		.tab_pressed = tab_pressed,
		.tab_released_on_area = tab_released_on_area,
		.tab_released_off_area = tab_released_off_area,
		.tabs_names = tabs_names,
		.tabs_count = tabs_count,
		.tabs_hovered = 0,
		.tabs_active = tab_active,
		.tabs_first_visible = 0,
		.tabs_total_visible = tabs_count,
		.tabs_size_x = 0,
		.tabs_size_y = 0,
		.tabs_pos_x = 0,
		.tabs_pos_y = 0,
		.nav_size_x = 0,
		.nav_size_y = 0,
		.nav_prev_pos_x = 0,
		.nav_prev_pos_y = 0,
		.nav_next_pos_x = 0,
		.nav_next_pos_y = 0,
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

	// prepare the fsms
	bool (*update_tab[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_tab_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_tab_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_tab_dragging,
	};

	bool (*update_prev[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_prev_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_prev_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_prev_dragging,
	};

	bool (*update_next[RZB_FSM_BUTTON_STATE_COUNT])(
		struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_next_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_next_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_next_dragging,
	};

	rzb_fsm_button_init(
		&(data->fsm_tab_button),
		widget,
		update_tab);

	rzb_fsm_button_init(
		&(data->fsm_prev_button),
		widget,
		update_prev);

	rzb_fsm_button_init(
		&(data->fsm_next_button),
		widget,
		update_next);

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

	data->tabs_total_visible = 0;
	data->tabs_size_x = 0;
	data->tabs_size_y = 0;
	data->tabs_pos_x = 0;
	data->tabs_pos_y = 0;
	data->nav_size_x = 0;
	data->nav_size_y = 0;
	data->nav_prev_pos_x = 0;
	data->nav_prev_pos_y = 0;
	data->nav_next_pos_x = 0;
	data->nav_next_pos_y = 0;

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

	data->tabs_total_visible =
		widget->width / context->sizes_current->tab_default_width;

	data->tabs_size_y =
		context->sizes_current->tab_default_height;

	data->tabs_pos_y =
		widget->y;

	// get the maximum number of tabs we can display
	if (data->tabs_total_visible >= data->tabs_count)
	{
		data->tabs_total_visible = data->tabs_count;
		data->tabs_pos_x = widget->x;
		data->tabs_first_visible = 0;
	}
	else if (data->tabs_total_visible < data->tabs_count)
	{
		data->tabs_total_visible =
			(widget->width - (2 * context->sizes_current->tab_default_height))
			/ context->sizes_current->tab_default_width;

		data->tabs_pos_x =
			widget->x
			+ context->sizes_current->tab_default_height;

		if ((data->tabs_first_visible + data->tabs_total_visible) > data->tabs_count)
		{
			data->tabs_first_visible = data->tabs_count - data->tabs_total_visible;
		}

		// render the buttons
		if (data->tabs_total_visible == 0)
		{
			data->nav_size_x =
				(widget->width - 3 * context->sizes_current->padding_tab_button) / 2;
			data->nav_size_y = context->sizes_current->tab_default_height
				- 2 * context->sizes_current->padding_tab_button;

			data->nav_prev_pos_x = widget->x
				+ context->sizes_current->padding_tab_button;
			data->nav_prev_pos_y = widget->y
				+ context->sizes_current->padding_tab_button;

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				data->nav_prev_pos_x,
				data->nav_prev_pos_y,
				data->nav_size_x,
				data->nav_size_y,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			data->nav_next_pos_x = widget->x
				+ (widget->width + context->sizes_current->padding_tab_button) / 2;
			data->nav_next_pos_y = widget->y
				+ context->sizes_current->padding_tab_button;

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				data->nav_next_pos_x,
				data->nav_next_pos_y,
				data->nav_size_x,
				data->nav_size_y,
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
			data->nav_size_x = context->sizes_current->tab_default_height
				- 2 * context->sizes_current->padding_tab_button;
			data->nav_size_y = context->sizes_current->tab_default_height
				- 2 * context->sizes_current->padding_tab_button;

			data->nav_prev_pos_x = widget->x
				+ context->sizes_current->padding_tab_button;
			data->nav_prev_pos_y = widget->y
				+ context->sizes_current->padding_tab_button;

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				data->nav_prev_pos_x,
				data->nav_prev_pos_y,
				data->nav_size_x,
				data->nav_size_y,
				context->sizes_current->radius_edge_border,
				false,
				context->color_background);

			data->nav_next_pos_x = widget->x
				+ widget->width
				- context->sizes_current->tab_default_height
				+ context->sizes_current->padding_tab_button;
			data->nav_next_pos_y = widget->y
				+ context->sizes_current->padding_tab_button;

			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				data->nav_next_pos_x,
				data->nav_next_pos_y,
				data->nav_size_x,
				data->nav_size_y,
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

	if (data->tabs_total_visible > 0)
	{
		data->tabs_size_x = context->sizes_current->tab_default_width;

		// render the active tab background
		int tabs_active_rank = data->tabs_active - data->tabs_first_visible;

		if ((tabs_active_rank >= 0) && (tabs_active_rank < data->tabs_total_visible))
		{
			rzb_helper_render_rounded_rectangle(
				rzb->argb,
				rzb->argb_width,
				cropping,
				data->tabs_pos_x + (tabs_active_rank * context->sizes_current->tab_default_width),
				widget->y,
				context->sizes_current->tab_default_width,
				context->sizes_current->tab_default_height,
				context->sizes_current->radius_edge_border,
				true,
				context->color_background);
		}

		// render the tabs text and separators
		for (int i = 0; i < data->tabs_total_visible; ++i)
		{
			// render separators
			if ((i != (data->tabs_active - data->tabs_first_visible))
				&& ((i + 1) != (data->tabs_active - data->tabs_first_visible))
				&& ((i + 1) < data->tabs_total_visible))
			{
				pos_x = data->tabs_pos_x + ((i + 1) * context->sizes_current->tab_default_width) - (context->sizes_current->size_tab_separator / 2);
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
	struct rzb_widget_tabs* data = widget->data_widget;

	// if there is a fsm is not idling, only try its handler

	enum rzb_fsm_button_state state;

	state = rzb_fsm_button_get_state(&(data->fsm_tab_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_tab_button),
			event_code,
			event_state);
	}

	state = rzb_fsm_button_get_state(&(data->fsm_next_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_next_button),
			event_code,
			event_state);
	}

	state = rzb_fsm_button_get_state(&(data->fsm_prev_button));

	if (state != RZB_FSM_BUTTON_STATE_IDLING)
	{
		return rzb_fsm_button_update(
			rzb,
			&(data->fsm_prev_button),
			event_code,
			event_state);
	}

	// otherwise try all fsm handlers

	bool handled;

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_tab_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_next_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	handled =
		rzb_fsm_button_update(
			rzb,
			&(data->fsm_prev_button),
			event_code,
			event_state);

	if (handled == true)
	{
		return handled;
	}

	// return false if no fsm handler accepted the event

	return handled;
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
