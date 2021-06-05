# Razorbeard
Razorbeard is a minimalistic, modular and multiplatform retained-mode toolkit.

**Razorbeard is still under construction and nothing is functional yet**.
**Contributions will be accepted after the example software backend is finished**.

## Default widgets
Razorbeard comes with some default widgets to get you started more easily.
They are independent from the core however and can be removed if needed.
The library itself is very permissive so it's easy to make custom ones.

## Writing custom widgets
Fundamentally, all you need to have custom widgets is to create a `rzb_widget`
and make it use your own callbacks for rendering, event handling and cleanup.

The `data_widget` field can be used to retreive your widget's data easily
(but keep in mind *you* are responsible for the memory you allocate...).
In the default widgets bundled with Razorbeard, it usually points to a structure
holding the widget's data, specific handlers executed by the main event callback
and a reference to the default bundle's context used to get color and size info.

Make sure you understand what is the library's duty and what is a widget set's
before you start building your own custom widget pack, so it stays practical.

### Custom actions handling
Your custom widget library must handle the input actions itself.
In the default set, the main widget actions callback delegates
this task to user-defined handles stored in `data_widget`.

Remember that user input also includes switching between widgets.
Widgets are responsible for this because they must be able to
override these actions in some cases.
In a numberbox for instance, moving down from the "increase" button
results in selecting the "decrease" button from the same widget.
This means you are responsible for allowing widget switching in yours,
which can be easily done using Razorbeard's core functionalities.

Razorbeard doesn't know about events in itself, you simply bind ids
to specific widget kit events and register memory areas for the kit to read values.

### Custom rendering
For now, Razorbeard only supports software rendering, but OpenGL is planned.
When I will have learned Vulkan programming it might happen, too.
In the end you will be able to provide custom rendering.

## See also
You might be interested in my other similarly-minded projects:
 - [Globox](https://github.com/nullgemm/globox) (windowing)
 - [Willis](https://github.com/nullgemm/willis) (input handling)
 - [DPIshit](https://github.com/nullgemm/dpishit) (density detection)
 - [Cursoryx](https://github.com/nullgemm/cursoryx) (mouse cursor management)

## Greetings
Qt and GTK by being so fucking complicated.
