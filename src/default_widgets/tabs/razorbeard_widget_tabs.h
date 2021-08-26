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
		void (*tab_on_area)(struct rzb*, struct rzb_widget*),
		void (*tab_off_area)(struct rzb*, struct rzb_widget*),
		void (*tab_pressed)(struct rzb*, struct rzb_widget*),
		void (*tab_released_on_area)(struct rzb*, struct rzb_widget*),
		void (*tab_released_off_area)(struct rzb*, struct rzb_widget*),
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

	void (*tab_on_area)(struct rzb*, struct rzb_widget*);
	void (*tab_off_area)(struct rzb*, struct rzb_widget*);
	void (*tab_pressed)(struct rzb*, struct rzb_widget*);
	void (*tab_released_on_area)(struct rzb*, struct rzb_widget*);
	void (*tab_released_off_area)(struct rzb*, struct rzb_widget*);

	struct rzb_fsm_button fsm_tab_button;
	struct rzb_fsm_button fsm_next_button;
	struct rzb_fsm_button fsm_prev_button;

	char** tabs_names;
	int tabs_count;
	int tabs_hovered;
	int tabs_active;
	int tabs_first_visible;
	int tabs_total_visible;
	int tabs_size_x;
	int tabs_size_y;
	int tabs_pos_x;
	int tabs_pos_y;
	int nav_size_x;
	int nav_size_y;
	int nav_prev_pos_x;
	int nav_prev_pos_y;
	int nav_next_pos_x;
	int nav_next_pos_y;
};

#endif
