#ifndef H_RAZORBEARD
#define H_RAZORBEARD

// TODO widget structs / ok: state + style + events
//
// TODO rzb alloc+free (root widget = handles with 1 section)
// TODO widget alloc+free (individual widgets)
//
// TODO layout executor (the layout functions are part of the example unless forced)
// TODO render executor (the rendering functions are part of the hard-coded defaults)
// TODO events executor (the event functions are not customizable and set at init)

// pre-declaration
struct rzb_widget;

// razorbeard context
struct rzb
{
	struct rzb_widget* root_widget;

	struct rzb_widget** render_list;
	uint32_t render_list_size;

	struct rzb_widget*** window_partition;
	uint32_t window_partition_width;
	uint32_t window_partition_height;
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
		struct rzb_widget*); // dlsym'd

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
	void* data_widget; // state info
	void* data_render; // style info
	void* data_events; // event callbacks
};

#endif
