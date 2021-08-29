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

// fsm update callbacks

static bool update_idling(struct rzb* rzb, void* data, int event_code, int event_state)
{
	bool handled = false;
	struct rzb_widget* widget = data;
	struct rzb_widget_handles* handles = widget->data_widget;
	struct rzb_default_widgets_context* context = handles->context;

	switch (event_code)
	{
		case RZB_MOUSE_MOTION:
		{
			int x;
			int y; 
			int width;
			int height;

			if (handles->horizontal == true)
			{
				x = widget->x - context->sizes_current->radius_handle;
				y = widget->y;
				width = 2 * context->sizes_current->radius_handle;
				height = widget->height;
			}
			else
			{
				x = widget->x;
				y = widget->y - context->sizes_current->radius_handle;
				width = widget->width;
				height = 2 * context->sizes_current->radius_handle;
			}

			bool hit;
			int i = 0;

			while (i < handles->sections_count - 1)
			{
				if (handles->horizontal == true)
				{
					x += handles->section_lengths[i];
				}
				else
				{
					y += handles->section_lengths[i];
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						x,
						y,
						width,
						height);

				if (hit == true)
				{
					rzb_fsm_button_set_state(
						&(handles->fsm_handles),
						RZB_FSM_BUTTON_STATE_HOVERING);

					handles->section_dragging = &(handles->section_lengths[i]);
					rzb_helper_transition_callback(handles->button_on_area, rzb, widget);

					handled = true;
					break;
				}

				++i;
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
	struct rzb_widget_handles* handles = widget->data_widget;
	struct rzb_default_widgets_context* context = handles->context;

	switch (event_code)
	{
		case RZB_MOUSE_CLICK_LEFT:
		{
			if (event_state == RZB_STATE_PRESS)
			{
				rzb_fsm_button_set_state(
					&(handles->fsm_handles),
					RZB_FSM_BUTTON_STATE_DRAGGING);

				rzb_select_widget(
					rzb,
					widget);

				if (handles->horizontal == true)
				{
					handles->selection_pos = 
						context->events_data.mouse_pos_x;
				}
				else
				{
					handles->selection_pos = 
						context->events_data.mouse_pos_y;
				}

				handles->selection_original_size =
					handles->section_dragging[0];
				handles->selection_original_size_next =
					handles->section_dragging[1];

				rzb_helper_transition_callback(handles->button_pressed, rzb, widget);

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

			if (handles->horizontal == true)
			{
				x = widget->x - context->sizes_current->radius_handle;
				y = widget->y;
				width = 2 * context->sizes_current->radius_handle;
				height = widget->height;
			}
			else
			{
				x = widget->x;
				y = widget->y - context->sizes_current->radius_handle;
				width = widget->width;
				height = 2 * context->sizes_current->radius_handle;
			}

			bool hit;
			int i = 0;

			while (i < handles->sections_count - 1)
			{
				if (handles->horizontal == true)
				{
					x += handles->section_lengths[i];
				}
				else
				{
					y += handles->section_lengths[i];
				}

				hit =
					rzb_helper_event_mouse_in_rect(
						context,
						x,
						y,
						width,
						height);
				
				if (hit == true)
				{
					handles->section_dragging = &(handles->section_lengths[i]);
					handled = true;
					break;
				}

				++i;
			}

			if (handled == false)
			{
				rzb_fsm_button_set_state(
					&(handles->fsm_handles),
					RZB_FSM_BUTTON_STATE_IDLING);

				handles->section_dragging = NULL;
				handled = true;

				rzb_helper_transition_callback(handles->button_off_area, rzb, widget);
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
	struct rzb_widget_handles* handles = widget->data_widget;
	struct rzb_default_widgets_context* context = handles->context;

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

				if (handles->horizontal == true)
				{
					x = widget->x - context->sizes_current->radius_handle;
					y = widget->y;
					width = 2 * context->sizes_current->radius_handle;
					height = widget->height;
				}
				else
				{
					x = widget->x;
					y = widget->y - context->sizes_current->radius_handle;
					width = widget->width;
					height = 2 * context->sizes_current->radius_handle;
				}

				bool hit;
				int i = 0;

				while (i < handles->sections_count - 1)
				{
					if (handles->horizontal == true)
					{
						x += handles->section_lengths[i];
					}
					else
					{
						y += handles->section_lengths[i];
					}

					hit =
						rzb_helper_event_mouse_in_rect(
							context,
							x,
							y,
							width,
							height);
					
					if (hit == true)
					{
						rzb_fsm_button_set_state(
							&(handles->fsm_handles),
							RZB_FSM_BUTTON_STATE_HOVERING);

						handles->section_dragging = &(handles->section_lengths[i]);
						handled = true;

						rzb_helper_transition_callback(handles->button_released, rzb, widget);

						break;
					}

					++i;
				}

				if (handled == false)
				{
					rzb_fsm_button_set_state(
						&(handles->fsm_handles),
						RZB_FSM_BUTTON_STATE_IDLING);

					handles->section_dragging = NULL;
					handled = true;

					rzb_helper_transition_callback(handles->button_released, rzb, widget);
				}

				break;
			}

			break;
		}
		case RZB_MOUSE_MOTION:
		{ 
			int mouse;

			if (handles->horizontal == true)
			{
				mouse = context->events_data.mouse_pos_x;
			}
			else
			{
				mouse = context->events_data.mouse_pos_y;
			}

			handles->section_dragging[0] =
				handles->selection_original_size
				+ (mouse - handles->selection_pos);

			if (handles->section_dragging[0] < 2 * context->sizes_current->radius_handle)
			{
				handles->section_dragging[0] = 2 * context->sizes_current->radius_handle;
			}

			if (handles->section_dragging[0] > (handles->selection_original_size + handles->selection_original_size_next - (2 * context->sizes_current->radius_handle)))
			{
				handles->section_dragging[0] = handles->selection_original_size + handles->selection_original_size_next - (2 * context->sizes_current->radius_handle);
			}

			handles->section_dragging[1] =
				handles->selection_original_size
				+ handles->selection_original_size_next
				- handles->section_dragging[0];

			rzb_helper_transition_callback(handles->button_dragged, rzb, widget);

			widget->render = true;

			break;
		}
		default:
		{
			break;
		}
	}

	return handled;
}

// handles

struct rzb_widget*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_dragged)(struct rzb*, struct rzb_widget*),
		void* handles_data,
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

		.button_on_area = button_on_area,
		.button_off_area = button_off_area,
		.button_pressed = button_pressed,
		.button_released = button_released,
		.button_dragged = button_dragged,
		.handles_data = handles_data,

		.horizontal = horizontal,
		.sections_count = sections_count,
		.section_lengths = malloc(sections_count * sizeof (int)),
		.section_dragging = NULL,
		.selection_pos = 0,
		.selection_original_size = 0,
		.selection_original_size_next = 0,
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

	// prepare the fsm
	bool (*update[RZB_FSM_BUTTON_STATE_COUNT])(struct rzb* rzb, void*, int, int) =
	{
		[RZB_FSM_BUTTON_STATE_IDLING] = update_idling,
		[RZB_FSM_BUTTON_STATE_HOVERING] = update_hovering,
		[RZB_FSM_BUTTON_STATE_DRAGGING] = update_dragging,
	};

	rzb_fsm_button_init(
		&(data->fsm_handles),
		widget,
		update);

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
	struct rzb_widget_handles* data = widget->data_widget;

	return rzb_fsm_button_update(rzb, &(data->fsm_handles), event_code, event_state);
}
