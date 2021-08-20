#define _XOPEN_SOURCE 700

#include "razorbeard.h"
#include "razorbeard_helpers.h"
#include "razorbeard_math.h"
#include "razorbeard_default_widgets.h"
#include "razorbeard_widget_dropmenu.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

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
