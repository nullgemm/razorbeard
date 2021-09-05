#include "razorbeard.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

bool rzb_init(
	struct rzb* rzb,
	struct rzb_display_info* display_info)
{
	if ((display_info == NULL) || (rzb == NULL))
	{
		return false;
	}

	// initialized by widget additions
	rzb->root_widget = NULL;
	rzb->events_grabber = NULL;

	// the only thing we actually initialize
	rzb->display_info = display_info;

	// send events
	rzb->callback_send_events_data_context = NULL;
	rzb->callback_send_events_data = NULL;

	// initialized by the user using setters
	rzb->argb = NULL;
	rzb->argb_width = 0;
	rzb->argb_height = 0;

	return true;
}

bool rzb_free(
	struct rzb* rzb)
{
	// TODO free widget hierarchy

	// TODO always alloc'd ? clarify this...
	free(rzb->display_info);
	free(rzb);

	return true;
}

void rzb_send_event(
	struct rzb* rzb,
	int event_code,
	int event_state)
{
	struct rzb_widget* tmp = rzb->events_grabber;
	struct rzb_widget* old;

	if ((tmp != NULL)
		&& (tmp->hide == false)
		&& (tmp->callback_events != NULL))
	{
		if (tmp->callback_events(rzb, tmp, event_code, event_state) == true)
		{
			return;
		}
	}

	tmp = rzb->root_widget;

	do
	{
		do
		{
			if ((tmp->hide == false) && (tmp->callback_events != NULL))
			{
				if (tmp->callback_events(rzb, tmp, event_code, event_state) == true)
				{
					return;
				}
			}

			old = tmp;
			tmp = tmp->children;
		}
		while (tmp != NULL);

		do
		{
			if (old == NULL)
			{
				return;
			}

			tmp = old->siblings;
			old = old->parent;
		}
		while (tmp == NULL);
	}
	while (true);
}

void rzb_send_events_data(
	struct rzb* rzb,
	void* data)
{
	rzb->callback_send_events_data(
		rzb->callback_send_events_data_context,
		data);
}

void rzb_set_events_data_callback(
	struct rzb* rzb,
	void (*callback)(void*, void*),
	void* context)
{
	rzb->callback_send_events_data = callback;
	rzb->callback_send_events_data_context = context;
}

