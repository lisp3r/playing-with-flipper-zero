#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>

#include "simple_app_2_icons.h"

#define TAG "simple_app_2"


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
    // FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    while(1) {
        // Выбираем событие из очереди в переменную event (ждем бесконечно долго, если очередь пуста)
        // и проверяем, что у нас получилось это сделать
        furi_check(furi_message_queue_get(app->event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if(event.key == InputKeyBack) {
            break;
        }
    }

    app_free(app);

    return 0;
}
