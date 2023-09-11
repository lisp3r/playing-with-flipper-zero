#include "../simple_app_5_i.h"
#include "simple_app_5_scene.h"

enum {
    SimpleApp5SceneSettingsIndexMode,
} SimpleApp5SceneSettingsIndex;

// enum {
//     SimpleApp5SceneSettingsMode1,
//     SimpleApp5SceneSettingsMode2,
//     SimpleApp5SceneSettingsModeCount,
// } SimpleApp5SceneSettingsModes;


const char* simple_app_5_scene_settings_mode_text[SimpleApp5SettingsModeCount] = {
    "Mode 1",
    "Mode 2",
};

int simple_app_5_scene_settings_mode_values[SimpleApp5SettingsModeCount] = {
    SimpleApp5SettingsMode1,
    SimpleApp5SettingsMode2
};

static void simple_app_5_scene_settings_set_mode(VariableItem* item) {
    SimpleApp5* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);

    variable_item_set_current_value_text(item, simple_app_5_scene_settings_mode_text[index]);
    app->mode = simple_app_5_scene_settings_mode_values[index];


    // if(index == SimpleApp5SceneSettingsMode1) {
    //     view_dispatcher_send_custom_event(app->view_dispatcher, SimpleApp5EventSettingsOff);
    // }
    // else if(index == SimpleApp5SceneSettingsMode2) {
    //     view_dispatcher_send_custom_event(app->view_dispatcher, SimpleApp5EventSettingsOn);
    // }
}

void simple_app_5_scene_settings_on_enter(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    // variable_item_list_set_enter_callback(app->settings, simple_app_5_scene_settings_enter_callback, app);

    VariableItem* item = variable_item_list_add(app->settings, "Mode",
        SimpleApp5SettingsModeCount,
        simple_app_5_scene_settings_set_mode,
        app
    );

    variable_item_set_current_value_index(item, app->mode);
    variable_item_set_current_value_text(item, simple_app_5_scene_settings_mode_text[app->mode]);

    // variable_item_list_set_selected_item(app->settings, scene_manager_get_scene_state(app->scene_manager, SimpleApp5SceneSettings));
    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp5ViewSettings);
}

bool simple_app_5_scene_settings_on_event(void* context, SceneManagerEvent event) {
    // furi_assert(context);
    // SimpleApp5* app = context;
    bool consumed = false;

    // if(event.type == SceneManagerEventTypeCustom) {
    //     if(event.event == SimpleApp5EventSettingsOff) {
    //         // do something
    //         consumed = true;
    //     } else if(event.event == SimpleApp5EventSettingsOn) {
    //         // do something else
    //         consumed = true;
    //     }
    // }

    UNUSED(context);
    UNUSED(event);
    return consumed;
}

void simple_app_5_scene_settings_on_exit(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    variable_item_list_reset(app->settings);
}