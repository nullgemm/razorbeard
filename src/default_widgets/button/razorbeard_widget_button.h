#ifndef H_RAZORBEARD_WIDGET_BUTTON
#define H_RAZORBEARD_WIDGET_BUTTON

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"
#include "razorbeard_helpers.h"

// button

struct rzb_widget*
	rzb_alloc_widget_button(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_released_off_area)(struct rzb*, struct rzb_widget*),
		bool toggle,
		char* text);

void rzb_free_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_button_click(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_button_release(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_button
{
	struct rzb_default_widgets_context* context;

	void (*button_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_off_area)(struct rzb*, struct rzb_widget*);
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_released_off_area)(struct rzb*, struct rzb_widget*);

	struct rzb_fsm_button fsm_button;
	bool pushed; // wether the button is being pushed
	bool toggle; // wether the button is a switch
	bool active; // wether the switch is active
	char* text;
};

#endif
