#include <furi.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/widget.h>


#define START_LIVES 40
#define MAX_LIVES 200

#define TAG "mtg_life_counter"


typedef enum {
    LifeCounterEventDeath
} LifeCounterEventType;


typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Widget* widget;

    int counter;
} LifeCounterApp;


void life_counter_reset(LifeCounterApp* app) {
    app->counter = START_LIVES;
}


typedef enum {
  LifeCounterMainView,
} LifeCounterViews;


// View Dispatcher callbacks

bool life_counter_custom_event_callback(void* context, uint32_t event) {
    // event -> gui_widget_view_input_callback -> life_counter_custom_event_callback
    FURI_LOG_I(TAG, "in custom_event_callback")

    LifeCounterApp* app = context;
    // if (event == LifeCounterEventDeath) {
    //     widget_add_string_element(app->widget, 49, 40, AlignCenter, AlignTop, FontPrimary, "Death!");
    //     widget_add_button_element(app->widget, GuiButtonTypeLeft, "Again", life_counter_reset_callback, app);
    // }

    return true; 
}

bool life_counter_navigation_event_callback(void* context) {
    UNUSED(context);
    FURI_LOG_I(TAG, "in navigation_event_callback")


    return true;
}

// void life_counter_reset_callback(GuiButtonType result, InputType type, void* context) {
//     FURI_LOG_I(TAG, "life_counter_reset_callback: resert game")
//     furi_assert(context);

//     LifeCounterApp* app = context;
//     // if(type == InputTypeShort) {
//     //     view_dispatcher_send_custom_event(nfc->view_dispatcher, result);
//     // }
    
//     life_counter_reset(app);
// }

// main view

// static void life_counter_main_view_draw_callback(Canvas* canvas, void* model) {
//     LifeCounterApp* app = model;
//     widget_reset(widget);

//     widget_add_string_element(widget, 21, 11, AlignCenter, AlignTop, FontPrimary, "MTG Life Counter");

//     if (app->counter == 0) {
//         widget_add_string_element(widget, 49, 40, AlignCenter, AlignTop, FontPrimary, "Death!");
//         widget_add_button_element(widget, GuiButtonTypeLeft, "Again", life_counter_reset_callback, app);
//     } else if (app->counter > 0) {
//         char tmp[11];
//         snprintf(tmp, sizeof(tmp), "%d", app->counter);
//         widget_add_string_element(widget, 49, 40, AlignCenter, AlignTop, FontPrimary, tmp);
//     }
// }


void setup_widget(LifeCounterApp* app) {
    widget_add_string_element(widget, 21, 11, AlignCenter, AlignTop, FontPrimary, "MTG Life Counter");
    // if (app->counter == 0) {
    //     widget_add_string_element(widget, 49, 40, AlignCenter, AlignTop, FontPrimary, "Death!");
    char tmp[11];
    snprintf(tmp, sizeof(tmp), "%d", app->counter);
    widget_add_string_element(widget, 49, 40, AlignCenter, AlignTop, FontPrimary, tmp);
}



LifeCounterApp* life_counter_alloc() {
    LifeCounterApp* app = malloc(sizeof(LifeCounterApp));
    life_counter_reset(app);

    // view dispatcher

    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(view_dispatcher);

    view_dispatcher_set_event_callback_context(view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(view_dispatcher, life_counter_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(view_dispatcher, life_counter_navigation_event_callback);
    // set a custom callback: `view_dispatcher_send_custom_event method`

    // setup widget
    app->widget = widget_alloc();
    setup_widget(app);

    view_dispatcher_add_view(app->view_dispatcher, LifeCounterMainView, widget_get_view(app->widget));

    app->gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    view_dispatcher_switch_to_view(view_dispatcher, LifeCounterMainView);
    view_dispatcher_run(view_dispatcher);
    return app;
}

void life_counter_free(LifeCounterApp* app) {
    view_dispatcher_remove_view(view_dispatcher, LifeCounterMainView);
    widget_free(app->widget);
    view_dispatcher_stop(app->view_dispatcher);
    view_dispatcher_free(app->view_dispatcher);
    furi_record_close(RECORD_GUI);    
    free(app);
}


int32_t life_counter_main(void* p) {
    UNUSED(p);

    FURI_LOG_I(TAG, "Just started the application");

    LifeCounterApp* app = life_counter_alloc();

    // bool running = true;

    // while (running) {
    //     InputEvent input;
    //     while(furi_message_queue_get(app->event_queue, &input, FuriWaitForever) == FuriStatusOk) {
    //         furi_check(furi_mutex_acquire(app->mutex, FuriWaitForever) == FuriStatusOk);

    //         if(input.key == InputKeyBack) {
    //             running = false;
    //             break;
    //         } else if (input.key == InputKeyUp) {
    //             if (app->counter >= MAX_LIVES) {
    //                 // pass
    //             } else {
    //                 app->counter++;
    //             }
    //         } else if (input.key == InputKeyDown) {
    //             if (app->counter == 0) {
    //                 // pass
    //             } else {
    //                 app->counter--;
    //             }
    //         }
    //         furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
    //         view_port_update(app->view_port);
    //     }
    // }
    // furi_check(furi_mutex_release(app->mutex) == FuriStatusOk);
    life_counter_free(app);
    return 0;
}