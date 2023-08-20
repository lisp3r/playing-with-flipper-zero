#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <storage/storage.h>
#include <toolbox/stream/stream.h>
#include <toolbox/stream/file_stream.h>


#define TAG "simple_app_4"


typedef struct AssetsStorage {
    Storage* storage;
    int max_len;
    char** files;
    char** dirs;
    uint16_t files_len;
    uint16_t dirs_len;
} AssetsStorage;

AssetsStorage* assets_storage_allock() {
    AssetsStorage* assets_storage = malloc(sizeof(AssetsStorage));

    // Storage open
    assets_storage->storage = furi_record_open(RECORD_STORAGE); 
    assets_storage->max_len = 200; // too small actually

    assets_storage->files = (char**)malloc(sizeof(char) * (assets_storage->max_len + 1));
    assets_storage->dirs = (char**)malloc(sizeof(char) * (assets_storage->max_len + 1));
    assets_storage->files_len = 0;
    assets_storage->dirs_len = 0;

    return assets_storage;
}

void assets_storage_free(AssetsStorage* assets_storage) {
    // Close storage
    furi_record_close(RECORD_STORAGE);
    free(assets_storage->files);
    free(assets_storage->dirs);
}


typedef struct SimpleApp4 {
    Gui* gui;
    ViewPort* view_port;
    FuriMessageQueue* event_queue;
    AssetsStorage* assets_storage;
} SimpleApp4;


static void draw_callback(Canvas* canvas, void* ctx) {
    SimpleApp4 *app = ctx;

    canvas_clear(canvas);
    canvas_draw_str(canvas, 2, 10, "Files:");

    int y_line = 20;

    for (int i = 0; i < app->assets_storage->files_len; i++) {
        canvas_draw_str(canvas, 2, y_line, app->assets_storage->files[i]);
        y_line += 10;
    }

    canvas_draw_str(canvas, 2, y_line+5, "Dirs:");
    y_line += 15;
    for (int i = 0; i < app->assets_storage->dirs_len; i++) {
        canvas_draw_str(canvas, 2, y_line, app->assets_storage->dirs[i]);
        y_line += 10;
    }
}

static void input_callback(InputEvent* input_event, void* ctx) {
    furi_assert(ctx);
    FuriMessageQueue* event_queue = ctx;
    furi_message_queue_put(event_queue, input_event, FuriWaitForever);
}


SimpleApp4* app_alloc() {
    SimpleApp4* app = malloc(sizeof(SimpleApp4));
    app->view_port = view_port_alloc();
    app->gui = furi_record_open(RECORD_GUI);
    app->event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    gui_add_view_port(app->gui, app->view_port, GuiLayerFullscreen);

    app->assets_storage = assets_storage_allock();

    // register callbacks
    view_port_draw_callback_set(app->view_port, draw_callback, app);
    view_port_input_callback_set(app->view_port, input_callback, app->event_queue);
    return app;
}

void app_free(SimpleApp4* app) {
    view_port_enabled_set(app->view_port, false);
    gui_remove_view_port(app->gui, app->view_port);
    view_port_free(app->view_port);

    assets_storage_free(app->assets_storage);

    furi_message_queue_free(app->event_queue);
    furi_record_close(RECORD_GUI);
    free(app);
}


void get_dir_content(SimpleApp4* app, char* dir_path) {
    File* assets_dir = storage_file_alloc(app->assets_storage->storage);
    FURI_LOG_I(TAG, "Opening \"%s\"...", dir_path);

    if (storage_dir_open(assets_dir, dir_path)) {

        FURI_LOG_I(TAG, "\"%s\"... sucessfully opened", dir_path);
        
        FileInfo file_info;
        // for the name of the next found file
        char* name_buff = malloc(app->assets_storage->max_len * sizeof(char));
        uint16_t buf_len = 0;

        // Read all the files in a folder
        while(true) {
            if (storage_dir_read(assets_dir, &file_info, name_buff, app->assets_storage->max_len)) {
                if(file_info_is_dir(&file_info)) {
                    FURI_LOG_I(TAG, "Directory found: \"%s\"", name_buff);

                    buf_len = strlen(name_buff) + 1;

                    app->assets_storage->dirs[app->assets_storage->dirs_len] = malloc(buf_len * sizeof(char));
                    memcpy(app->assets_storage->dirs[app->assets_storage->dirs_len], name_buff, buf_len);
                    app->assets_storage->dirs_len++;
                } else {
                    FURI_LOG_I(TAG, "File found: \"%s\"", name_buff);

                    // Сохраняем имя файла для дальнейшего использования
                    buf_len = strlen(name_buff) + 1;

                    app->assets_storage->files[app->assets_storage->files_len] = malloc(buf_len * sizeof(char));
                    memcpy(app->assets_storage->files[app->assets_storage->files_len], name_buff, buf_len);
                    app->assets_storage->files_len++;
                }
            } else {
                FURI_LOG_E(TAG, "error while reading files from \"%s\"", dir_path);
                break;
            }
        }

        free(name_buff);

    } else {
        FURI_LOG_E(TAG, "error while openning \"%s\"", dir_path);
    }

    storage_dir_close(assets_dir);
    storage_file_free(assets_dir);
}


int32_t simple_app_4_main(void* p) {
    UNUSED(p);

    SimpleApp4* app = app_alloc();
    InputEvent event;

    get_dir_content(app, STORAGE_APP_ASSETS_PATH_PREFIX);

    while(1) {
        furi_check(furi_message_queue_get(app->event_queue, &event, FuriWaitForever) == FuriStatusOk);

        if(event.key == InputKeyBack) {
            break;
        }
    }

    app_free(app);
    return 0;
}
