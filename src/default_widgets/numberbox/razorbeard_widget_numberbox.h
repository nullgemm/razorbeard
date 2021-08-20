#ifndef H_RAZORBEARD_WIDGET_NUMBERBOX
#define H_RAZORBEARD_WIDGET_NUMBERBOX

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// numberbox

struct rzb_widget*
	rzb_alloc_widget_numberbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*number_changed)(struct rzb*, struct rzb_widget*),
		char* value);

void rzb_free_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_numberbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_numberbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_numberbox_increase(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_numberbox_decrease(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_numberbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_numberbox
{
	struct rzb_default_widgets_context* context;

	void (*number_changed)(struct rzb*, struct rzb_widget*);

	bool pushed_up;
	bool pushed_down;
	char* value;
};

#endif
