#pragma once

#include <gui/scene_manager.h>

/* Here we define the scenes of the application and their calbacks.

Insead of writing stuff like this:

    typedef enum {
        SimpleApp5SceneMain,
        SimpleApp5SceneAbout,
        SimpleApp5SceneCount
    }

    void (*const simple_app_5_scene_on_enter_handlers[])(void*) = {
        simple_app_5_scene_menu_on_enter,
        simple_app_5_scene_about_on_enter,
    };

    void (*const simple_app_5_scene_on_event_handlers[])(void*) = { ... }
    void (*const simple_app_5_scene_on_exit_handlers[])(void*) = { ... }

we let the preprocessing magic do the work.
*/

// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) SimpleApp5Scene##id,  // scene id prefix
typedef enum {
#include "simple_app_5_scene_config.h"
    SimpleApp5SceneCount, // it is nesessery to have the lase scene `SimpleApp5SceneCount`
} SimpleApp5Scene;
#undef ADD_SCENE

extern const SceneManagerHandlers simple_app_5_scene_event_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "simple_app_5_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "simple_app_5_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "simple_app_5_scene_config.h"
#undef ADD_SCENE
