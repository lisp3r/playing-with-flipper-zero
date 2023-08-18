#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <string.h>

#include "simple_app_3_icons.h"

#define TAG "simple_app_3"


typedef struct {
    uint8_t x, y;
} ImgPosition;

typedef struct {
    bool btn_down_big;
    bool btn_up_big;
    bool btn_left_big;
    bool btn_right_big;
} Interface;

static void update_interface(Interface* interface, bool down, bool up, bool left, bool right) {
    interface->btn_down_big = down;
    interface->btn_up_big = up;
    interface->btn_left_big = left;
    interface->btn_right_big = right;
}

typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;

    ImgPosition* img_position;
    Interface* interface;
} App;

static void draw_callback(Canvas* canvas, void* ctx) {
    App* app = ctx;

    canvas_clear(canvas);
    canvas_draw_icon(canvas, app->img_position->x, app->img_position->y, &I_airplane);
    // canvas_draw_bitmap(canvas, image_position.x, image_position.y, 32, 32, icon_get_data(&I_airplane));

    if(app->interface->btn_down_big) {
        canvas_draw_icon(canvas, 118, 57, &I_ButtonDown_7x4);
    } else {
        canvas_draw_icon(canvas, 118, 57, &I_ButtonDownSmall_5x3);
    }

    if(app->interface->btn_up_big) {
        canvas_draw_icon(canvas, 111, 57, &I_ButtonUp_7x4);
    } else {
        canvas_draw_icon(canvas, 111, 57, &I_ButtonUpSmall_5x3);
    }

    if(app->interface->btn_left_big) {
        canvas_draw_icon(canvas, 100, 56, &I_ButtonLeft_4x7);
    } else {
        canvas_draw_icon(canvas, 100, 56, &I_ButtonLeftSmall_3x5);
    }

    if(app->interface->btn_right_big) {
       canvas_draw_icon(canvas, 106, 56, &I_ButtonRight_4x7);
    } else {
        canvas_draw_icon(canvas, 106, 56, &I_ButtonRightSmall_3x5);
    }

    char temp_str[7];
    snprintf(temp_str, 7, "x: %d", app->img_position->x);
    canvas_draw_str(canvas, 100, 8, temp_str);

    snprintf(temp_str, 7, "y: %d", app->img_position->y);
    canvas_draw_str(canvas, 100, 16, temp_str);
}

static void input_callback(InputEvent* input_event, void* ctx) {
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

    // ...
    app->img_position = malloc(sizeof(ImgPosition));
    app->interface = malloc(sizeof(Interface));
    app->interface->btn_down_big = false;
    app->interface->btn_up_big = false;
    app->interface->btn_left_big = false;
    app->interface->btn_right_big = false;

    // register callbacks
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app->event_queue);
    return app;
}

void app_free(App* app) {
    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);

    furi_message_queue_free(app->event_queue);
    
    furi_record_close(RECORD_GUI);
    free(app);
}

int32_t simple_app_3_main(void* p) {
    UNUSED(p);

    App* app = app_alloc();
    InputEvent event;

    bool running = true;
    while(running) {
        if(furi_message_queue_get(app->event_queue, &event, 100) == FuriStatusOk) {
            if((event.type == InputTypePress) || (event.type == InputTypeRepeat)) {
                switch(event.key) {
                case InputKeyLeft:
                    update_interface(app->interface, false, false, true, false);
                    app->img_position->x -= 2;
                    if(app->img_position->x < 224 && app->img_position->x > 128) {
                        app->img_position->x = 128;
                    }
                    break;
                case InputKeyRight:
                    update_interface(app->interface, false, false, false, true);
                    app->img_position->x += 2;
                    if(app->img_position->x >= 128 && app->img_position->x < 224) {
                        app->img_position->x = 224;
                    }
                    break;
                case InputKeyUp:
                    update_interface(app->interface, false, true, false, false);
                    app->img_position->y -= 2;
                    if(app->img_position->y <= 230 && app->img_position->y > 58) {
                        app->img_position->y = 58;
                    }
                    break;
                case InputKeyDown:
                    update_interface(app->interface, true, false, false, false);
                    app->img_position->y += 2;
                    if(app->img_position->y >= 58 && app->img_position->y < 230) {
                        app->img_position->y = 230;
                    }
                    break;
                case InputKeyOk:
                    update_interface(app->interface, false, false, false, false);
                    break;
                default:
                    running = false;
                    break;
                }
            }
        }
        view_port_update(app->view_port);
    }
    app_free(app);

    return 0;
}