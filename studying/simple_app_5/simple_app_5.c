#include "simple_app_5_i.h"


// void simple_app_5_app_set_log_level() {
// #ifdef FURI_DEBUG
//     furi_log_set_level(FuriLogLevelTrace);
// #else
//     furi_log_set_level(FuriLogLevelInfo);
// #endif
// }


// Default application callbacks for View Dispatcher
// see: https://github.com/jamisonderek/flipper-zero-tutorials/wiki/User-Interface#scenemanager-with-viewdispatcher

bool simple_app_5_back_event_callback(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    return scene_manager_handle_back_event(app->scene_manager);
}

bool simple_app_5_custom_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    SimpleApp5* app = context;

    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}


// Scene Manager alloc and free

void simple_app_5_scene_manager_init(SimpleApp5* app) {
    // `simple_app_5_scene_event_handlers` is defined in scenes/simple_app_scene.h
    app->scene_manager = scene_manager_alloc(&simple_app_5_scene_event_handlers, app);
}

void simple_app_5_scene_manager_free(SimpleApp5* app) {
    scene_manager_free(app->scene_manager);
}

// View Dispatcher alloc and free

void simple_app_5_view_dispatcher_init(SimpleApp5* app) {
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);

    // Register our calbacks
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, simple_app_5_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, simple_app_5_back_event_callback);

    // Allocate modules
    app->submenu = submenu_alloc();     // <gui/modules/submenu.h>
    app->popup = popup_alloc();         // <gui/modules/popup.h>
    app->dialog = dialog_ex_alloc();    // ...
    app->settings = variable_item_list_alloc(); // ...

    // Register views
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp5ViewMenu, submenu_get_view(app->submenu));     // submenu_get_view() is from <gui/modules/submenu.h>
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp5ViewPopup, popup_get_view(app->popup));        // popup_get_view() is from <gui/modules/popup.h>
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp5ViewDialog, dialog_ex_get_view(app->dialog));  // ...
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp5ViewSettings, variable_item_list_get_view(app->settings)); // ...
}

void simple_app_5_view_dispatcher_free(SimpleApp5* app) {
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp5ViewMenu);
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp5ViewPopup);
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp5ViewDialog);
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp5ViewSettings);
    view_dispatcher_free(app->view_dispatcher);

    submenu_free(app->submenu);
    popup_free(app->popup);
    dialog_ex_free(app->dialog);
    variable_item_list_free(app->settings);
}

// Main app alloc and free

SimpleApp5* simple_app_5_alloc() {
    FURI_LOG_D(TAG, "app init");
    SimpleApp5* app = malloc(sizeof(SimpleApp5));

    FURI_LOG_D(TAG, "scene manager init");
    simple_app_5_scene_manager_init(app);

    FURI_LOG_D(TAG, "view dispatcher init");
    simple_app_5_view_dispatcher_init(app);

    app->mode = SimpleApp5SettingsMode1;

    return app;
}

void simple_app_5_free(SimpleApp5* app) {
    FURI_LOG_D(TAG, "scene manager free");
    simple_app_5_scene_manager_free(app);

    FURI_LOG_D(TAG, "view dispatcher free");
    simple_app_5_view_dispatcher_free(app);

    FURI_LOG_D(TAG, "app free");
    free(app);
}

// Entrypoint

int32_t simple_app_5_app(void* p) {
    UNUSED(p);

    // simple_app_5_app_set_log_level();

    FURI_LOG_D(TAG, "Starting app...");

    SimpleApp5* app = simple_app_5_alloc();

    // set the scene and launch the main loop
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, SimpleApp5SceneMenu);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    // free all memory
    FURI_LOG_I(TAG, "Test app finishing...");
    furi_record_close(RECORD_GUI);
    simple_app_5_free(app);
    return 0;
}