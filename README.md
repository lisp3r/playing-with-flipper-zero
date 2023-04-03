# Coding for Flipper Zero :flipper:
A bunch of notes on writing applications for [Flipper Zero](https://flipperzero.one/).

## How to build an app

1. Install the dev firmware version on your Flipper with [qFlipper](https://github.com/flipperdevices/qFlipper) or from source code. To be honest, I don't know if this is needed or not, but it saved me from the "API mismatch" error.
2. Download and build the Flipper Zero firmware:
    ```
    $ mkdir -p ~/flipperZero/official/
    $ cd ~/flipperZero/official/
    $ git clone --recursive https://github.com/flipperdevices/flipperzero-firmware.git ./
    $ ./fbt
    ```
3. Move your app's code into `~/flipperZero/official/applications_user` folder:
    ```sh
    $ ~/flipperZero/official/flipperzero-firmware  tree applications_user
    applications_user
    ├── simple_app_1
    │   ├── application.fam
    │   ├── simple_app_1.c
    │   └── CS.png
    ├── ...
    ...
    ```
5. Build your app:

    ```sh
    $ cd ~/flipperZero/official/
    
    # build
    $ ./fbt fap_<appid>
    
    # or build and install and launch
    $ ./fbt launch_app APPSRC=<appid>
    ```

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
* [Flipper Zero Documentation](https://github.com/flipperdevices/flipperzero-firmware/tree/dev/documentation) - incomplete but still useful. Read it first.
* [Flipper Zero Example Apps](https://github.com/flipperdevices/flipperzero-firmware/tree/dev/applications/examples) - some examples for .
* [Flipper Zero Firmware Documentation](https://doxy.flipperzero.one/dev/index.html)
* ["Сборка Hello World под Flipper Zero" by Pavel Yakovlev](https://yakovlev.me/hello-flipper-zero/)
* [Flipper Hello World app](https://github.com/giolaq/helloflipper)
* [The Flipper Zero user interface editor](https://ilin.pt/stuff/fui-editor/)
* [FlipperZero_plugin_howto](https://github.com/csBlueChip/FlipperZero_plugin_howto) - detailed installation description + complicated and overcommented plugin code.
* [Your First Plugin](https://flipper.atmanos.com/docs/category/your-first-plugin) - WARNING: this one is completely outdated though is a good step by step guide for newbies. 
