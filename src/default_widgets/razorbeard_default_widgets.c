#include <stdint.h>
#include <stdbool.h>
#include "razorbeard.h"
#include "razorbeard_default_widgets.h"

bool rzb_default_widgets_init(
	struct rzb_default_widgets_context* context,
	void** events_data,
	int* events_table)
{
	context->events_table = events_table;
	context->events_data_typed_string = events_data;
	context->events_data_mouse_pos_x = 0;
	context->events_data_mouse_pos_y = 0;

	struct rzb_default_widgets_sizes sizes_density_low =
	{
		.size_shadow = 3;
		.size_edge_border = 1;
		.size_selected_border = 1;
		.size_font = 10;

		.size_shine_edge = 1;
		.size_shine_gradient = 15;
		.radius_edge_border = 3;

		.size_slider = 6;
		.size_separator = 1;
		.size_textarea_corner = 6;

		.tab_default_width = 44;
		.tab_default_height = 16;

		.padding_bar = 1;
		.padding_checkbox = 1;
		.padding_radiobutton = 2;
	};

	struct rzb_default_widgets_sizes sizes_density_medium =
	{
		.size_shadow = 5;
		.size_edge_border = 1;
		.size_selected_border = 2;
		.size_font = 12;

		.size_shine_edge = 1;
		.size_shine_gradient = 30;
		.radius_edge_border = 4;

		.size_slider = 8;
		.size_separator = 2;
		.size_textarea_corner = 12;

		.tab_default_width = 106;
		.tab_default_height = 33;

		.padding_bar = 1;
		.padding_checkbox = 3;
		.padding_radiobutton = 5;
	};

	struct rzb_default_widgets_sizes sizes_density_high =
	{
		.size_shadow = 8;
		.size_edge_border = 2;
		.size_selected_border = 3;
		.size_font = 18;

		.size_shine_edge = 2;
		.size_shine_gradient = 45;
		.radius_edge_border = 7;

		.size_slider = 12;
		.size_separator = 3;
		.size_textarea_corner = 18;

		.tab_default_width = 159;
		.tab_default_height = 49;

		.padding_bar = 2;
		.padding_checkbox = 5;
		.padding_radiobutton = 7;
	};

	context->sizes_current = &(context->sizes_density_medium);
	context->sizes_density_low = sizes_density_low;
	context->sizes_density_medium = sizes_density_medium;
	context->sizes_density_high = sizes_density_high;

	context->color_shadow = 0x000000;
	context->color_edge = 0x0e0e0e;
	context->color_selected = 0x2b527b;
	context->color_text = 0xeaeaea;
	context->color_background = 0x292929;
	context->color_foreground = 0x191919;
	context->color_background_box = 0x232323;
	context->color_foreground_shine = 0x1e1e1e;
}

bool rzb_default_widgets_free(
	struct rzb_default_widgets_context* context)
{
	// only here as a template in case of developer modifications,
	// it is not actually needed by default as we don't make
	// dynamic memory allocations in the widgets context
}

// handles

struct rzb_widget*
	rzb_alloc_widget_handles(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal,
		uint32_t sections_count)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = sections_count;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = rzb_event_widget_handles;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_handles handles =
	{
		.context = context;
		.horizontal = horizontal;
		.sections_count = sections_count;
		.section_lengths = malloc(sections_count * sizeof (uint32_t));
		.section_dragging = NULL;
	};

	if (handles.section_lengths == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_handles* data =
		malloc(sizeof (struct rzb_widget_handles));

	if (data == NULL)
	{
		free(handles.section_lengths);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = handles;
	widget->data_widget = data;
}

void rzb_free_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_handles* data = widget->data_widget;

	free(data->section_lengths);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_handles(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_handles_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_handles_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

struct rzb_widget_handles
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	uint32_t sections_count;
	uint32_t* section_lengths;
	uint32_t* section_dragging;
};

// pager

struct rzb_widget*
	rzb_alloc_widget_pager(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		bool horizontal)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_pager;
	widget->callback_events = rzb_event_widget_pager;
	widget->callback_free = rzb_free_widget_pager;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_pager pager =
	{
		.context = context;
		.horizontal = horizontal;
		.size_page = 0;
		.position_page = 0;
	};

	// prepare persistent memory area
	struct rzb_widget_pager* data =
		malloc(sizeof (struct rzb_widget_pager));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = pager;
	widget->data_widget = data;
}

