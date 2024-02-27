## `file_browser.h`

```c
typedef bool (*FileBrowserLoadItemCallback)(
    FuriString* path,
    void* context,
    uint8_t** icon,
    FuriString* item_name);
```

## File browser dialog (`dialogs/`)

### File browser dialog extra options

* `extension` - file extension to be offered for selection
* `base_path` - root folder path for navigation with back key
* `skip_assets` - true - do not show assets folders
* `hide_dot_files` - true - hide dot files
* `icon` - file icon pointer, `NULL` for default icon
* `hide_ext` - true - hide extensions for files
* `item_loader_callback` - callback function for providing custom icon & entry name
* `item_loader_context` - callback context

```c
typedef struct {
    const char* extension;
    const char* base_path;
    bool skip_assets;
    bool hide_dot_files;
    const Icon* icon;
    bool hide_ext;
    FileBrowserLoadItemCallback item_loader_callback;
    void* item_loader_context;
} DialogsFileBrowserOptions;
```

### Initialize file browser dialog options and set default values

```c
void dialog_file_browser_set_basic_options(DialogsFileBrowserOptions* options, const char* extension, const Icon* icon);
```

### Shows and processes the file browser dialog

```c
bool dialog_file_browser_show(DialogsApp* context, FuriString* result_path, FuriString* path, const DialogsFileBrowserOptions* options);
```

## `dialogs_api.h`

```c
bool dialog_file_browser_show(DialogsApp* context, FuriString* result_path, FuriString* path, const DialogsFileBrowserOptions* options);

DialogMessageButton dialog_message_show(DialogsApp* context, const DialogMessage* dialog_message);

void dialog_message_show_storage_error(DialogsApp* context, const char* error_text);
```



## How to use

### `DialogsFileBrowserOptions` init

```c
DialogsFileBrowserOptions browser_options;
dialog_file_browser_set_basic_options(&browser_options, "*", NULL);
browser_options.hide_ext = false;
browser_options.hide_dot_files = false;
```