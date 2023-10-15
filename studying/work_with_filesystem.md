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
