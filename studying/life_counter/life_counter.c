#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <gui/elements.h>

#include "life_counter_icons.h"


#define START_LIVES 40
#define MAX_LIVES 200

#define TAG "mtg_life_counter"

#define PLAYER_1_X 40

#define PLAYER_1_Y 35

#define PLAYER_2_X 70

#define PLAYER_2_Y 35


typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMutex** mutex;
    FuriMessageQueue* event_queue;

    int counter1;
    int counter2;
    bool death;
} LifeCounterApp;


static void life_counter_draw_callback(Canvas* const canvas, void* ctx) {
    LifeCounterApp* app = ctx;

    furi_check(furi_mutex_acquire(app->mutex, FuriWaitForever) == FuriStatusOk);
    canvas_clear(canvas);

    elements_frame(canvas, 0, 0, 128, 64);

    //elements_bold_rounded_frame(canvas, 0, 0, 127, 63);
    // canvas_draw_icon(canvas, 20, 3, &I_button_up_16x16);
    // canvas_draw_icon(canvas, 20, 45, &I_button_down_16x16);

    elements_frame(canvas, 20, 3, 16, 16);
    elements_frame(canvas, 20, 45, 16, 16);

    canvas_draw_icon(canvas, 24, 26, &I_4_right_6x7);
    canvas_draw_icon(canvas, 24, 34, &I_4_right_6x7);

    // 16+16 = 32


    // 128 x 64
    // 64 * 2 x 64

    canvas_set_font(canvas, FontPrimary);
    // canvas_draw_str(canvas, 35, 15, "Life Counter");
    // canvas_draw_icon(canvas, 20, 10, &I_white_mana_17x17);
    // canvas_draw_icon(canvas, 40, 10, &I_blue_mana_17x17);
    // canvas_draw_icon(canvas, 60, 10, &I_green_mana_17x17);
    // canvas_draw_icon(canvas, 80, 10, &I_black_mana_17x17);


    if (app->counter1 == 0) {
        app->death = true;
        canvas_draw_str(canvas, PLAYER_1_X, PLAYER_1_Y, "Death!");
    } else if(app->counter2 == 0) {
        app->death = true;
        canvas_draw_str(canvas, PLAYER_2_X, PLAYER_2_Y, "Death!");
    } else {
        char tmp[11];
        snprintf(tmp, sizeof(tmp), "%d", app->counter1);
        canvas_draw_str(canvas, PLAYER_1_X, PLAYER_1_Y, tmp);
        snprintf(tmp, sizeof(tmp), "%d", app->counter2);
        canvas_draw_str(canvas, PLAYER_2_X, PLAYER_2_Y, tmp);
    }


    
    // if(app->counter == 0) {
    //     canvas_draw_str(canvas, 49, 40, "Death!");
    //     elements_button_left(canvas, "Again");
    // } else if(app->counter > 0) {
    //     char tmp[11];
    //     snprintf(tmp, sizeof(tmp), "%d", app->counter);
    //     canvas_draw_str(canvas, 57, 40, tmp);
    // }

    furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
}


static void life_counter_input_callback(InputEvent* input_event, void* ctx) {
    LifeCounterApp* app = ctx;
    // process short presses
    if(input_event->type == InputTypeShort) {
        furi_message_queue_put(app->event_queue, input_event, 0);
    }
}

LifeCounterApp* life_counter_alloc() {
    LifeCounterApp* app = malloc(sizeof(LifeCounterApp));
    app->view_port = view_port_alloc();
    view_port_draw_callback_set(app->view_port, life_counter_draw_callback, app);
    view_port_input_callback_set(app->view_port, life_counter_input_callback, app);

    app->gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));  // TODO: change to a custom event type

    app->counter1 = START_LIVES;
    app->counter2 = START_LIVES;
    app->death = false;
    return app;
}

void life_counter_free(LifeCounterApp* app) {
    view_port_enabled_set(app->view_port, false); // Disable ViewPort
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);

    furi_mutex_free(app->mutex);
    furi_message_queue_free(app->event_queue);

    furi_record_close(RECORD_GUI);
    free(app);
}


int32_t life_counter_main(void* p) {
    UNUSED(p);

    FURI_LOG_I(TAG, "Just started the application");

    LifeCounterApp* app = life_counter_alloc();
    bool running = true;

    while (running) {
        InputEvent input;
        while(furi_message_queue_get(app->event_queue, &input, FuriWaitForever) == FuriStatusOk) {
            furi_check(furi_mutex_acquire(app->mutex, FuriWaitForever) == FuriStatusOk);

            if(input.key == InputKeyBack) {
                running = false;
                break;
            } else if (input.key == InputKeyUp) {
                if (app->counter1 >= MAX_LIVES) {
                    // pass
                } else {
                    app->counter1++;
                }
            } else if (input.key == InputKeyDown) {
                if (app->counter1 == 0) {
                    // pass
                } else {
                    app->counter1--;
                }
            } else if (input.key == InputKeyLeft) {
                if (app->counter2 >= MAX_LIVES) {
                    // pass
                } else {
                    app->counter2++;
                }
            } else if (input.key == InputKeyRight) {
                 if (app->counter2 == 0) { // pass 
                } else {
                    app->counter2--;
                }
            }
            furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
            view_port_update(app->view_port);
        }
    }
    furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
    life_counter_free(app);
    return 0;
}