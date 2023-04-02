#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification_messages.h>

#include "simple_app_1_icons.h"

// tag for logs
#define TAG "simple_app_1"

typedef struct {
    Gui* gui;
    ViewPort* view_port;
} App;

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    canvas_clear(canvas);

    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "Hello, good traveller");

    canvas_draw_icon(canvas, 116, 54, &I_badusb_10px);
}

App* app_alloc() {
    App* app = malloc(sizeof(App));
    app->view_port = view_port_alloc();
    app->gui = furi_record_open(RECORD_GUI);

    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    view_port_draw_callback_set(app->view_port, draw_callback, app);
    return app;
}

void app_free(App* app) {
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);
    
    furi_record_close(RECORD_GUI);
    free(app);
}

int32_t simple_app_1_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();
    furi_delay_ms(2000);

    app_free(app);

    return 0;
}
