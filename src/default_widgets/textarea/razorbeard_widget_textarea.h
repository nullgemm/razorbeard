#ifndef H_RAZORBEARD_WIDGET_TEXTAREA
#define H_RAZORBEARD_WIDGET_TEXTAREA

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// textarea

struct rzb_widget*
	rzb_alloc_widget_textarea(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*text_changed)(struct rzb*, struct rzb_widget*),
		bool resizable,
		char* text);

void rzb_free_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_textarea_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textarea_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textarea_resize_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textarea_resize_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textarea_text(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_textarea
{
	struct rzb_default_widgets_context* context;

	void (*text_changed)(struct rzb*, struct rzb_widget*);

	bool resizable;
	bool resizing;
	char* cursor_beg;
	char* cursor_end;
	char* text;
};

#endif