void rzb_grab_events(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

bool rzb_render(
	struct rzb* rzb)
{
	struct rzb_widget* tmp = rzb->root_widget;
	struct rzb_widget* lim = NULL;
	struct rzb_widget* old;

	struct rzb_cropping crop;

	int stack_max = 0;
	int stack_size = 0;
	struct rzb_widget** stack = NULL;


	// infinite loop - yes this is the best way...
	do
	{
		// process all first children, updating their
		// layout and rendering when necessary
		do
		{
			// save the current widget stream from root in a stack
			if (stack_size == stack_max)
			{
				stack_max += 10;
				stack = realloc(stack, stack_max * (sizeof (struct rzb_widget*)));

				if (stack == NULL)
				{
					return false;
				}
			}

			stack[stack_size] = tmp;
			++stack_size;

			// update the layout of the current widget
			old = tmp;
			tmp->callback_layout(rzb, tmp);

			// save the widget that triggered this render stream
			if ((tmp->render == true) && (lim == NULL))
			{
				crop.x = tmp->x;
				crop.y = tmp->y;
				crop.width = tmp->width;
				crop.height = tmp->height;

				for (int i = 0; i < stack_size - 1; ++i)
				{
					stack[i]->callback_render(rzb, stack[i], &crop);
					stack[i]->render = false;
				}

				if (tmp->hide == false)
				{
					lim = tmp;
				}
			}

			// render the current widget if in the rest of the render stream
			if ((tmp->hide == false) && (lim != NULL))
			{
				tmp->callback_render(rzb, tmp, &crop);
				tmp->render = false;
			}

			// repeat on child widget
			tmp = tmp->children;
		}
		while (tmp != NULL);

		// select the closest sibling
		do
		{
			// if we reach the root then everything has been processed
			if (old == NULL)
			{
				free(stack);

				return true;
			}

			// if we reach the widget that initialized the render stream
			// it is time to reset and wait for another explicit render
			if (old == lim)
			{
				lim = NULL;
			}

			tmp = old->siblings;
			old = old->parent;

			--stack_size;
		}
		while (tmp == NULL);
	}
	while (true);
}

void rzb_render_all(
	struct rzb* rzb)
{
	struct rzb_widget* tmp = rzb->root_widget;
	struct rzb_widget* old;

	struct rzb_cropping crop =
	{
		.x = 0,
		.y = 0,
		.width = rzb->argb_width,
		.height = rzb->argb_height,
	};

	// infinite loop - yes this is the best way...
	do
	{
		// process all first children, updating their
		// layout and rendering when necessary
		do
		{
			// update the layout of the current widget
			old = tmp;
			tmp->callback_layout(rzb, tmp);

			// save the widget that triggered this render stream
			if (tmp->hide == false)
			{
				tmp->callback_render(rzb, tmp, &crop);
				tmp->render = false;
			}

			// repeat on child widget
			tmp = tmp->children;
		}
		while (tmp != NULL);

		// select the closest sibling
		do
		{
			// if we reach the root then everything has been processed
			if (old == NULL)
			{
				return;
			}

			tmp = old->siblings;
			old = old->parent;
		}
		while (tmp == NULL);
	}
	while (true);
}

bool rzb_update_root_widget(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	if (rzb->root_widget != NULL)
	{
		if (widget->children_limit < rzb->root_widget->children_count)
		{
			return false;
		}

		widget->siblings = rzb->root_widget->siblings;
		widget->children = rzb->root_widget->children;
		widget->children_count = rzb->root_widget->children_count;

		rzb->root_widget->siblings = NULL;
		rzb->root_widget->children = NULL;
		rzb->root_widget->children_count = 0;
	}

	widget->parent = NULL;
	rzb->root_widget = widget;

	return true;
}

bool rzb_update_display_info(
	struct rzb* rzb,
	struct rzb_display_info* display_info)
{
	if ((rzb->display_info->px_width != display_info->px_width)
		|| (rzb->display_info->px_height != display_info->px_height)
		|| (rzb->display_info->mm_width != display_info->mm_width)
		|| (rzb->display_info->mm_height != display_info->mm_height)
		|| (rzb->display_info->dpi_logic != display_info->dpi_logic)
		|| (rzb->display_info->scale != display_info->scale))
	{
		rzb->display_info->px_width = display_info->px_width;
		rzb->display_info->px_height = display_info->px_height;
		rzb->display_info->mm_width = display_info->mm_width;
		rzb->display_info->mm_height = display_info->mm_height;
		rzb->display_info->dpi_logic = display_info->dpi_logic;
		rzb->display_info->scale = display_info->scale;
		rzb->root_widget->render = true;
		return true;
	}

	return false;
}

bool rzb_make_child(
	struct rzb_widget* widget,
	struct rzb_widget* parent)
{
	widget->parent = parent;
	widget->siblings = parent->children;
	parent->children = widget;
	parent->children_count += 1;

	return true;
}

bool rzb_make_sibling(
	struct rzb_widget* widget,
	struct rzb_widget* sibling)
{
	widget->siblings = sibling->siblings;
	sibling->siblings = widget;

	widget->parent = sibling->parent;

	if (widget->parent != NULL)
	{
		widget->parent->children_count += 1;
	}

	return true;
}

bool rzb_make_detached(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget* parent = widget->parent;

	// reset event grabber if needed
	if (widget == rzb->events_grabber)
	{
		rzb->events_grabber = NULL;
	}

	// if this is a root-level widget, detach carefully
	if (parent == NULL)
	{
		// the widget is the root
		if (rzb->root_widget == widget)
		{
			rzb->root_widget = widget->siblings;
			widget->siblings = NULL;

			return true;
		}

		// the widget is a root sibling
		struct rzb_widget* child = rzb->root_widget;

		while (child->siblings != widget)
		{
			child = child->siblings;
		}

		child->siblings = widget->siblings;
		widget->siblings = NULL;
	}

	// if the first child is our widget, detach carefully
	if (parent->children == widget)
	{
		parent->children = widget->siblings;
		parent->children_count -= 1;
		widget->siblings = NULL;
		widget->parent = NULL;

		return true;
	}

	// otherwise use the generic method
	struct rzb_widget* child = parent->children;

	while (child->siblings != widget)
	{
		child = child->siblings;
	}

	child->siblings = widget->siblings;
	parent->children_count -= 1;
	widget->siblings = NULL;
	widget->parent = NULL;

	return true;
}

void rzb_select_widget(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	if (rzb->events_grabber != NULL)
	{
		rzb->events_grabber->render = true;
	}

	if ((widget != NULL) && (widget->hide == false))
	{
		rzb->events_grabber = widget;
		widget->render = true;
		return;
	}

	rzb->events_grabber = NULL;
}

void rzb_widget_set_hide(
	struct rzb_widget* widget,
	bool hide)
{
	if (hide != widget->hide)
	{
		widget->render = true;
	}

	widget->hide = hide;
}

void rzb_widget_layout_update(
	struct rzb_widget* widget,
	int x,
	int y,
	int width,
	int height)
{
	if ((widget->x != x)
		|| (widget->y != y)
		|| (widget->width != width)
		|| (widget->height != height))
	{
		widget->render = true;
	}

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;
}
