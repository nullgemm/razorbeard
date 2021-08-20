#ifndef H_RAZORBEARD_WIDGET_TEXTBOX
#define H_RAZORBEARD_WIDGET_TEXTBOX

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// textbox

struct rzb_widget*
	rzb_alloc_widget_textbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*text_changed)(struct rzb*, struct rzb_widget*),
		char* text);

void rzb_free_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_textbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_textbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_textbox
{
	struct rzb_default_widgets_context* context;

	void (*text_changed)(struct rzb*, struct rzb_widget*);

	char* cursor_beg;
	char* cursor_end;
	char* text;
};

#endif
