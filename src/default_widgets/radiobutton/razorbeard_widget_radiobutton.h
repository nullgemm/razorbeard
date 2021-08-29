#ifndef H_RAZORBEARD_WIDGET_RADIOBUTTON
#define H_RAZORBEARD_WIDGET_RADIOBUTTON

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// radiobutton

struct rzb_widget*
	rzb_alloc_widget_radiobutton(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_released_off_area)(struct rzb*, struct rzb_widget*),
		bool checked);

void rzb_free_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

struct rzb_widget_radiobutton
{
	struct rzb_default_widgets_context* context;

	void (*button_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_off_area)(struct rzb*, struct rzb_widget*);
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_released_off_area)(struct rzb*, struct rzb_widget*);

	struct rzb_fsm_button fsm_radiobutton;

	bool checked;
};

#endif
