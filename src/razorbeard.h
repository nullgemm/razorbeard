#ifndef H_RAZORBEARD
#define H_RAZORBEARD

// TODO rzb alloc+free (root widget = handles with 1 section)
// TODO events callbacks (main handle callers)
//      (events are sent to razorbeard by the user, getting them in the first place is not our problem)
//      (the user will use a lut to translate willis events to razorbeard events)
//      (advanced input methods should not be taken into account quite yet
//       ibus does not support the wayland protocol
//       fcitx supports everything)
//      (when it is time remember XIM is THE method everyone uses under X11,
//       and the text_input_method protocol should be used for wayland,
//       clients are NOT supposed to use dbus directly)
// TODO widget alloc+free (individual widgets)
//
// TODO layout executor (the layout functions are part of the example unless forced)
// TODO render executor (the rendering functions are part of the hard-coded defaults)
// TODO events executor (the event functions are not customizable and set at init)

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

	struct rzb_widget** render_list;
	uint32_t render_list_size;

	struct rzb_widget*** window_partition;
	uint32_t window_partition_width;
	uint32_t window_partition_height;

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
