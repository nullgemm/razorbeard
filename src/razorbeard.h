#ifndef H_RAZORBEARD
#define H_RAZORBEARD

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// DONE forget about this corner saving idea, for both software and gpu backends...
// DONE ultra-specialize the opengl backends
//      we can't generalize GPU-based UI rendering, so just like the software backend,
//      this one will also be targetting exclusively classic desktop applications:
//      its only goal will be to provide an alternative backend for classic apps.
//      for games and other uses of the GPU-based UI rendering,
//      the developer is supposed to come up with custom rendering.
//
// DONE widget init and free functions (final)
// DONE widget sample render functions (final)
//      make it compile in a globox example
//      write a widget's render function, test in globox, iterate until finished
//      	(cropping must be supported and all cases test-rendered)
//      same for the next widget
//      etc.
// DONE widget stand-alone example render with globox (only for initial tests)
//
// DONE razorbeard init/free
// DONE razorbeard rendering
// DONE razorbeard event handling and selection (with smart arrow keys as well)
// 		- smart arrow keys work only for non-container widgets (in the demo kit)
// 		- smart arrow keys is a function called in widgets event callbacks
// 		- inputs are redirected to selected widget if any
// 		- otherwise or if the input was rejected we traverse the whole tree
// TODO widget event handling (specific stuff: tab switch, button click, scroll...)
// TODO razorbeard complete example with globox + willis + dpishit + cursoryx
//
// TODO smart removal of event grabber (when doing other ops such as detaching a widget...)
// TODO fix smart arrow keys selection out-of-bounds error
// TODO image widget
// TODO text rendering (bundle Noto and raqm in example rendering)
// TODO widget data getters and setters
// TODO new lib for complex input methods under linux, windows, macOS
//
// TODO popup and dropmenu widgets rendering (invalidate the whole window)
// TODO scrollbar widget and view
// TODO unit tests
// TODO rewrite

// pre-declaration
struct rzb_widget;

// dpi info
struct rzb_display_info
{
	int px_width;
	int px_height;
	int mm_width;
	int mm_height;
	double dpi_logic;
	double scale;
};

// razorbeard context
struct rzb
{
	struct rzb_widget* root_widget;
	struct rzb_widget* events_grabber;
	struct rzb_display_info* display_info;

	void* callback_send_events_data_context;
	void (*callback_send_events_data)(void* context, void* data);

	uint32_t* argb;
	int argb_width;
	int argb_height;
};

// razorbeard cropping info
struct rzb_cropping
{
	int x;
	int y;
	int width;
	int height;
};

// razorbeard widget
struct rzb_widget
{
	// linked list

	struct rzb_widget* parent;
	struct rzb_widget* siblings;
	struct rzb_widget* children;
	size_t children_count;
	size_t children_limit;
	bool render;
	bool hide;

	// layout

	// - cropped by render callback using parent data
	// - when inside a pager widget, the layout callback
	//   must be written by the user to offset the positions
	//   using the scroll value stored in the parent's widget data
	// - after re-rendering, the pager height and scroll value is updated
	int x;
	int y;
	int width;
	int height;

	// callbacks

	// layout-updating recursive traveral
	// saves resized widgets references in rzb
	// saves all widgets presence in hasmap cells
	void (*callback_layout)(
		struct rzb*,
		struct rzb_widget*);

	// called sequentially from the widget references list in rzb
	// this way we only render updated widgets, from background to foreground
	void (*callback_render)(
		struct rzb*,
		struct rzb_widget*,
		struct rzb_cropping*); // dlsym'd

	// called from a visual hashmap
	// this way we can find the targetted widget quickly
	// while allowing easy up/down/left/right navigation
	bool (*callback_events)(
		struct rzb*,
		struct rzb_widget*,
		int,
		int); // dlsym'd

	// free callback for widget-specific data
	void (*callback_free)(
		struct rzb*,
		struct rzb_widget*); // dlsym'd

	// widget-specific data (malloc'd on init)
	void* data_widget;
};


bool rzb_init(
	struct rzb* rzb,
	struct rzb_display_info* display_info);

bool rzb_free(
	struct rzb* rzb);

void rzb_send_event(
	struct rzb* rzb,
	int event_code,
	int event_state);

void rzb_send_events_data(
	struct rzb* rzb,
	void* data);

void rzb_set_events_data_callback(
	struct rzb* rzb,
	void (*callback)(void*, void*),
	void* context);

void rzb_grab_events(
	struct rzb* rzb,
	struct rzb_widget* widget);

bool rzb_render(
	struct rzb* rzb);

void rzb_render_all(
	struct rzb* rzb);


bool rzb_update_root_widget(
	struct rzb* rzb,
	struct rzb_widget* widget);

bool rzb_update_display_info(
	struct rzb* rzb,
	struct rzb_display_info* display_info);

bool rzb_make_child(
	struct rzb_widget* widget,
	struct rzb_widget* parent);

bool rzb_make_sibling(
	struct rzb_widget* widget,
	struct rzb_widget* sibling);

bool rzb_make_detached(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_select_widget(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_widget_set_hide(
	struct rzb_widget* widget,
	bool hide);

void rzb_widget_layout_update(
	struct rzb_widget* widget,
	int x,
	int y,
	int width,
	int height);

#endif
