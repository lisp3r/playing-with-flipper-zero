# View

from [@jamisonderek's User Interface](https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface#viewport)

- A View is the objects that are added to the [ViewDispatcher](#viewdispatcher) and referenced later in the ViewDispatcher by id. 
- operates through callbacks:
  - `view_set_draw_callback`
  - `view_set_input_callback`
  - lot more callbacks that can also be registered with a View
- set a custom callback: `view_dispatcher_send_custom_event method`
- set a previous callback: it will be invoked when the Back button is pressed.
  - If the previous callback returns a view id then the navigation is changed to view associated with that id. 
- There are also enter/exit callbacks that can be registered to know when a View is switched to/or away from.

## Key concepts

### Import <gui/view.h>

Import the view header file, which will import canvas, input/input.h, and a few other headers.

```c
#include <gui/view.h>
```

### Define drawing callback

- Define a callback function to get invoked for drawing. You can name the function anything.
- Typically, your callback will render on the Canvas.

```c
static void my_draw_callback(Canvas* canvas, void* model) {
  // TODO: If we need the model, cast the model to the correct type.
  // TODO: Render something on the canvas.
}
```

### Define input callback

- Define a callback function to get invoked for input. 
- **Typically, your callback will _put messages in the MessageQueue to be processed by your application.**

```c
static void my_input_callback(InputEvent* input_event, void* context) {
  // TODO: queue an input event 
}
```

### Previous callback

- The navigation callback method will get invoked when the Back button is pressed. 
- This method should return the view id that matches one of the identifiers registered in the ViewDispatcher. 
- You can use VIEW_NONE to hide the view_port and VIEW_IGNORE to ignore the request. 
- The context passed to the callback is the object specified in view_set_context(view, context);

Create the callback method that will get invoked when the Back button is pressed.


```c
uint32_t my_view_navigation_callback(void* context) {
  return MyDemoViewId; // Return the view id that is registered in the ViewDispatcher.
}
```

Register the callback method.

```c
view_set_previous_callback(view, my_view_navigation_callback);
```
### Enter callback

- The enter callback method will get invoked when the `view_dispatcher_set_current_view` is called and the view is switched to.
- Create the callback method that will get invoked when the view is switched:

```c
void my_enter_view_callback(void* context) {
}
```

### Register the callback method.

```c
view_set_enter_callback(view, my_enter_view_callback)
```

...


### Free resources

When your application is done, you should free the resources you allocated.

```c
view_free(view);
```

# Modules

- Modules are views that you can configure with specific data, then leverage the view in your application. 
- All modules expose _alloc, _free and _get_view methods. 
- Many modules also expose methods to configure the module and to set additional callbacks needed by the module (like result_callback, validator_callback, etc.)












