#include "simple_app_5_scene.h"

// Generate scene on_enter handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_enter,
void (*const simple_app_5_scene_on_enter_handlers[])(void*) = {
#include "simple_app_5_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_event handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_event,
bool (*const simple_app_5_scene_on_event_handlers[])(void* context, SceneManagerEvent event) = {
#include "simple_app_5_scene_config.h"
};
#undef ADD_SCENE

// Generate scene on_exit handlers array
#define ADD_SCENE(prefix, name, id) prefix##_scene_##name##_on_exit,
void (*const simple_app_5_scene_on_exit_handlers[])(void* context) = {
#include "simple_app_5_scene_config.h"
};
#undef ADD_SCENE

/** collection of all on_enter, on_event, on_exit handlers */
const SceneManagerHandlers simple_app_5_scene_event_handlers = {
    .on_enter_handlers = simple_app_5_scene_on_enter_handlers,
    .on_event_handlers = simple_app_5_scene_on_event_handlers,
    .on_exit_handlers = simple_app_5_scene_on_exit_handlers,
    // nesessery to have this also
    .scene_num = SimpleApp5SceneCount
};