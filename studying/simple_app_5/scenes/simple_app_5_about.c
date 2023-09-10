#include "../simple_app_5_i.h"
#include "simple_app_5_scene.h"


static void simple_app_5_scene_about_dialog_callback(DialogExResult result, void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    view_dispatcher_send_custom_event(app->view_dispatcher, result);
}


void simple_app_5_scene_about_on_enter(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    dialog_ex_set_context(app->dialog, context);
    dialog_ex_set_result_callback(app->dialog, simple_app_5_scene_about_dialog_callback);

    dialog_ex_set_text(app->dialog, "(c) lisp3r", 64, 11, AlignCenter, AlignTop);
    dialog_ex_set_left_button_text(app->dialog, "Back");

    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp5ViewDialog);
}

bool simple_app_5_scene_about_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    SimpleApp5* app = context;

    bool consumed = false;

    // getting event from `simple_app_5_scene_about_dialog_callback`
    // we are looking for left button pressed event
    if(event.type == SceneManagerEventTypeCustom && event.event == DialogExResultLeft) {
        consumed = scene_manager_previous_scene(app->scene_manager);
    }
    // todo: what is gonna heppen when BACK is pressed?

    return consumed;
}

void simple_app_5_scene_about_on_exit(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    dialog_ex_reset(app->dialog);
}