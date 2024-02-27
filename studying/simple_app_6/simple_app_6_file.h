#include <stream/stream.h>
#include <stream/buffered_file_stream.h>


typedef struct {
    Stream* file_stream;

} SimpleApp6FileModel


SimpleApp6FileModel* simple_app_6_file_model_alloc(Storage* storage) {
    SimpleApp6FileModel file* = malloc(sizeof(SimpleApp6FileModel));
    file->stream = buffered_file_stream_alloc(storage);

    FURI_LOG_D(TAG, "Initialising file model");
    FURI_LOG_D(TAG, "Cache data size: %d", (BufferedFileStream*)file->stream->cache->data_size);
    FURI_LOG_D(TAG, "Cache position: %d", (BufferedFileStream*)file->stream->cache->position);

    return file;
}

void simple_app_6_file_model_free(SimpleApp6FileModel* file) {
    // if(file->buffer) {
    //     free(file->buffer);
    // }
    stream_free(file->stream);
    free(file);
}


void simple_app_6_file_open_file(Storage* stotage) {
    SimpleApp6FileModel file* = simple_app_6_file_model_alloc(storage);
    
}


void simple_app_6_file_model_read_existing(SimpleApp6FileModel* file, const char* file_path) {
    if (!buffered_file_stream_open(file->stream, file_path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FS_Error error = buffered_file_stream_get_error(file->stream);
        FURI_LOG_E(TAG, "Cannot open file: %d", error);
        buffered_file_stream_close(file->stream);
        stream_free(file->stream);
    }
}