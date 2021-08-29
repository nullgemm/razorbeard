#ifndef H_RAZORBEARD_WIDGET_HANDLES
#define H_RAZORBEARD_WIDGET_HANDLES

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

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
		int sections_count);

void rzb_free_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

struct rzb_widget_handles
{
	struct rzb_default_widgets_context* context;

	void (*button_on_area)(struct rzb*, struct rzb_widget*);
	void (*button_off_area)(struct rzb*, struct rzb_widget*);
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released)(struct rzb*, struct rzb_widget*);
	void (*button_dragged)(struct rzb*, struct rzb_widget*);
	void* handles_data;

	struct rzb_fsm_button fsm_handles;
	bool horizontal;
	int sections_count;
	int* section_lengths;
	int* section_dragging;
	int selection_pos;
	int selection_original_size;
	int selection_original_size_next;
};

#endif
