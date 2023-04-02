#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>
#include <string.h>

#include "simple_app_2_icons.h"

#define TAG "simple_app_2"


static void log_events(InputEvent* input_event) {

    char* evt_key = "UnknownEvent";
    char* evt_type = "UnknownType";

    switch(input_event->key) {
        case InputKeyBack:
            evt_key = "InputKeyBack";
            break;
        case InputKeyUp:
            evt_key = "InputKeyUp";
            break;
        case InputKeyDown:
            evt_key = "InputKeyDown";
            break;
        case InputKeyRight:
            evt_key = "InputKeyRight";
            break;
        case InputKeyLeft:
            evt_key = "InputKeyLeft";
            break;
        case InputKeyOk:
            evt_key = "InputKeyOk";
            break;
        case InputKeyMAX:
            evt_key = "InputKeyMAX";
            break;
    }

    switch(input_event->type) {
        case InputTypePress:
            evt_type = "InputTypePress";
            break;
        case InputTypeRelease:
            evt_type = "InputTypeRelease";
            break;
        case InputTypeShort:
            evt_type = "InputTypeShort";
            break;
        case InputTypeLong:
            evt_type = "InputTypeLong";
            break;
        case InputTypeRepeat:
            evt_type = "InputTypeRepeat";
            break;
        case InputTypeMAX:
            evt_type = "InputTypeMAX";
            break;
    }

    FURI_LOG_I(TAG, "New event: %s (%s)", evt_key, evt_type);
}


typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
} App;

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "Hello, good traveller");

    canvas_draw_icon(canvas, 116, 54, &I_badusb_10px);
}

static void input_callback(InputEvent* input_event, void* ctx) {
    // check that the context is not null
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;

    log_events(input_event);

    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}

App* app_alloc() {
    App* app = malloc(sizeof(App));

    app->view_port = view_port_alloc();
    app->gui = furi_record_open(RECORD_GUI);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    // register callbacks
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app->event_queue);
    return app;
}

void app_free(App* app) {
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);

    furi_message_queue_free(app->event_queue);
    
    furi_record_close(RECORD_GUI);
    free(app);
}

int32_t simple_app_2_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();

    InputEvent event;

    while(1) {
        furi_check(furi_message_queue_get(app->event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if(event.key == InputKeyBack) {
            break;
        }
    }

    app_free(app);

    return 0;
}
