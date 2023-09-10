#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/popup.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>

#include "scenes/simple_app_5_scene.h"

#define TAG "simple_app_5"


typedef struct {
    SceneManager* scene_manager;
    ViewDispatcher* view_dispatcher;

    // Modules
    Popup* popup;
    DialogEx* dialog;
    Submenu* submenu;
} SimpleApp5;


// Views

typedef enum {
    SimpleApp5ViewMenu,
    SimpleApp5ViewPopup,
    SimpleApp5ViewDialog,
} SimpleApp5View;