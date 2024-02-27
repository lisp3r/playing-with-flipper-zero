#pragma once

#include <gui/scene_manager.h>


// Generate scene id and total number
#define ADD_SCENE(prefix, name, id) SimpleApp6Scene##id,  // scene id prefix
typedef enum {
#include "simple_app_6_scene_config.h"
    SimpleApp6SceneCount, // it is nesessery to have the lase scene `SimpleApp6SceneCount`
} SimpleApp6Scene;
#undef ADD_SCENE

extern const SceneManagerHandlers simple_app_6_scene_event_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "simple_app_6_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "simple_app_6_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "simple_app_6_scene_config.h"
#undef ADD_SCENE
