#ifndef H_RAZORBEARD_DEFAULT_WIDGETS
#define H_RAZORBEARD_DEFAULT_WIDGETS

#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"

enum rzb_default_widgets_event_state
{
	RZB_STATE_NONE = 0,
	RZB_STATE_PRESS,
	RZB_STATE_RELEASE,
};

enum rzb_default_widgets_events
{
	RZB_NONE = 0,

	RZB_MOUSE_CLICK_LEFT,
	RZB_MOUSE_CLICK_RIGHT,
	RZB_MOUSE_CLICK_MIDDLE,
	RZB_MOUSE_WHEEL_UP,
	RZB_MOUSE_WHEEL_DOWN,
	RZB_MOUSE_MOTION,

	RZB_KEY_ESCAPE,
	RZB_KEY_BACKSPACE,
	RZB_KEY_TAB,
	RZB_KEY_ENTER,
	RZB_KEY_SHIFT_LEFT,
	RZB_KEY_SHIFT_RIGHT,
	RZB_KEY_CTRL_LEFT,
	RZB_KEY_SPACE,
	RZB_KEY_CTRL_RIGHT,
	RZB_KEY_DELETE,
	RZB_KEY_HOME,
	RZB_KEY_END,
	RZB_KEY_PAGE_UP,
	RZB_KEY_PAGE_DOWN,
	RZB_KEY_UP,
	RZB_KEY_DOWN,
	RZB_KEY_LEFT,
	RZB_KEY_RIGHT,
};

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

	// default size of the window frame bar
	int frame_default_height;
	// default size of the window frame borders
	int frame_border_size;
	// default width of the window frame buttons
	int frame_button_size;

	// padding between the edge border and the content for some widgets
	int padding_bar;
	int padding_textbox;
	int padding_checkbox;
	int padding_radiobutton;
};

struct rzb_default_widgets_events_data
{
	int mouse_pos_x;
	int mouse_pos_y;
	char* typed_string;
};

struct rzb_default_widgets_context
{
	// events

	struct rzb_default_widgets_events_data events_data;

	// sizes

	int sizes_scale;
	struct rzb_default_widgets_sizes* sizes_current;
	struct rzb_default_widgets_sizes sizes_density_low;
	struct rzb_default_widgets_sizes sizes_density_medium;
	struct rzb_default_widgets_sizes sizes_density_high;

	// colors

	// widget shadow
	uint32_t color_shadow;
	// window frame
	uint32_t color_frame;
	// widget border
	uint32_t color_edge;
	// accent color
	uint32_t color_accent;
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
	struct rzb* rzb,
	struct rzb_default_widgets_context* context);

void rzb_default_widgets_density(
	struct rzb* rzb,
	struct rzb_default_widgets_context* context);

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context);

// tools

void rzb_nearest_widget(
	struct rzb* rzb,
	struct rzb_widget* widget,
	enum rzb_default_widgets_events direction);

#endif
