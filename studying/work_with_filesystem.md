# Work with filesystem

To work with files and directories on Flipper Zero you need to use the next sources:

- `applications/services/storage/` - base functions for working with storage and files
- `lib/toolbox/stream/` - streams (FileStream, BufferedFileStream) incorporating storage functionality

## `File` objet ([`filesystem_api_internal.h`](https://github.com/flipperdevices/flipperzero-firmware/blob/dev/applications/services/storage/filesystem_api_internal.h))

```c
struct File {
    uint32_t file_id;
    FileType type;
    FS_Error error_id;
    int32_t internal_error_id;
    void* storage;
};
```

The functions working with `File` objects are located in  [`storage.h`](https://github.com/flipperdevices/flipperzero-firmware/blob/dev/applications/services/storage/storage.h).

## Allocate and free a `File` object

```c
File* storage_file_alloc(Storage* storage);
void storage_file_free(File* file);
```

## File functions

### Get file information

```c
bool storage_file_is_open(File* file);
bool storage_file_is_dir(File* file);
uint64_t storage_file_size(File* file);
bool storage_file_exists(Storage* storage, const char* path);
```

### Create, open and close a file

Opens an existing file or create a new one:

```c
typedef enum {
    FSOM_OPEN_EXISTING = 1,  // Open file, fail if file doesn't exist
    FSOM_OPEN_ALWAYS   = 2,  // Open file. Create new file if not exist
    FSOM_OPEN_APPEND   = 4,  // Open file. Create new file if not exist. Set R/W pointer to EOF
    FSOM_CREATE_NEW    = 8,  // Creates a new file. Fails if the file is exist
    FSOM_CREATE_ALWAYS = 16, // Creates a new file. If file exist, truncate to zero size
} FS_OpenMode;

bool storage_file_open(File* file, const char* path, FS_AccessMode access_mode, FS_OpenMode open_mode);
bool storage_file_close(File* file);
```

### Read bytes from a file into a buffer

```c
uint16_t storage_file_read(File* file, void* buff, uint16_t bytes_to_read);
```

## Write bytes from a buffer to a file

```c
uint16_t storage_file_write(File* file, const void* buff, uint16_t bytes_to_write);
```

### Moves the r/w pointer

```c
bool storage_file_seek(File* file, uint32_t offset, bool from_start);
```

### Gets the position of the r/w pointer

```c
uint64_t storage_file_tell(File* file);
```

### Truncates the file size to the current position of the r/w pointer

```c
bool storage_file_truncate(File* file);
```

### Writes file cache to storage

```c
bool storage_file_sync(File* file);
```

### Checks that the r/w pointer is at the end of the file

```c
bool storage_file_eof(File* file);
```

## Dirs fnctions

### Is the dir exists

```c
bool storage_dir_exists(Storage* storage, const char* path);
```

### Opens a directory to get objects from it, closes a directory

```c
bool storage_dir_open(File* file, const char* path);
bool storage_dir_close(File* file);
```

### Reads the next object in the directory

```c
bool storage_dir_read(File* file, FileInfo* fileinfo, char* name, uint16_t name_length);
```

### Rewinds the read pointer to first item in the directory

```c
bool storage_dir_rewind(File* file);
```

## Common functions

For many of this functions the file/dir must not be open

### Retrieves unix timestamp of last access

```c
FS_Error storage_common_timestamp(Storage* storage, const char* path, uint32_t* timestamp);
```

### Retrieves information about a file/directory
```c
FS_Error storage_common_stat(Storage* storage, const char* path, FileInfo* fileinfo);
```

### Removes a file/directory from the repository

The directory must be empty and the file/directory must not be open

```c
FS_Error storage_common_remove(Storage* storage, const char* path);
```

### Renames file/directory

File/directory must not be open. Will overwrite existing file.

```c
FS_Error storage_common_rename(Storage* storage, const char* old_path, const char* new_path);
```

### Copy file

File must not be open.

```c
FS_Error storage_common_copy(Storage* storage, const char* old_path, const char* new_path);
```

### Copy one folder contents into another with rename of all conflicting files

```c
FS_Error storage_common_merge(Storage* storage, const char* old_path, const char* new_path);
```

### Creates a directory

```c
FS_Error storage_common_mkdir(Storage* storage, const char* path);
```

### Gets general information about the storage

```
FS_Error storage_common_fs_info(Storage* storage, const char* fs_path, uint64_t* total_space, uint64_t* free_space);
```

### Parse aliases in path and replace them with real path

Also will create special folders if they are not exist

```c
void storage_common_resolve_path_and_ensure_app_directory(Storage* storage, FuriString* path);
```

### Move content of one folder to another, with rename of all conflicting files. 

Source folder will be deleted if the migration is successful.

```c
FS_Error storage_common_migrate(Storage* storage, const char* source, const char* dest);
```

### Check that file or dir exists

```c
bool storage_common_exists(Storage* storage, const char* path);
```

## Error Functions

### Retrieves the error text from the error id

```c
const char* storage_error_get_desc(FS_Error error_id);
```

### Retrieves the error id from the file object

```c
FS_Error storage_file_get_error(File* file);
```

### Retrieves the internal (storage-specific) error id from the file object

```c
int32_t storage_file_get_internal_error(File* file);
```

### Retrieves the error text from the file object

```c
const char* storage_file_get_error_desc(File* file);
```

### Removes a file/directory

The directory must be empty and the file/directory must not be open

```c
bool storage_simply_remove(Storage* storage, const char* path);
```

### Recursively removes a file/directory

The directory can be not empty

```c
bool storage_simply_remove_recursive(Storage* storage, const char* path);
```

### Creates a directory

```c
bool storage_simply_mkdir(Storage* storage, const char* path);
```

### Get next free filename

```c
void storage_get_next_filename(Storage* storage, const char* dirname, const char* filename, const char* fileextension, FuriString* nextfilename, uint8_t max_len);
```

## Streams

### `Stream` object and functions from `lib/toolbox/stream/stream.h`

```c
struct Stream {
    const StreamVTable* vtable;
};

void stream_free(Stream* stream);
void stream_clean(Stream* stream);
```

#### Gets the size of the stream

```c
size_t stream_size(Stream* stream);
```

#### Is the RW pointer is at the end of the stream

```c
bool stream_eof(Stream* stream);
```

#### Move the RW pointer

```c
bool stream_seek(Stream* stream, int32_t offset, StreamOffset offset_type);
```

#### Moves the RW pointer to the start

```c
bool stream_rewind(Stream* stream);
```

#### Gets the value of the RW pointer

```c
size_t stream_tell(Stream* stream);
```

#### Read from stream

Read N bytes from stream:

```c
size_t stream_read(Stream* stream, uint8_t* data, size_t count);
```

Read line from a stream (supports LF and CRLF line endings):

```c
bool stream_read_line(Stream* stream, FuriString* str_result);
```

#### Seek to next occurrence of the character

```c
bool stream_seek_to_char(Stream* stream, char c, StreamDirection direction);
```

#### Write to the stream

```c
// Write N bytes to the stream
size_t stream_write(Stream* stream, const uint8_t* data, size_t size);

// Write char to the stream
size_t stream_write_char(Stream* stream, char c);

// Write string to the stream
size_t stream_write_string(Stream* stream, FuriString* string);

// Write const char* to the stream
size_t stream_write_cstring(Stream* stream, const char* string);

// Write formatted string to the stream
size_t stream_write_format(Stream* stream, const char* format, ...)

// Write formatted string to the stream, va_list version
size_t stream_write_vaformat(Stream* stream, const char* format, va_list args);
```

#### Insert chars to the stream

```c
// Insert N chars to the stream, starting at the current pointer
bool stream_insert(Stream* stream, const uint8_t* data, size_t size);

// Insert char to the stream
bool stream_insert_char(Stream* stream, char c);

// Insert string to the stream
bool stream_insert_string(Stream* stream, FuriString* string);

// Insert const char* to the stream
bool stream_insert_cstring(Stream* stream, const char* string);

// Insert formatted string to the stream
bool stream_insert_format(Stream* stream, const char* format, ...)

// Insert formatted string to the stream, va_list version:
bool stream_insert_vaformat(Stream* stream, const char* format, va_list args);
```

#### Delete from the stream

```c
// Delete N chars from the stream and write data by calling write_callback(context)
bool stream_delete_and_insert(Stream* stream, size_t delete_size, StreamWriteCB write_callback, const void* context);

// Delete N chars from the stream and insert char to the stream
bool stream_delete_and_insert_char(Stream* stream, size_t delete_size, char c);

// Delete N chars from the stream and insert string to the stream
bool stream_delete_and_insert_string(Stream* stream, size_t delete_size, FuriString* string);

// Delete N chars from the stream and insert const char* to the stream:
bool stream_delete_and_insert_cstring(Stream* stream, size_t delete_size, const char* string);

// Delete N chars from the stream and insert formatted string to the stream:
bool stream_delete_and_insert_format(Stream* stream, size_t delete_size, const char* format, ...)

// Delete N chars from the stream and insert formatted string to the stream, va_list version:
bool stream_delete_and_insert_vaformat(Stream* stream, size_t delete_size, const char* format, va_list args);

// Remove N chars from the stream, starting at the current pointer. The size may be larger than stream size, the stream will be cleared from current RW pointer to the end.
bool stream_delete(Stream* stream, size_t size);
```

#### Copy data from one stream to another

```c
// Copy data from one stream to another. Data will be copied from current RW pointer and to current RW pointer.
size_t stream_copy(Stream* stream_from, Stream* stream_to, size_t size);

// Copy data from one stream to another. Data will be copied from start of one stream and to start of other stream.
size_t stream_copy_full(Stream* stream_from, Stream* stream_to);
```

#### Splits one stream into two others. The original stream will remain untouched.

```c
bool stream_split(Stream* stream, Stream* stream_left, Stream* stream_right);
```

#### Loads data to the stream from a file. Data will be loaded to the current RW pointer. RW pointer will be moved to the end of the stream.

```c
size_t stream_load_from_file(Stream* stream, Storage* storage, const char* path);
```

#### Writes data from a stream to a file. Data will be saved starting from the current RW pointer. RW pointer will be moved to the end of the stream.

```c
size_t stream_save_to_file(Stream* stream, Storage* storage, const char* path, FS_OpenMode mode);
```

#### Dump stream inner data (size, RW position, content)

```c
void stream_dump_data(Stream* stream);
```