void rzb_free_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_pager(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_pager_scroll_up(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_pager_scroll_down(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

struct rzb_widget_pager
{
	struct rzb_default_widgets_context* context;

	bool horizontal;
	uint32_t size_page;
	uint32_t position_page;
};

// tabs

struct rzb_widget*
	rzb_alloc_widget_tabs(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*tab_switched)(struct rzb*, struct rzb_widget*),
		char** tabs_names,
		uint32_t tab_active)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_tabs;
	widget->callback_events = rzb_event_widget_tabs;
	widget->callback_free = rzb_free_widget_tabs;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_tabs tabs =
	{
		.context = context;
		.tab_switched = tab_switched;
		.tabs_names = tabs_names;
		.tab_active = tab_active;
	};

	// prepare persistent memory area
	struct rzb_widget_tabs* data =
		malloc(sizeof (struct rzb_widget_tabs));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = tabs;
	widget->data_widget = data;
}

void rzb_free_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_tabs(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_tabs_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_tabs_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

struct rzb_widget_tabs
{
	struct rzb_default_widgets_context* context;

	void (*tab_switched)(struct rzb*, struct rzb_widget*);

	char** tabs_names;
	uint32_t tab_active;
};

// popup

struct rzb_widget*
	rzb_alloc_widget_popup(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_popup;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_popup;
	widget->data_widget = context;
}

void rzb_free_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_popup(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

// dropmenu

struct rzb_widget*
	rzb_alloc_widget_dropmenu(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 1;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_dropmenu;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_dropmenu;
	widget->data_widget = context;
}

void rzb_free_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_dropmenu(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

// separator

struct rzb_widget*
	rzb_alloc_widget_separator(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_separator;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_separator;
	widget->data_widget = context;
}

void rzb_free_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget);
}

void rzb_render_widget_separator(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

// text

struct rzb_widget*
	rzb_alloc_widget_text(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		char* text)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_text;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_text;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_text text =
	{
		.context = context;
		.text = strdup(text);
	};

	if (text.text == NULL)
	{
		free(widget);
	}

	// prepare persistent memory area
	struct rzb_widget_text* data =
		malloc(sizeof (struct rzb_widget_text));

	if (data == NULL)
	{
		free(text.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = text;
	widget->data_widget = data;
}

void rzb_free_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_text* data = widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_text(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

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
		uint32_t* rgba)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = NULL;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_image image =
	{
		.context = context;
		.alpha_premult = alpha_premult;
		.gamma = gamma;
		.rgba = rgba;
	};

	struct rzb_widget_image* data =
		malloc(sizeof (struct rzb_widget_image));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = image;
	widget->data_widget = data;
}

void rzb_free_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_image(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

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
		char* text)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = rzb_event_widget_handles;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_button button =
	{
		.context = context;

		.button_pressed = button_pressed;
		.button_released = button_released;
		.button_activated = button_activated;

		.pushed = false;
		.toggle = toggle;
		.active = false;
		.text = strdup(text);
	};

	if (button.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_button* data =
		malloc(sizeof (struct rzb_widget_button));

	if (data == NULL)
	{
		free(button.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = button;
	widget->data_widget = data;
}

void rzb_free_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_button* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_button(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_button_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_button_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		char* value)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = rzb_event_widget_handles;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_numberbox numberbox =
	{
		.context = context;
		.number_changed = number_changed;
		.pushed_up = false;
		.pushed_down = false;
		.value = strdup(value);
	};

	if (numberbox.value == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_numberbox* data =
		malloc(sizeof (struct rzb_widget_numberbox));

	if (data == NULL)
	{
		free(numberbox.value);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = numberbox;
	widget->data_widget = data;
}

void rzb_free_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_numberbox* data =
		widget->data_widget;

	free(data->value);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_numberbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_increase(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_decrease(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_numberbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		char* text)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_textbox;
	widget->callback_events = rzb_event_widget_textbox;
	widget->callback_free = rzb_free_widget_textbox;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_textbox textbox =
	{
		.context = context;
		.text_changed = text_changed;
		.cursor_beg = NULL;
		.cursor_end = NULL;
		.text = strdup(text);
	};

	if (textbox.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_textbox* data =
		malloc(sizeof (struct rzb_widget_textbox));

	if (data == NULL)
	{
		free(textbox.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = textbox;
	widget->data_widget = data;
}

void rzb_free_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_textbox* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_textbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textbox_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		char* text)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_handles;
	widget->callback_events = rzb_event_widget_handles;
	widget->callback_free = rzb_free_widget_handles;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_textarea textarea =
	{
		.context = context;
		.text_changed = text_changed;
		.resizable = resizable;
		.resizing = false;
		.cursor_beg = NULL;
		.cursor_end = NULL;
		text = strdup(text);
	};

	if (textarea.text == NULL)
	{
		free(widget);
		return NULL;
	}

	// prepare persistent memory area
	struct rzb_widget_textarea* data =
		malloc(sizeof (struct rzb_widget_textarea));

	if (data == NULL)
	{
		free(textarea.text);
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = textarea;
	widget->data_widget = data;
}

void rzb_free_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	struct rzb_widget_textarea* data =
		widget->data_widget;

	free(data->text);
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_textarea(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_select_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_select_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_resize_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_resize_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_textarea_text(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		bool checked)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_radiobutton;
	widget->callback_events = rzb_event_widget_radiobutton;
	widget->callback_free = rzb_free_widget_radiobutton;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_radiobutton radiobutton =
	{
		.context = context;
		.radiobutton_changed = radiobutton_changed;
		.checked = checked;
	};

	// prepare persistent memory area
	struct rzb_widget_radiobutton* data =
		malloc(sizeof (struct rzb_widget_radiobutton));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = radiobutton;
	widget->data_widget = data;
}

void rzb_free_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_radiobutton(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_radiobutton_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_radiobutton_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		bool checked)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_checkbox;
	widget->callback_events = rzb_event_widget_checkbox;
	widget->callback_free = rzb_free_widget_checkbox;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_checkbox checkbox =
	{
		.context = context;
		.checkbox_changed = checkbox_changed;
		.checked = checked;
	};

	// prepare persistent memory area
	struct rzb_widget_checkbox* data =
		malloc(sizeof (struct rzb_widget_checkbox));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = checkbox;
	widget->data_widget = data;
}

void rzb_free_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_checkbox(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_checkbox_click(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_checkbox_release(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		void (*scrollbar_moved)(struct rzb*, struct rzb_widget*))
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_scrollbar;
	widget->callback_events = rzb_event_widget_scrollbar;
	widget->callback_free = rzb_free_widget_scrollbar;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_scrollbar scrollbar =
	{
		.context = context;
		.scrollbar_moved = scrollbar_moved;
		.pushed = false;
		.size_scrollbar = 0;
		.position_scrollbar = 0;
	};

	// prepare persistent memory area
	struct rzb_widget_scrollbar* data =
		malloc(sizeof (struct rzb_widget_scrollbar));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = scrollbar;
	widget->data_widget = data;
}

void rzb_free_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_scrollbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_scrollbar_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_scrollbar_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

struct rzb_widget_scrollbar
{
	struct rzb_default_widgets_context* context;

	void (*scrollbar_moved)(struct rzb*, struct rzb_widget*);

	bool pushed;
	uint32_t size_scrollbar;
	uint32_t position_scrollbar;
};

// slider

struct rzb_widget*
	rzb_alloc_widget_slider(
		struct rzb* rzb,
		void (*callback_layout)(struct rzb*, struct rzb_widget*),
		struct rzb_default_widgets_context* context,
		void (*slider_moved)(struct rzb*, struct rzb_widget*),
		bool vertical,
		uint32_t progress)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_slider;
	widget->callback_events = rzb_event_widget_slider;
	widget->callback_free = rzb_free_widget_slider;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_slider slider =
	{
		.context = context;
		.slider_moved = slider_moved;
		.pushed = false;
		.vertical = vertical;
		.progress = progress;
	};

	// prepare persistent memory area
	struct rzb_widget_slider* data =
		malloc(sizeof (struct rzb_widget_slider));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = slider;
	widget->data_widget = data;
}

void rzb_free_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

void rzb_event_widget_slider(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_slider_move_start(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

void rzb_event_widget_slider_move_end(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	// TODO
}

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
		uint32_t progress)
{
	struct rzb_widget* widget = malloc(sizeof (struct rzb_widget));

	if (widget == NULL)
	{
		return NULL;
	}

	widget->parent = NULL;
	widget->siblings = NULL;
	widget->children = NULL;
	widget->window_partition_slot = NULL;
	widget->children_count = 0;
	widget->children_limit = 0;

	widget->x = 0;
	widget->y = 0;
	widget->width = 0;
	widget->height = 0;

	widget->callback_layout = callback_layout;
	widget->callback_render = rzb_render_widget_progressbar;
	widget->callback_events = rzb_event_widget_progressbar;
	widget->callback_free = rzb_free_widget_progressbar;
	widget->data_widget = NULL;

	// widget-specific data
	struct rzb_widget_progressbar progressbar =
	{
		.context = context;
		.vertical = vertical;
		.progress = progress;
	};

	// prepare persistent memory area
	struct rzb_widget_progressbar* data =
		malloc(sizeof (struct rzb_widget_progressbar));

	if (data == NULL)
	{
		free(widget);
		return NULL;
	}

	// naive-copy the data and link it to the widget
	*data = progressbar;
	widget->data_widget = data;
}

void rzb_free_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget)
{
	free(widget->data_widget);
	free(widget);
}

void rzb_render_widget_progressbar(
	struct rzb* rzb,
	struct rzb_widget* widget,
	struct rzb_cropping* cropping)
{
	// TODO
}

struct rzb_widget_progressbar
{
	struct rzb_default_widgets_context* context;

	bool vertical;
	uint32_t progress; // 16.16 fixed-point
};
