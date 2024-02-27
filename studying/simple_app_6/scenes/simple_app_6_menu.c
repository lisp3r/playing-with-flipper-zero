#include "../simple_app_6_i.h"
#include "simple_app_6_scene.h"


enum {
    SimpleApp6SceneMenuIndexFileBrowser
} SimpleApp6MenuIndex;


static void simple_app_6_scene_menu_callback(void* context, uint32_t index) {
    furi_assert(context);
    SimpleApp6* app = context;

    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}


void simple_app_6_scene_menu_on_enter(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    submenu_reset(app->submenu);

    submenu_add_item(
        app->submenu,
        "Open a book",
        SimpleApp6SceneMenuIndexFileBrowser,
        simple_app_6_scene_menu_callback,
        app
    );

    submenu_set_selected_item(app->submenu, scene_manager_get_scene_state(app->scene_manager, SimpleApp6SceneMenu));

    view_dispatcher_switch_to_view(app->view_dispatcher, SimpleApp6ViewMenu);
}

bool simple_app_6_scene_menu_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);
    SimpleApp6* app = context;

    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, SimpleApp6SceneMenu, event.event);
        
        switch(event.event) {
        case SimpleApp6SceneMenuIndexFileBrowser:
            scene_manager_next_scene(app->scene_manager, SimpleApp6SceneFileBrowser);
            consumed = true;
            break;
        
        default:
            break;
        }
    }

    return consumed;

}

void simple_app_6_scene_menu_on_exit(void* context) {
    furi_assert(context);
    SimpleApp6* app = context;

    submenu_reset(app->submenu);
}