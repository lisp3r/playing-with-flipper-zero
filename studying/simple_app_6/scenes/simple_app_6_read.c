#include "../simple_app_6_i.h"
#include "simple_app_6_scene.h"


// bool simple_app_6_load_book(SimpleApp6* app) {
//     app->reader->stream = file_stream_alloc(app->storage);  // buffered_file_stream_alloc(app->storage);

//     bool is_ok = true;

//     do {
//         if(!file_stream_open(app->reader->stream, furi_string_get_cstr(app->file_path), FSAM_READ, FSOM_OPEN_EXISTING)) {
//             FURI_LOG_E(TAG, "Unable to open stream: %s", furi_string_get_cstr(app->file_path));
//             is_ok = false;
//             break;
//         }
//         app->reader->file_size = stream_size(app->reader->stream);
//     } while(false);
//     return is_ok;
// }


// static bool simple_app_6_read_file(SimpleApp6* app) {
//     furi_assert(app);
//     furi_assert(app->reader->stream);
//     furi_assert(app->reader->file_offset % SIMPLE_APP_6_BYTES_PER_LINE == 0);

//     memset(app->reader->file_bytes, 0x0, SIMPLE_APP_6_BUF_SIZE); // ???
//     bool is_ok = true;

//     do {
//         uint32_t offset = app->reader->file_offset;
//         if(!stream_seek(app->reader->stream, offset, true)) {
//             FURI_LOG_E(TAG, "Unable to seek stream");
//             is_ok = false;
//             break;
//         }

//         app->reader->file_read_bytes = stream_read(app->reader->stream, (uint8_t*)app->reader->file_bytes, SIMPLE_APP_6_BUF_SIZE);
//     } while(false);

//     return is_ok;
// }

void simple_app_6_writing_to_config_file(SimpleApp6* app, FuriString *data) {
    // See: https://github.com/flipperdevices/flipperzero-firmware/blob/c924693a84abe88a6c53e1e3b062f0a9ab1c5886/lib/flipper_format/flipper_format.h
    // https://github.com/flipperdevices/flipperzero-firmware/blob/c924693a84abe88a6c53e1e3b062f0a9ab1c5886/lib/lfrfid/lfrfid_dict_file.c#L143

    // UNUSED(app);

    // Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* file = flipper_format_file_alloc(app->storage);

    const uint32_t version = 1;

    do {
        if(!flipper_format_file_open_new(file, APP_ASSETS_PATH(".simple_app_6_data"))) break;
        if(!flipper_format_write_header_cstr(file, "Simple App 6 config file", version)) break;
        if(!flipper_format_write_comment_cstr(file, "kekekeke")) break;
        if(!flipper_format_write_string_cstr(file, "Last opened file", furi_string_get_cstr(data))) break;

        // signal that the file was written successfully
    } while(0);

    flipper_format_free(file);
    // furi_record_close(RECORD_STORAGE);
}


void simple_app_6_scene_read_on_enter(void* context) {
    SimpleApp6* app = context;

    // simple_app_6_writing_to_config_file(app, app->file_path);

    

    bool is_ok = simple_app_6_load_book(app);
    if(is_ok) {
        simple_app_6_read_file(app);
        text_box_set_text(app->text_box, (const char*)app->reader->file_bytes);
    }

    // memset(app->reader->file_bytes, 0x0, SIMPLE_APP_6_BUF_SIZE); // ???

    // if(is_ok) {
    //     uint32_t file_read_bytes = stream_read(app->reader->stream,  (uint8_t*)app->reader->file_bytes, SIMPLE_APP_6_BUF_SIZE);
    //     text_box_set_text(app->text_box, (const char*)file_read_bytes);
    // }

    // file_stream_close(stream);
    // stream_free(stream);

    // text_box_set_text(app->text_box, furi_string_get_cstr(app->file_path));
    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp6SceneRead);
}

bool simple_app_6_scene_read_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);

    bool consumed = false;

    return consumed;
}

void simple_app_6_scene_read_on_exit(void* context) {
    SimpleApp6* app = context;
    text_box_reset(app->text_box);
}