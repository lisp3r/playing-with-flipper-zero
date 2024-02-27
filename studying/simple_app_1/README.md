# Simple App 1

What to learn here?

* How to print a string
* How to add an icon to the app
* How to log shit

![screenshot](Screenshot-20230402-193400.png)

Using `ViewPort` is the easiest way to create GUI interface.

Import headers:

```c
#include <furi.h>
#include <gui/gui.h>
```

Create a view_port object:
```c
ViewPort* view_port = view_port_alloc();
```

Create draw and input callbacks:

```c
static void draw_callback(Canvas* canvas, void* ctx) {}
static void input_callback(InputEvent* input_event, void* ctx) {}

...
view_port_draw_callback_set(view_port, draw_callback, app);
view_port_input_callback_set(view_port, input_callback, app);
```

Register view_port in gui:

```c
Gui* gui = furi_record_open(RECORD_GUI);
gui_add_view_port(gui, view_port, GuiLayerFullscreen);
```

Free:
```c
view_port_enabled_set(view_port, false); // Disable ViewPort
gui_remove_view_port(gui, view_port);
view_port_free(view_port);
furi_record_close(RECORD_GUI);
```

## Structure

App:

```c
typedef struct {
    Gui* gui;
    ViewPort* view_port;
} App;
```

...

## Highlights

If `context` attr is not used you shoud use `UNUSED()` function.

```c
static void draw_callback(Canvas* canvas, void* ctx) {
    // App* app = ctx;
    UNUSED(ctx);
```
