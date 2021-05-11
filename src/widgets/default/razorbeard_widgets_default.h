#ifndef H_RAZORBEARD_WIDGETS_DEFAULT
#define H_RAZORBEARD_WIDGETS_DEFAULT

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

struct rzb_default_widgets_sizes
{
	// widget shadow thickness
	uint32_t size_shadow;
	// widget border thickness
	uint32_t size_edge_border;
	// selection border thickness
	uint32_t size_selected_border;
	// all text size
	uint32_t size_font;

	// thickness of the slider bars (with padding and edge border)
	uint32_t size_slider;
	// thickness of the handles and separators
	uint32_t size_separator;
	// size of the resizing corner of the text areas
	uint32_t size_textarea_corner;
	// default size of the tabs
	uint32_t tab_default_width;
	uint32_t tab_default_height;
	// padding between the edge border and the content for some widgets
	uint32_t padding_bar;
	uint32_t padding_checkbox;
	uint32_t padding_radiobutton;
};

struct rzb_default_widgets
{
	// sizes

	// TODO
	struct rzb_default_widgets_sizes sizes_dpi_XX1;
	struct rzb_default_widgets_sizes sizes_dpi_XX2;
	struct rzb_default_widgets_sizes sizes_dpi_XX3;

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
};

// handles

struct rzb_widget_handles*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_handles* widget,
		bool horizontal,
		uint32_t sections_count);

struct rzb_widget_handles
{
	bool horizontal;
	uint32_t sections_count;
	uint32_t* section_lengths;
	uint32_t* section_dragging;
};

// pager

struct rzb_widget_pager*
	rzb_alloc_widget_pager(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_pager* widget,
		bool horizontal);

struct rzb_widget_pager
{
	bool horizontal;
	uint32_t size_page;
	uint32_t position_page;
};

// tabs

struct rzb_widget_tabs*
	rzb_alloc_widget_tabs(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_tabs* widget,
		struct rzb_widget_tabs_events* events_callbacks,
		char** tabs_names,
		uint32_t tab_active);

struct rzb_widget_tabs
{
	char** tabs_names;
	uint32_t tab_active;
};

struct rzb_widget_tabs_events
{
	void (*tab_switched)(struct rzb*, struct rzb_widget*);
};

// popup

struct rzb_widget_popup*
	rzb_alloc_widget_popup(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_popup* widget);

// dropmenu

struct rzb_widget_dropmenu*
	rzb_alloc_widget_dropmenu(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_dropmenu* widget);

// separator

struct rzb_widget_separator*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_separator* widget);

// text

struct rzb_widget_text*
	rzb_alloc_widget_text(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_text* widget,
		char* text);

struct rzb_widget_text
{
	char* text;
};

// image

struct rzb_widget_image*
	rzb_alloc_widget_image(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_image* widget,
		bool alpha_premult,
		uint32_t gamma,
		uint32_t rgba);

struct rzb_widget_image
{
	bool alpha_premult;
	uint32_t gamma; // 16.16 fixed-point
	uint32_t* rgba;
};

// button

struct rzb_widget_button*
	rzb_alloc_widget_button(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_button* widget,
		struct rzb_widget_button_events* events_callbacks,
		bool toggle,
		char* text);

struct rzb_widget_button
{
	bool pushed; // wether the button is being pushed
	bool toggle; // wether the button is a switch
	bool active; // wether the switch is active
	char* text;
};

struct rzb_widget_button_events
{
	void (*button_pressed)(struct rzb*, struct rzb_widget*);
	void (*button_released)(struct rzb*, struct rzb_widget*);
	void (*button_activated)(struct rzb*, struct rzb_widget*);
};

// numberbox

struct rzb_widget_numberbox*
	rzb_alloc_widget_numberbox(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_numberbox* widget,
		struct rzb_widget_numberbox_events* events_callbacks,
		char* value);

struct rzb_widget_numberbox
{
	bool pushed_up;
	bool pushed_down;
	char* value;
};

struct rzb_widget_numberbox_events
{
	void (*number_changed)(struct rzb*, struct rzb_widget*);
};

// textbox

struct rzb_widget_textbox*
	rzb_alloc_widget_textbox(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_textbox* widget,
		struct rzb_widget_textbox_events* events_callbacks,
		char* text);

struct rzb_widget_textbox
{
	char* cursor_beg;
	char* cursor_end;
	char* text;
};

struct rzb_widget_textbox_events
{
	void (*text_changed)(struct rzb*, struct rzb_widget*);
};

// textarea

struct rzb_widget_textarea*
	rzb_alloc_widget_textarea(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_textarea* widget,
		struct rzb_widget_textarea_events* events_callbacks,
		bool resizable,
		char* text);

struct rzb_widget_textarea
{
	bool resizable;
	bool resizing;
	char* cursor_beg;
	char* cursor_end;
	char* text;
};

struct rzb_widget_textarea_events
{
	void (*text_changed)(struct rzb*, struct rzb_widget*);
};

// radiobutton

struct rzb_widget_radiobutton*
	rzb_alloc_widget_radiobutton(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_radiobutton* widget,
		struct rzb_widget_radiobutton_events* events_callbacks,
		bool checked);

struct rzb_widget_radiobutton
{
	bool checked;
};

struct rzb_widget_radiobutton_events
{
	void (*radiobutton_changed)(struct rzb*, struct rzb_widget*);
};

// checkbox

struct rzb_widget_checkbox*
	rzb_alloc_widget_checkbox(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_checkbox* widget,
		struct rzb_widget_checkbox_events* events_callbacks,
		bool checked);

struct rzb_widget_checkbox
{
	bool checked;
};

struct rzb_widget_checkbox_events
{
	void (*checkbox_changed)(struct rzb*, struct rzb_widget*);
};

// scrollbar

struct rzb_widget_scrollbar*
	rzb_alloc_widget_scrollbar(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_scrollbar* widget,
		struct rzb_widget_scrollbar_events* events_callbacks);

struct rzb_widget_scrollbar
{
	bool pushed;
	uint32_t size_scrollbar;
	uint32_t position_scrollbar;
};

struct rzb_widget_scrollbar_events
{
	void (*scrollbar_moved)(struct rzb*, struct rzb_widget*);
};

// slider

struct rzb_widget_slider*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_slider* widget,
		struct rzb_widget_slider_events* events_callbacks,
		bool vertical,
		uint32_t progress);

struct rzb_widget_slider
{
	bool pushed;
	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

struct rzb_widget_slider_events
{
	void (*slider_moved)(struct rzb*, struct rzb_widget*);
};

// progressbar

struct rzb_widget_progressbar*
	rzb_alloc_widget_progressbar(
		struct rzb* rzb,
		struct rzb_default_widgets* default_widgets,
		struct rzb_widget_progressbar* widget,
		bool vertical,
		uint32_t progress);

struct rzb_widget_progressbar
{
	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};

#endif
