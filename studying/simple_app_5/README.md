## Sources:

- [@jamisonderek's](https://github.com/jamisonderek) flipper zero tutorials:
  - [Scenes Demo Application Tutorial](https://github.com/jamisonderek/flipper-zero-tutorials/tree/main/ui/basic_scenes) - basic Scene tutorial with rich description
  - [User Interface article](https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface#viewport)

- [flipper-zero-tutorial-app](https://github.com/instantiator/flipper-zero-tutorial-app/tree/main)

This app is WAY MORE difficult then the previous ones.

`ViewPort` was suitable for creating simple UI interfaces. In this app we switch it for `ViewDispatcher`.



## View

A **View** is the objects that are added to the **ViewDispatcher** and referenced later in the ViewDispatcher by **id**. A **view** operates through callbacks.

**View** assosiated with **Modules**. There are many Gui modules for various tasks, like input, file dialogs, menus, etc. See: [A Visual Guide to Flipper Zero GUI Modules](https://brodan.biz/blog/a-visual-guide-to-flipper-zero-gui-components/)

There are many pre-built modules, which expose a configured View object. You can use the `View*` that is returned from the Modules `_get_view(...)` function.

Get View* object from Menu module object:

```c
#include <gui/modules/menu.h>

Menu* menu = menu_alloc();
View* view = menu_get_view(menu);
```

Get View* object from PopUp module object:

```c
#include <gui/modules/popup.h>

Popup* popup = popup_alloc();
View* view = popup_get_view(popup);
```

Callbacks:

- custom callback - invoked by the `view_dispatcher_send_custom_event` method. 
- previous callback - invoked when the Back button is pressed.

    If the *previous callback* returns a view id (that was previously registered with the ViewDispatcher) then the navigation is changed to view associated with that id. 

- enter/exit callbacks

I do not use vanilla View objects here because Modules is enough for now. They implemented some of the callbacks.

```c
// from applications/services/gui/modules/menu.c

#include "menu.h"
...

struct Menu {
    View* view;
};

...

static void menu_draw_callback(Canvas* canvas, void* _model) {
    ...
static bool menu_input_callback(InputEvent* event, void* context) {
    ...

Menu* menu_alloc() {
    Menu* menu = malloc(sizeof(Menu));
    menu->view = view_alloc(menu->view);
    view_set_context(menu->view, menu);
    view_allocate_model(menu->view, ViewModelTypeLocking, sizeof(MenuModel));
    view_set_draw_callback(menu->view, menu_draw_callback);
    view_set_input_callback(menu->view, menu_input_callback);
    view_set_enter_callback(menu->view, menu_enter);
    view_set_exit_callback(menu->view, menu_exit);
    ...
```

More info on View in general: [User Interface#View](https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface#view)

Obviously Models doesn't implement custom and previous callbacks because it is beyond their area of responsibility. This is how to add them if you are using Modules:

```c
#include <gui/modules/variable_item_list.h>

typedef struct {
    ...
    ViewDispatcher* view_dispatcher;
    VariableItemList* variable_item_list;  // Module
} NotificationAppSettings;

typedef enum {
    NotificationAppSettingsViewSubmenu,
    NotificationAppSettingsViewPopup,
} NotificationAppSettingsView;

// previous callback returns id of another view
static uint32_t notification_app_settings_exit(void* context) {
    UNUSED(context);
    return NotificationAppSettingsViewPopup;
}

static NotificationAppSettings* alloc_settings() {
    NotificationAppSettings* app = malloc(sizeof(NotificationAppSettings));

    app->variable_item_list = variable_item_list_alloc();
    View* view = variable_item_list_get_view(app->variable_item_list);  // getting view from the module
    view_set_previous_callback(view, notification_app_settings_exit);   // set the callback
    ...
    view_dispatcher_add_view(app->view_dispatcher, 0, view);
    ...

static void free_settings(NotificationAppSettings* app) {
    variable_item_list_free(app->variable_item_list);
    ...
```

I b this application we do not need to do it because `SceneManager` does it for us.

## ViewDispatcher

The view dispatcher is used for applications that want multiple **View** objects to switch between them by view id. If the BACK button is pressed and the view does not return true from the *input callback*, then the *view's navigation callback* is used to determine the view id to switch to.

### Import <gui/view_dispatcher.h>

```c
#include <gui/view_dispatcher.h>
```

### Allocate a ViewDispatcher and enable its queue

```c
ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
view_dispatcher_enable_queue(view_dispatcher);
```

### Create an enum of view ids

```c
typedef enum {
  TestAppDemoViewId,
  TestAppOtherDemoViewId,
} TestAppViewIds;
```

### Add view to view dispatcher

```c
// View* view = get_demo_view_object(); // Get a populated View* object.
view_dispatcher_add_view(view_dispatcher, MyDemoViewId, view);

// view = get_other_demo_view_object(); // Get a populated View* object.
view_dispatcher_add_view(view_dispatcher, MyOtherDemoViewId, view);
```

### Navigation events

You must use `view_dispatcher_set_navigation_event_callback` if you want the back button to eventually terminate your application.

See [Navigation Events](https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface#navigation-events) for mor information.

```c
bool test_app_view_dispatcher_navigation_event_callback(void* context) {
  // Return true if you handled the event, or if you want to ignore the event.
  // Only return false if you want the ViewDispatcher to stop.
  return true; 
}

view_dispatcher_set_navigation_event_callback(
    view_dispatcher,
    test_app_view_dispatcher_navigation_event_callback
);
```

I use `SceneManager` so I just need to deligate the callback to it:

```c
bool test_app_scene_manager_navigation_event_callback(void* context) {
    furi_assert(context);
    TestApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

view_dispatcher_set_navigation_event_callback(
    view_dispatcher,
    test_app_scene_manager_navigation_event_callback
);
```

### Attach view dispatcher to Gui

```c
Gui* gui = furi_record_open(RECORD_GUI);
view_dispatcher_attach_to_gui(view_dispatcher, gui, ViewDispatcherTypeFullscreen);
```

### Switch to view

Switch to one of the registered views and start running the dispatcher.

```c
view_dispatcher_switch_to_view(view_dispatcher, TestAppDemoViewId);
view_dispatcher_run(view_dispatcher);
```

### Switch to another view

In your callback code: switch to a different view when something interesting happens.

```c
view_dispatcher_switch_to_view(view_dispatcher, TestAppOtherDemoViewId);
```

### Stop view dispatcher

In your callback code: stop view needed.

```c
view_dispatcher_stop(view_dispatcher);
```

### Free resources

When your application is done, you should disable the free the resources you allocated.

```c
view_dispatcher_remove_view(view_dispatcher, TestAppDemoViewId); // Be sure to remove your view.
view_free(view);
view_dispatcher_free(view_dispatcher);
furi_record_close(RECORD_GUI);
```

## Custom events

`view_dispatcher_send_custom_event` can be used to send an event_id to the registered **`_custom_event_callback`** method. 

The event will first be passed to the View's `custom_callback` handler, but if that returns `false` then the ViewDispatcher **`_custom_event_callback`** will be invoked. 

The context is set using `view_dispatcher_set_event_callback_context` (which impacts the void* context for navigation, tick and custom).

```c
bool my_view_dispatcher_custom_event_callback(void* context, uint32_t event) {
  // NOTE: The return value is not currently used by the ViewDispatcher,
  // however I recommend returning true if you handled the event and false
  // if it is still unhandled, since the API may change in the future.
  return true; 
}

view_dispatcher_set_event_callback_context(view_dispatcher, my_context);
view_dispatcher_set_custom_event_callback(view_dispatcher, my_view_dispatcher_custom_event_callback);

uint32_t event_id = 42; // Send a custom event of 42 to the custom_event_callback.
view_dispatcher_send_custom_event(view_dispatcher, event_id);
```

I use `SceneManager` so I just need to deligate the callback to it:

```c
bool test_app_scene_manager_custom_event_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    TestApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}
```

## SceneManager

The scene manager is typically used in conjunction with the view dispatcher. 

When you allocate the scene manager, you pass configuration to register your scenes (each scene has an on_enter, on_event and on_exit callback handler). 

The scene manager has methods to navigate (previous, back, another, next) to a scene. 

Each scene has a state (which is a uint32_t number) with methods to get and set the state for a scene.

### Create scenes

Note: the last scene named `XXXCount` is nesessery.

```c
// In our demo we have two scenes: MainMenu and GreetingMessage.
typedef enum {
    DemoMainMenuScene,
    DemoGreetingMessageScene,
    DemoSceneCount, // Last element should be "Count".
} DemoScene;
```

### Define handlers

Each scene has three default callbacks: 

- `on_enter` - invoked when the scene manager switches to this scene

    ```c
    void demo_main_menu_scene_on_enter(void* context) {
        UNUSED(context);
    }
    ```

- `on_exit` - invoked when the scene manager switches away from this scene

    ```c
    void demo_main_menu_scene_on_exit(void* context) {
        UNUSED(context);
    }
    ```

- `on_event` - invoked when **Tick**, **Back** and **Custom** events happen. The event has a type (`SceneManagerEventTypeCustom`, `SceneManagerEventTypeBack` or `SceneManagerEventTypeTick`) and optionally an event (`uint32_t`). If the event was handled, the method should return `true`.

    ```c
    bool demo_main_menu_scene_on_event(void* context, SceneManagerEvent event) {
        UNUSED(context);
        UNUSED(event);
        return false; // event not handled.
    }
    ```

### Group handlers

```c
void (*const demo_scene_on_enter_handlers[])(void*) = {
    demo_main_menu_scene_on_enter,
    demo_greeting_message_scene_on_enter,
};

void (*const demo_scene_on_exit_handlers[])(void*) = {
    demo_main_menu_scene_on_exit,
    demo_greeting_message_scene_on_exit,
};

bool (*const demo_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    demo_main_menu_scene_on_event,
    demo_greeting_message_scene_on_event,
};

static const SceneManagerHandlers demo_scene_manager_handlers = {
    .on_enter_handlers = demo_scene_on_enter_handlers,
    .on_event_handlers = demo_scene_on_event_handlers,
    .on_exit_handlers = demo_scene_on_exit_handlers,
    .scene_num = DemoSceneCount,
};
```

I use [preprocessor magic](#preprocessor-magic) to do this.

### Define an object for context

```c
typedef struct App {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;
    Submenu* submenu;
    Widget* widget;
} App;
```

### Allocate the scene manager

```c
App* app = malloc(sizeof(App));
app->scene_manager = scene_manager_alloc(&demo_scene_manager_handlers, app);
```

### Switch to another scene

```c
scene_manager_next_scene(app->scene_manager, BasicScenesMainMenuScene);
```

### Handle back event

When the back button is pressed, `scene_manager_handle_back_event` will first invoke the current scenes `on_event` handler with a `SceneManagerEventTypeBack` type event. If that method returns `false`, then it will invoke the `scene_manager_previous_scene` method.

```c
scene_manager_handle_back_event(app->scene_manager);
```

### Stop scene manager

Calling `scene_manager_stop` will invoke the current scene's `on_exit` handler.

```c
scene_manager_stop(app->scene_manager);
```

### Free scene manager

Calling `scene_manager_free` will free any resources used by the `scene_manager`. You should call `scene_manager_stop`, if you need to ensure the current scene had its `on_exit` handler invoked.

```c
scene_manager_free(app->scene_manager);
```

## SceneManager with ViewDispatcher

Common patterns for using SceneManager with ViewDispatcher

### Callbacks for back event and custom event

* Create a `back_event` callback for your view dispatcher, that will invoke the `scene_manager_handle_back_event` method:

    ```c
    bool demo_back_event_callback(void* context) {
        furi_assert(context);
        App* app = context;

        return scene_manager_handle_back_event(app->scene_manager);
    }
    ```

* Create a custom callback for your view dispatcher, that will invoke the `scene_manager_handle_custom_event` method: 

    ```c
    bool demo_custom_callback(void* context, uint32_t custom_event) {
        furi_assert(context);
        App* app = context;
        return scene_manager_handle_custom_event(app->scene_manager, custom_event);
    }
    ```

* In the method that allocates the view dispatcher, register your navigation callback and your custom callback with the view dispatcher:

    ```c
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app); // make sure context is app.
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, demo_custom_callback); // here
    // navigation callback from previous step.
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, demo_back_event_callback);
    ```

### on_enter switches View

* In your on_enter callbacks, you should configure your view (or module) and then switch to the view. 

    ```c
    void demo_greeting_message_scene_on_enter(void* context) {
        App* app = context;
        widget_reset(app->widget);
        widget_add_string_element(
            app->widget, 25, 15, AlignLeft, AlignCenter, FontPrimary, "Hello World!");
        view_dispatcher_switch_to_view(app->view_dispatcher, DemoWidgetView);
    }
    ```

### callbacks forward to on_event

* If you have a callback (like a menu callback) that callback should leverage `scene_manager_handle_custom_event` passing a custom event id as the second parameter. This will end up forwarding the event back to your on_event callback for the scene. This helps keep all of the scene's logic in the on_event method. 

    ```c
    void demo_menu_callback(void* context, uint32_t index) {
        App* app = context;

        switch(index) {
            case DemoMenuSceneGreeting:
                scene_manager_handle_custom_event(app->scene_manager, DemoGreetingEvent); // Some custom event id that we will handle.
                break;
            }
    }
    ```

* In your `on_event` for the scene handle the custom events that you sent. 

    ```c
    bool basic_scenes_main_menu_scene_on_event(void* context, SceneManagerEvent event) {
        App* app = context;
        bool consumed = false; // by default return false (so back logic, etc. will work)
        switch(event.type) {
            case SceneManagerEventTypeCustom:
                switch(event.event) {
                    case DemoGreetingEvent:
                        scene_manager_next_scene(app->scene_manager, DemoGreetingScene);
                        consumed = true; // We handled event, so return true.
                        break;
                }
            break;
        default:
            break;
        }
        return consumed;
    }
    ```

## df


Structure:

- simple_app_5.c
- simple_app_5_i.h




You can think of a scene as a UI screen with some purpose. Each scene typically uses one module, but multiple scenes may reuse the same module. For example in a sound application, a "volume scene" and a "frequency scene" may both use the same knob module to let the user select the value (but one scene is setting the volume value while the other is setting the frequency).

The SceneManager will handle the transition between scenes and uses the ViewDispatcher to handle the transition between views. 

### Scenes

A scene uses a module to display the user interface on the Flipper Zero. The module for the scene also handles the user input. **The scene manager** will display the scene and handle the transition between scenes. 

**The scene manager** will call the scene's `on_enter` callback when the scene is displayed. 
**The scene manager** will call the scene's `on_event` callback when a custom event is invoked. 
**The scene manager** will call the scene's `on_exit` callback when the scene is no longer displayed.

Our application will have the following scenes:

- SimpleApp5ViewStart - The main menu scene.

    BasicScenesMainMenuScene - The main menu scene.
    BasicScenesLotteryScene - The lottery number generator scene.
    BasicScenesGreetingInputScene - The greeting name input scene.
    BasicScenesGreetingMessageScene - The greeting message scene.


### Modules / Views

A module (or more specifically, a module's view) will display the user interface on the Flipper Zero. It will also handle the user input.

List of all (?) of the modules available in applications/services/gui: [A Visual Guide to Flipper Zero GUI Components](https://brodan.biz/blog/a-visual-guide-to-flipper-zero-gui-components/).

Our application will use the following modules:

- widget - The widget module (for displaying the greeting message)

    submenu - The submenu module (for displaying the main menu)
    widget - The widget module (for displaying the lottery numbers & greeting message)
    text_input - The text input module (for entering the user's name)

We will refer to the above module's view using the following:

- SimpleApp5ViewView - The widget's view.

    BasicScenesSubmenuView - The submenu's view.
    BasicScenesWidgetView - The widget's view.
    BasicScenesTextInputView - The text_input's view.





















For each scene you should create three callbacks:

```c
void simple_app_5_scene_start_on_enter(void* context) {}

void simple_app_5_scene_start_on_exit(void* context) {}

bool simple_app_5_scene_start_on_event(void* context, SceneManagerEvent event) {}
```

plus addidional callbacks for keys and so:

```c
static void simple_app_5_scene_start_ok_callback(GuiButtonType result, InputType type, void* context) {}
```


## Preprocessor magic

The code below expands as follows

```c
// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const file_browser_scene_on_enter_handlers[])(void*) = {
#include "file_browser_scene_config.h"
};
#undef ADD_SCENE
```

Аirst of all, the `#include` is inserted:

```c
// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const file_browser_scene_on_enter_handlers[])(void*) = {
ADD_SCENE(file_browser, start, Start)
ADD_SCENE(file_browser, browser, Browser)
ADD_SCENE(file_browser, result, Result)
};
#undef ADD_SCENE
```

Then `#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter` is at work:

```c
// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const file_browser_scene_on_enter_handlers[])(void*) = {
file_browser_scene_start_on_enter,
file_browser_scene_browser_on_enter,
file_browser_scene_result_on_enter,
};
#undef ADD_SCENE
```

## Note on naming

...
