# MTG life counter

```sh
./fbt fap_life_counter
./fbt launch_app APPSRC=life_counter
```


## View Dispatcher

Import:

```c
#include <gui/view_dispatcher.h>
#include <gui/modules/widget.h>
```

Init view dispatcher:

```c
ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
view_dispatcher_enable_queue(view_dispatcher);
```

Add views to view dispatcher

```c
typedef enum {
  LifeCounterMainView,
  LifeCounterDeathView,
} LifeCounterViews;

// View* view = get_demo_view_object(); // Get a populated View* object.
view_dispatcher_add_view(view_dispatcher, MyDemoViewId, view);

// view = get_other_demo_view_object(); // Get a populated View* object.
view_dispatcher_add_view(view_dispatcher, MyOtherDemoViewId, view);
```

## Modules

Widget has 

- `static void gui_widget_view_draw_callback(Canvas* canvas, void* _model)`
- `static bool gui_widget_view_input_callback(InputEvent* event, void* context)`

```c
Widget 
```