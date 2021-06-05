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

	// initialized by rendering updates
	rzb->render_list = NULL;
	rzb->render_list_size = 0;

	// initialized by rendering updates
	rzb->window_partition = NULL;
	rzb->window_partition_width = 0;
	rzb->window_partition_height = 0;

	// initialized by the user using setters
	rzb->argb = NULL;
	rzb->argb_width = 0;
	rzb->argb_height = 0;

	// the only thing we actually initialize
	rzb->display_info = display_info;

	return true;
}

bool rzb_free(
	struct rzb* rzb)
{
	// TODO free widget hierarchy
	free(rzb->render_list);

	for (uint32_t i = 0; i < rzb->window_partition_width; ++i)
	{
		free(rzb->window_partition[i]);
	}

	free(rzb->window_partition);
	free(rzb->display_info);
	free(rzb);

	// TODO check if this function can fail
	return true;
}

void rzb_send_event(
	struct rzb* rzb,
	int event_id)
{
	// TODO
}

void rzb_grab_events(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_render(
	struct rzb* rzb)
{
	// TODO
}


bool rzb_update_root_widget(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
	return true;
}

bool rzb_update_display_info(
	struct rzb* rzb,
	struct rzb_display_info* display_info)
{
	// TODO
	return true;
}

bool rzb_make_child(
	struct rzb_widget* widget,
	struct rzb_widget* parent)
{
	// TODO
	return true;
}

bool rzb_make_sibling(
	struct rzb_widget* widget,
	struct rzb_widget* sibling)
{
	// TODO
	return true;
}

bool rzb_make_detached(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
	return true;
}


bool rzb_renderlist_add(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
	return true;
}

bool rzb_layout_update(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
	return true;
}
