#include "../simple_app_5_i.h"
#include "simple_app_5_scene.h"


enum {
    SimpleApp5SceneMenuIndexSettings,
    SimpleApp5SceneMenuIndexAbout
} SimpleApp5MenuIndex;


static void simple_app_5_scene_menu_callback(void* context, uint32_t index) {
    furi_assert(context);
    SimpleApp5* app = context;

    // scene_manager_handle_custom_event(app->scene_manager, index);
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}


void simple_app_5_scene_menu_on_enter(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    submenu_reset(app->submenu);

    submenu_add_item(
        app->submenu,
        "Settings",
        SimpleApp5SceneMenuIndexSettings,
        simple_app_5_scene_menu_callback,
        app
    );

    submenu_add_item(
        app->submenu, 
        "About", 
        SimpleApp5SceneMenuIndexAbout, 
        simple_app_5_scene_menu_callback, 
        app);

    submenu_set_selected_item(app->submenu, scene_manager_get_scene_state(app->scene_manager, SimpleApp5SceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp5ViewMenu);
}

bool simple_app_5_scene_menu_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    SimpleApp5* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, SimpleApp5SceneMenu, event.event);
        
        switch(event.event) {
        case SimpleApp5SceneMenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, SimpleApp5SceneAbout);
            consumed = true;
            break;

        case SimpleApp5SceneMenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, SimpleApp5SceneSettings);
            consumed = true;
            break;
        
        default:
            break;
        }
    }

    return consumed;

}

void simple_app_5_scene_menu_on_exit(void* context) {
    furi_assert(context);
    SimpleApp5* app = context;

    submenu_reset(app->submenu);
}