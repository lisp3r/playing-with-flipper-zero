#include "../simple_app_6_i.h"
#include "simple_app_6_scene.h"


void simple_app_6_file_browser_callback(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    FURI_LOG_I(TAG, "File is selected. I am gonna sent event");

    view_dispatcher_send_custom_event(app->view_dispatcher, SimpleApp6EventSelectFile);
}

// bool simple_app_6_load_book(SimpleApp6* app) {
//     app->reader->stream = buffered_file_stream_alloc(app->storage);

//     bool is_ok = true;
//     if(!buffered_file_stream_open(app->reader->stream, app->file_path, FSAM_READ, FSOM_OPEN_EXISTING)) {
//         FURI_LOG_E(TAG, "Unable to open stream: %s", file_path);
//         is_ok = false;
//     }
//     app->reader->file_size = stream_size(app->reader->stream);
//     return is_ok;
// }


void simple_app_6_scene_file_browser_on_enter(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    file_browser_set_callback(app->file_browser, simple_app_6_file_browser_callback, app);

    file_browser_start(app->file_browser, app->file_path);
    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp6ViewFileBrowser);
}

bool simple_app_6_scene_file_browser_on_event(void* context, SceneManagerEvent event) {
    // UNUSED(context);
    bool consumed = false;
    SimpleApp6* app = context;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SimpleApp6EventSelectFile) {
            FURI_LOG_I(TAG, "Selected file: %s", furi_string_get_cstr(app->file_path));

            scene_manager_next_scene(app->scene_manager, SimpleApp6SceneRead);

            consumed = true;
        }
    }
    return consumed;
}

void simple_app_6_scene_file_browser_on_exit(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    file_browser_stop(app->file_browser);
}