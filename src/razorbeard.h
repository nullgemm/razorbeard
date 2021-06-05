#ifndef H_RAZORBEARD
#define H_RAZORBEARD

// DONE forget about this corner saving idea, for both software and gpu backends...
// TODO ultra-specialize the opengl backends
//      we can't generalize GPU-based UI rendering, so just like the software backend,
//      this one will also be targetting exclusively classic desktop applications:
//      its only goal will be to provide an alternative backend for classic apps.
//      for games and other uses of the GPU-based UI rendering,
//      the developer is supposed to come up with custom rendering.
// TODO bundle Noto inside the default widgets library
//
// DONE widget init and free functions (final)
// TODO widget sample render functions (final)
//      make it compile in a globox example
//      write a widget's render function, test in globox, iterate until finished
//      	(cropping must be supported and all cases test-rendered)
//      same for the next widget
//      etc.
// TODO widget stand-alone example render with globox (only for initial tests)
//
// TODO razorbeard init/free
// TODO razorbeard rendering
// TODO razorbeard event handling
// TODO widget event handling
// TODO razorbeard complete example with globox + willis + dpishit + cursoryx
//
// TODO widget data getters and setters

// pre-declaration
struct rzb_widget;

// dpi info
struct rzb_display_info
{
	uint16_t px_width;
	uint16_t px_height;
	uint16_t mm_width;
	uint16_t mm_height;
	double dpi_logic;
	double scale;
};

// razorbeard context
struct rzb
{
	struct rzb_widget* root_widget;
	struct rzb_widget* events_grabber;

	struct rzb_widget** render_list;
	uint32_t render_list_size;

	struct rzb_widget*** window_partition;
	uint32_t window_partition_width;
	uint32_t window_partition_height;

	uint32_t* rzb_argb;
	struct rzb_display_info* display_info;
};

// razorbeard cropping info
struct rzb_cropping
{
	uint64_t x;
	uint64_t y;
	uint64_t width;
	uint64_t height;
};

// razorbeard widget
struct rzb_widget
{
	// linked list

	struct rzb_widget* parent;
	struct rzb_widget* siblings;
	struct rzb_widget* children;
	struct rzb_widget* window_partition_slot;
	size_t children_count;
	size_t children_limit;

	// layout

	// - cropped by render callback using parent data
	// - when inside a pager widget, the layout callback
	//   must be written by the user to offset the positions
	//   using the scroll value stored in the parent's widget data
	// - after re-rendering, the pager height and scroll value is updated
	uint64_t x;
	uint64_t y;
	uint64_t width;
	uint64_t height;

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
	void (*callback_events)(
		struct rzb*,
		struct rzb_widget*); // dlsym'd

	// free callback for widget-specific data
	void (*callback_free)(
		struct rzb*,
		struct rzb_widget*); // dlsym'd

	// widget-specific data (malloc'd on init)
	void* data_widget;
};


bool rzb_init(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_display_info* display_info);

bool rzb_free(
	struct rzb* rzb);

void rzb_send_event(
	struct rzb* rzb,
	int event_id);

void rzb_grab_events(
	struct rzb* rzb,
	struct rzb_widget* widget);

void rzb_render(
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


bool rzb_renderlist_add(
	struct rzb* rzb,
	struct rzb_widget* widget);

bool rzb_layout_update(
	struct rzb* rzb,
	struct rzb_widget* widget);

#endif
