# Coding for Flipper Zero :flipper:
notes on writing applications for flipper zero

## Build

    $ ./fbt launch_app APPSRC=appid

## Debug

* `FURI_LOG_D(TAG, MSG)` - debug
* `FURI_LOG_T(TAG, MSG)` - trace
* `FURI_LOG_I(TAG, MSG)` - info
* `FURI_LOG_E(TAG, MSG)` - error
* `FURI_LOG_W(TAG, MSG)` - warning

Example: `FURI_LOG_I("SNAKE", "Score is %d", score)`

To get the debug output you need to open Flipper Zero Command Line Interface:

```sh
$ screen /dev/serial/by-id/usb-Flipper_Devices_Inc._Flipper_Unfp0ur_flip_Unfp0ur-if00
...
>: log
Press CTRL+C to stop...

# To end the session, use ctl+a then ctl+d to detach.

$ screen -list
$ screen -X -S <sess name> quit  # lol don't blame me
```



## Code

Path to the Apps Assets folder will be /ext/apps_assets/hello_world

### Simple app

```c
#include <furi.h>

// Define log tag
#define TAG "app_name_or_smth"

...

// Application entry point
int32_t hello_world(void* p) {
    // Mark argument as unused
    UNUSED(p);

    ...
    return 0;
}
```

### Simple GUI app (does nothing)

```c
#include <furi.h>
#include <gui/gui.h>

typedef struct {
    Gui* gui;
    ViewPort* view_port;
} CounterApp;

void counter_app_free(CounterApp* app) {
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    furi_record_close(RECORD_GUI);
    free(app);
}

CounterApp* counter_app_alloc() {
    CounterApp* app = malloc(sizeof(CounterApp));
    app->view_port = view_port_alloc();
    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);
    return app;
}

int32_t counter_app(void* p) {
    UNUSED(p);

    CounterApp* app = counter_app_alloc();

    furi_delay_ms(2000);

    counter_app_free(app);
    return 0;
}

```

