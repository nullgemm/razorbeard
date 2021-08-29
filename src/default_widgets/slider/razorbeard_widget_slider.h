#ifndef H_RAZORBEARD_WIDGET_SLIDER
#define H_RAZORBEARD_WIDGET_SLIDER

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_on_area)(struct rzb*, struct rzb_widget*),
		void (*button_off_area)(struct rzb*, struct rzb_widget*),
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_dragged)(struct rzb*, struct rzb_widget*),
		void* handles_data,
		bool vertical,
		uint32_t progress);

void rzb_free_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

struct rzb_widget_slider
{
	struct rzb_default_widgets_context* context;

	void (*button_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_off_area)(struct rzb*, struct rzb_widget*);
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released)(struct rzb*, struct rzb_widget*);
	void (*button_dragged)(struct rzb*, struct rzb_widget*);
	void* slider_data;

	struct rzb_fsm_button fsm_slider;

	bool vertical;
	int progress;
	int selection_offset;
};

#endif
