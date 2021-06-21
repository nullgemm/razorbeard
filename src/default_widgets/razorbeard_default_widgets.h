#ifndef H_RAZORBEARD_DEFAULT_WIDGETS
#define H_RAZORBEARD_DEFAULT_WIDGETS

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

struct rzb_default_widgets_sizes
{
	// widget shadow thickness
	int size_shadow;
	// widget border thickness
	int size_edge_border;
	// selection border thickness
	int size_selected_border;
	// all text size
	int size_font;

	// fine top shine border thickness
	int size_shine_edge;
	// botton shine gradient size
	int size_shine_gradient;
	// corner radius
	int radius_edge_border;

	// thickness of the slider bars (with padding and edge border)
	int size_slider;
	// thickness of the separators
	int size_separator;
	// size of the resizing corner of the text areas
	int size_textarea_corner;
	// size of the flat part of the arrow
	int size_arrow_base;

	// thickness of the textbox scrollbar
	int size_textbox_scrollbar;
	// padding around the textbox scrollbar
	int padding_textbox_scrollbar;

	// thickness of the tab separator
	int size_tab_separator;
	// space between the tips of the separator and the limits of the tab area
	int padding_tab_separator;
	// space between the tab navigation button and the tab bar
	int padding_tab_button;
	// padding around the separator
	int padding_separator;

	// thickness of the handles
	int size_handle;
	// radius of the handle circles
	int radius_handle;
	// padding between the handle circles
	int padding_handle;

	// default size of the tabs
	int tab_default_width;
	int tab_default_height;

	// padding between the edge border and the content for some widgets
	int padding_bar;
	int padding_textbox;
	int padding_checkbox;
	int padding_radiobutton;
};

struct rzb_default_widgets_context
{
	// events

	int* events_table;
	char** events_data_typed_string;
	int events_data_mouse_pos_x;
	int events_data_mouse_pos_y;

	// sizes

	struct rzb_default_widgets_sizes* sizes_current;
	struct rzb_default_widgets_sizes sizes_density_low;
	struct rzb_default_widgets_sizes sizes_density_medium;
	struct rzb_default_widgets_sizes sizes_density_high;

	// colors

	// widget shadow
	uint32_t color_shadow;
	// widget border
	uint32_t color_edge;
	// selection border
	uint32_t color_selected;
	// all text color
	uint32_t color_text;

	// regular window background
	uint32_t color_background;
	// button, drop-down menu, popup window
	uint32_t color_foreground;
	// text box and text area color
	uint32_t color_background_box;
	// button light reflection
	uint32_t color_foreground_shine;
	// button light reflection gradient
	uint32_t color_foreground_shine_gradient;
};

bool rzb_default_widgets_init(
	struct rzb_default_widgets_context* context,
	void** events_data,
	int* events_table);

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context);

// handles

struct rzb_widget*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal,
		int sections_count);

void rzb_free_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_handles_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_handles_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_handles
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	int sections_count;
	int* section_lengths;
	int* section_dragging;
};

// pager

struct rzb_widget*
	rzb_alloc_widget_pager(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal);

void rzb_free_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_pager_scroll_up(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_pager_scroll_down(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_pager
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	int size_page;
	int position_page;
};

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

void rzb_event_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget);

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

// popup

struct rzb_widget*
	rzb_alloc_widget_popup(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context);

void rzb_free_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

// dropmenu

struct rzb_widget*
	rzb_alloc_widget_dropmenu(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context);

void rzb_free_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

// separator

struct rzb_widget*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context);

void rzb_free_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

// text

struct rzb_widget*
	rzb_alloc_widget_text(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* text);

void rzb_free_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_text
{
	struct rzb_default_widgets_context* context;

	char* text;
};

// image

struct rzb_widget*
	rzb_alloc_widget_image(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool alpha_premult,
		uint32_t gamma,
		uint32_t* rgba);

void rzb_free_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_image
{
	struct rzb_default_widgets_context* context;

	bool alpha_premult;
	uint32_t gamma; // 16.16 fixed-point
	uint32_t* rgba;
};

// button

struct rzb_widget*
	rzb_alloc_widget_button(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*button_pressed)(struct rzb*, struct rzb_widget*),
		void (*button_released)(struct rzb*, struct rzb_widget*),
		void (*button_activated)(struct rzb*, struct rzb_widget*),
		bool toggle,
		char* text);

void rzb_free_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_button_click(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_button_release(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_button
{
	struct rzb_default_widgets_context* context;

	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released)(struct rzb*, struct rzb_widget*);
	void (*button_activated)(struct rzb*, struct rzb_widget*);

	bool pushed; // wether the button is being pushed
	bool toggle; // wether the button is a switch
	bool active; // wether the switch is active
	char* text;
};

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

void rzb_event_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

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

void rzb_event_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

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

void rzb_event_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget);

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

// radiobutton

struct rzb_widget*
	rzb_alloc_widget_radiobutton(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*radiobutton_changed)(struct rzb*, struct rzb_widget*),
		bool checked);

void rzb_free_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_radiobutton_click(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_radiobutton_release(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_radiobutton
{
	struct rzb_default_widgets_context* context;

	void (*radiobutton_changed)(struct rzb*, struct rzb_widget*);

	bool checked;
};

// checkbox

struct rzb_widget*
	rzb_alloc_widget_checkbox(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*checkbox_changed)(struct rzb*, struct rzb_widget*),
		bool checked);

void rzb_free_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_checkbox_click(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_checkbox_release(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_checkbox
{
	struct rzb_default_widgets_context* context;

	void (*checkbox_changed)(struct rzb*, struct rzb_widget*);

	bool checked;
};

// scrollbar

struct rzb_widget*
	rzb_alloc_widget_scrollbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*scrollbar_moved)(struct rzb*, struct rzb_widget*));

void rzb_free_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_scrollbar_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_scrollbar_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_scrollbar
{
	struct rzb_default_widgets_context* context;

	void (*scrollbar_moved)(struct rzb*, struct rzb_widget*);

	bool pushed;
	int size_scrollbar;
	int position_scrollbar;
};

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*slider_moved)(struct rzb*, struct rzb_widget*),
		bool vertical,
		uint32_t progress);

void rzb_free_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

void rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_slider_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_event_widget_slider_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget);

struct rzb_widget_slider
{
	struct rzb_default_widgets_context* context;

	void (*slider_moved)(struct rzb*, struct rzb_widget*);

	bool pushed;
	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

// progressbar

struct rzb_widget*
	rzb_alloc_widget_progressbar(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool vertical,
		uint32_t progress);

void rzb_free_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping);

struct rzb_widget_progressbar
{
	struct rzb_default_widgets_context* context;

	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

#endif
