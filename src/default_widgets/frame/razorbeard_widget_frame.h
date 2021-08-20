#ifndef H_RAZORBEARD_WIDGET_FRAME
#define H_RAZORBEARD_WIDGET_FRAME

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// frame

enum rzb_widget_frame_status
{
	RZB_WIDGET_FRAME_IDLE = 0,

	RZB_WIDGET_FRAME_MOVE,
	RZB_WIDGET_FRAME_SIZE_E,
	RZB_WIDGET_FRAME_SIZE_NE,
	RZB_WIDGET_FRAME_SIZE_N,
	RZB_WIDGET_FRAME_SIZE_NW,
	RZB_WIDGET_FRAME_SIZE_W,
	RZB_WIDGET_FRAME_SIZE_SW,
	RZB_WIDGET_FRAME_SIZE_S,
	RZB_WIDGET_FRAME_SIZE_SE,

	RZB_WIDGET_FRAME_HOVER_CLOSE,
	RZB_WIDGET_FRAME_HOVER_MAX,
	RZB_WIDGET_FRAME_HOVER_MIN,

	RZB_WIDGET_FRAME_PRESS_CLOSE,
	RZB_WIDGET_FRAME_PRESS_MAX,
	RZB_WIDGET_FRAME_PRESS_MIN,
};

enum rzb_widget_frame_action
{
	RZB_WIDGET_FRAME_MINIMIZE = 0,
	RZB_WIDGET_FRAME_MAXIMIZE,
	RZB_WIDGET_FRAME_CLOSE,
};

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
		void* callback_action_data);

void rzb_free_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_frame(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_frame_size_move(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_frame_close(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_frame_maximize(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_frame_minimize(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_frame
{
	struct rzb_default_widgets_context* context;

	char* title;
	void (*callback_interactive)(void*, enum rzb_widget_frame_status);
	void (*callback_interactive_hover)(void*, enum rzb_widget_frame_status);
	void (*callback_frame_action)(void*, enum rzb_widget_frame_action);
	void* callback_data;
	void* callback_hover_data;
	void* callback_action_data;

	enum rzb_widget_frame_status status;
};

#endif
