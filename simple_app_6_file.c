#include "simple_app_6_file.h"


SimpleApp6File* simple_app_6_raw_file_alloc(Storage* storage) {
    SimpleApp6File* file = malloc(sizeof(SimpleApp6File));
    file->stream = file_stream_alloc(storage);
    return simple_app_6_raw_file_restore(file);
}

void simple_app_6_raw_file_free(SimpleApp6File* file) {
    if(file->buffer) {
        free(file->buffer);
    }
    stream_free(file->stream);
    free(file);
}

void simple_app_6_raw_file_restore(SimpleApp6File* file) {
    file->buffer = NULL;
    file->max_buffer_size = SIMPLE_APP_6_BUF_SIZE;
    file->buffer_size = 0;
    file->offset = 0;
}

bool simple_app_6_raw_file_open_read(SimpleApp6File* file, const char* file_path) {
    return file_stream_open(file->stream, file_path, FSAM_READ, FSOM_OPEN_EXISTING);
}

bool simple_app_6_load_file(SimpleApp6* app) {
    simple_app_6_raw_file_alloc(app->storage);

    bool success = true;

    do {
        if(!simple_app_6_raw_file_open_read(app->file,  furi_string_get_cstr(app->file_path))) {
            FURI_LOG_E(TAG, "Unable to open stream: %s", furi_string_get_cstr(app->file_path));
            success = false;
            break;
        }
        app->file->buffer_size = stream_size(app->file->stream);
    } while(false);

    return success;
}


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

void simple_app_6_file_go_to_start(SimpleApp6File* file) {
    file->offset = 0;
}


void simple_app_6_file_go_to_end(SimpleApp6File* file) {
    // file->offset = 0;
}


bool simple_app_6_read_file(SimpleApp6File* file) {

    bool success = true;

    if ((stream_size(file->stream) - file->offset) <= 0) {
        // we are at the end of the file
        // file->offset = 0;
        FURI_LOG_E(TAG, "we are at the end of the file");
        return sussess;
    }

    do {
        if(!stream_seek(file->stream, file->offset, true)) {
            FURI_LOG_E(TAG, "Unable to seek stream");
            success = false;
            break;
        }

        file->buffer_size = stream_read(file->stream, (uint8_t*)file->buffer, SIMPLE_APP_6_BUF_SIZE);

        if (file->buffer_size != SIMPLE_APP_6_BUF_SIZE) {
            FURI_LOG_E(TAG, "file->buffer_size != SIMPLE_APP_6_BUF_SIZE");
            success = false;
            break;
        }

    } while(false);
    return success;
}
