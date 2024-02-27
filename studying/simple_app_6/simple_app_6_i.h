#pragma once

#include <furi.h>
#include <furi/core/log.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_box.h>
#include <gui/modules/file_browser.h>
#include <storage/storage.h>

// #include <lib/flipper_format/flipper_format.h>

#include "simple_app_6_file.h"
#include "scenes/simple_app_6_scene.h"

#define TAG "simple_app_6"

#define START_LIVES 40


typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;

    Submenu* submenu;
    TextBox* text_box;
    FileBrowser* file_browser;
    FuriString* file_path;      // path tp a choosen file will be there

    SimpleApp6File *file;
    Storage *storage;

    // FuriString* app_config_path = furi_string_alloc_set_str(APP_ASSETS_PATH(".simple_app_6_data"));
} SimpleApp6;


// Views

typedef enum {
    SimpleApp6ViewMenu,
    SimpleApp6ViewFileBrowser,
    SimpleApp6ViewReadFile
} SimpleApp6View;


// Events

typedef enum {
    SimpleApp6EventSelectFile
} SimpleApp6Events;