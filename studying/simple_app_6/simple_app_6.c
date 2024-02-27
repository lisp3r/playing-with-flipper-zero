#include "simple_app_6_i.h"

#include <simple_app_6_icons.h>


bool simple_app_6_back_event_callback(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    return scene_manager_handle_back_event(app->scene_manager);
}

bool simple_app_6_custom_callback(void* context, uint32_t custom_event) {
    furi_assert(context);
    SimpleApp6* app = context;

    return scene_manager_handle_custom_event(app->scene_manager, custom_event);
}

// Scene Manager alloc and free

void simple_app_6_scene_manager_init(SimpleApp6* app) {
    app->scene_manager = scene_manager_alloc(&simple_app_6_scene_event_handlers, app);
}

void simple_app_6_scene_manager_free(SimpleApp6* app) {
    scene_manager_free(app->scene_manager);
}

// View Dispatcher alloc and free

void simple_app_6_view_dispatcher_init(SimpleApp6* app) {
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);

    // Register our calbacks
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, simple_app_6_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, simple_app_6_back_event_callback);
    
    // Register views
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp6ViewMenu, submenu_get_view(app->submenu));
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp6ViewFileBrowser, file_browser_get_view(app->file_browser));
    view_dispatcher_add_view(app->view_dispatcher, SimpleApp6ViewReadFile, text_box_get_view(app->text_box));
}

void simple_app_6_view_dispatcher_free(SimpleApp6* app) {
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp6ViewMenu);
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp6ViewFileBrowser);
    view_dispatcher_remove_view(app->view_dispatcher, SimpleApp6ViewReadFile);

    view_dispatcher_free(app->view_dispatcher);
}


// Modules alloc and free

void simple_app_6_modules_alloc(SimpleApp6* app) {
    app->submenu = submenu_alloc();
    app->file_path = furi_string_alloc_set_str(STORAGE_APP_ASSETS_PATH_PREFIX);  // assets path: /assets

    app->file_browser = file_browser_alloc(app->file_path);
    file_browser_configure(app->file_browser, "*", NULL, true, false, NULL, true);

    app->text_box = text_box_alloc();
    app->storage = furi_record_open(RECORD_STORAGE);
}

void simple_app_6_modules_free(SimpleApp6* app) {
    submenu_free(app->submenu);
    file_browser_free(app->file_browser);
    furi_string_free(app->file_path);
    text_box_free(app->text_box);

    furi_record_close(RECORD_STORAGE);
}


// Main app alloc and free

SimpleApp6* simple_app_6_alloc() {
    FURI_LOG_D(TAG, "app init");
    SimpleApp6* app = malloc(sizeof(SimpleApp6));

    FURI_LOG_D(TAG, "scene manager init");
    simple_app_6_scene_manager_init(app);

    FURI_LOG_D(TAG, "modules init");
    simple_app_6_modules_alloc(app);

    FURI_LOG_D(TAG, "view dispatcher init");
    simple_app_6_view_dispatcher_init(app);

    return app;
}

void simple_app_6_free(SimpleApp6* app) {
    FURI_LOG_D(TAG, "scene manager free");
    simple_app_6_scene_manager_free(app);

    FURI_LOG_D(TAG, "modules free");
    simple_app_6_modules_free(app);

    FURI_LOG_D(TAG, "view dispatcher free");
    simple_app_6_view_dispatcher_free(app);

    free(app);
}

// Entrypoint

int32_t simple_app_6_app(void* p) {
    UNUSED(p);

    FURI_LOG_D(TAG, "Starting app...");

    SimpleApp6* app = simple_app_6_alloc();

    // set the scene and launch the main loop
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);
    scene_manager_next_scene(app->scene_manager, SimpleApp6SceneMenu);
    FURI_LOG_D(TAG, "Starting dispatcher...");
    view_dispatcher_run(app->view_dispatcher);

    // free all memory
    FURI_LOG_I(TAG, "Test app finishing...");
    furi_record_close(RECORD_GUI);
    simple_app_6_free(app);
    return 0;
}