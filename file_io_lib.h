#include <toolbox/stream/stream.h>


struct FileModel {
    Stream* stream;

    uint32_t max_buffer_size;
    uint32_t buffer_size;       // actual buffer size

    uint8_t* buffer;
    uint32_t offset;            // ...
};

/**
 * Allocate FileModel.
 * @return FileModel* pointer to a FileModel instance
 */
FileModel* file_lib_alloc(Storage* storage) {

}

void file_lib_free(FileModel* file) {
    
}
// void file_io_lib_file_restore(FileModel* file);
// bool file_io_lib_file_open_read(FileModel* file, const char* file_path);
// bool file_io_lib_load_file(SimpleApp6* app);



bool file_lib_open(FileModel* file, const char* filename) {
    furi_assert(file);
    return buffered_file_stream_open(file->stream, filename, FSAM_READ, FSOM_OPEN_EXISTING);
}