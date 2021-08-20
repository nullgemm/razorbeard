#ifndef H_RAZORBEARD_WIDGET_TABS
#define H_RAZORBEARD_WIDGET_TABS

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

// tabs

struct rzb_widget*
	rzb_alloc_widget_tabs(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*tab_switched)(struct rzb*, struct rzb_widget*),
		char** tabs_names,
		int tabs_count,
		int tab_active);

void rzb_free_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

bool rzb_event_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget,
	int event_code,
	int event_state);

void rzb_event_widget_tabs_click(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_tabs_release(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_tabs
{
	struct rzb_default_widgets_context* context;

	void (*tab_switched)(struct rzb*, struct rzb_widget*);

	char** tabs_names;
	int tabs_count;
	int tab_active;
	int tab_first_visible;
};

#endif
