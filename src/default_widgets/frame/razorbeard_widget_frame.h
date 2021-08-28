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

	RZB_WIDGET_FRAME_MINIMIZE,
	RZB_WIDGET_FRAME_MAXIMIZE,
	RZB_WIDGET_FRAME_CLOSE,
};

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
		void* button_data);

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
	void (*button_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_off_area)(struct rzb*, struct rzb_widget*);
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released)(struct rzb*, struct rzb_widget*);
	void (*button_dragged)(struct rzb*, struct rzb_widget*);
	void* button_data;

	struct rzb_fsm_button fsm_min_button;
	struct rzb_fsm_button fsm_max_button;
	struct rzb_fsm_button fsm_close_button;
	struct rzb_fsm_button fsm_frame_button;

	int frame_title_size;
	int frame_border_size;
	int button_min_x;
	int button_min_y;
	int button_min_width;
	int button_min_height;
	int button_max_x;
	int button_max_y;
	int button_max_width;
	int button_max_height;
	int button_close_x;
	int button_close_y;
	int button_close_width;
	int button_close_height;
	enum rzb_widget_frame_status status;
};

#endif
