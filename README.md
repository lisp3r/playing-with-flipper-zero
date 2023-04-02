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

### Studying

* Simple GUI app (does nothing): [simple_app_1](studying/simple_app_1/)
* Simple GUI app (does nothing) with input callback: [simple_app_2](studying/simple_app_2/)

## Resourses
* [Flipper Zero Firmware Documentation](https://doxy.flipperzero.one/dev/index.html)
* ["Сборка Hello World под Flipper Zero" by Pavel Yakovlev](https://yakovlev.me/hello-flipper-zero/)
* [Flipper Hello World app](https://github.com/giolaq/helloflipper)
* [The Flipper Zero user interface editor](https://ilin.pt/stuff/fui-editor/)
* WARNING: this one is completely outdated though is a good step by step guide for newbies. [Your First Plugin](https://flipper.atmanos.com/docs/category/your-first-plugin)
