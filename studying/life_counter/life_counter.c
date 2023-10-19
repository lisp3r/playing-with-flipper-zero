#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>


#define START_LIVES 40;
#define TAG "mtg_life_counter"


typedef struct {
    Gui* gui;
    ViewPort* view_port;
    FuriMutex** mutex;
    FuriMessageQueue* event_queue;

    int counter;
} LifeCounterApp;


static void life_counter_draw_callback(Canvas* const canvas, void* ctx) {
    LifeCounterApp* app = ctx;

    furi_check(furi_mutex_acquire(app->mutex, FuriWaitForever) == FuriStatusOk);
    canvas_clear(canvas);

    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "Counter :)");
    
    if(app->counter == 0) {
        canvas_draw_str(canvas, 37, 31, "Death");
    } else if(app->counter > 0) {
        char tmp[11];
        snprintf(tmp, sizeof(tmp), "%d", app->counter);
        canvas_draw_str(canvas, 37, 31, tmp);
    }

    furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
}


static void life_counter_input_callback(InputEvent* input_event, void* ctx) {
    LifeCounterApp* app = ctx;
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

    app->counter = START_LIVES;
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
                app->counter++;
            } else if (input.key == InputKeyDown) {
                app->counter--;
            }
            furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
            view_port_update(app->view_port);
        }
    }
    furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
    life_counter_free(app);
    return 0;
}