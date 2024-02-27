#include <toolbox/stream/stream.h>
#include <toolbox/stream/string_stream.h>
#include <toolbox/stream/file_stream.h>
#include <toolbox/stream/buffered_file_stream.h>

// #define SIMPLE_APP_6_BYTES_PER_LINE 20u
// #define SIMPLE_APP_6_LINES_ON_SCREEN 5u
// #define SIMPLE_APP_6_BUF_SIZE (SIMPLE_APP_6_LINES_ON_SCREEN * SIMPLE_APP_6_BYTES_PER_LINE)

// typedef struct {
//     Stream* stream;
//     uint8_t file_bytes[SIMPLE_APP_6_LINES_ON_SCREEN][SIMPLE_APP_6_BYTES_PER_LINE];
//     uint32_t file_offset;
//     uint32_t file_read_bytes;
//     uint32_t file_size;
// } SimpleApp6FileModel;


// void simple_app_6_reader_alloc(SimpleApp6* app) {
//     app->reader = malloc(sizeof(SimpleApp6FileModel));
//     memset(app->reader, 0x0, sizeof(SimpleApp6FileModel));
// }

// void simple_app_6_reader_free(SimpleApp6* app) {
//     if(app->reader->stream) buffered_file_stream_close(app->reader->stream);
//     free(app->reader);
// }

#define SIMPLE_APP_6_BUF_SIZE 1024

struct SimpleApp6File {
    Stream* stream;

    uint32_t max_buffer_size;
    uint32_t buffer_size;  // actual buffer size

    uint8_t* buffer;
    uint32_t offset;
};

SimpleApp6File* simple_app_6_raw_file_alloc(Storage* storage);
void simple_app_6_raw_file_free(SimpleApp6File* file);
void simple_app_6_raw_file_restore(SimpleApp6File* file);
bool simple_app_6_raw_file_open_read(SimpleApp6File* file, const char* file_path);
bool simple_app_6_load_file(SimpleApp6* app);
