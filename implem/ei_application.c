#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_event.c"
#include "ei_draw_tool.c"
#include "ei_callback_functions.h"

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        // Initialisation of the application
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);
        //pick_surface  = hw_create_window(main_window_size, fullscreen);
        pick_surface  = hw_surface_create(main_surface, main_window_size, false);
        root_size = malloc(sizeof(ei_size_t));
        root_size->width = main_window_size.width;
        root_size->height = main_window_size.height;
        // Creation of widget class frame (to be registered later)
        ei_widgetclass_t* frame_class = create_frame_class();
        ei_widgetclass_t* button_class = create_button_class();
        ei_widgetclass_t* toplevel_class = create_toplevel_class();

        // Register the widget class frame (to be used later)
        ei_widgetclass_register(frame_class);
        ei_widgetclass_register(button_class);
        ei_widgetclass_register(toplevel_class);


        // Creation of geometry manager
        ei_geometrymanager_t* placeur_mng = create_placeur_mng();

        // Register placeur (to be used later)
        ei_geometrymanager_register(placeur_mng);

        // Can't two root at the same moment (button_root to test button and  frame_root to test_root)
        root = ei_widget_create("frame", NULL, NULL, NULL);
}

void ei_app_free(void){
        hw_quit();
}

void ei_app_run(void) {
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

        hw_surface_lock(main_surface);
        hw_surface_lock(pick_surface);

        // Call the draw function for the root widget to draw the entire widget hierarchy
        if (root_widget->wclass != NULL) {
                if (root_widget->wclass->drawfunc != NULL) {
                        root_widget->wclass->drawfunc(root_widget, main_surface, pick_surface, NULL);
                }
        }
        ei_rect_t *clipper = &(root_widget->children_head->screen_location);
        ei_impl_widget_draw_children(root_widget, main_surface, pick_surface, clipper);


        hw_surface_unlock(main_surface);
        hw_surface_unlock(pick_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);
        //hw_surface_update_rects(pick_surface, NULL);

        ei_bind(ei_ev_mouse_buttondown, NULL, "button", callback_buttondown_reverse_relief, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_buttondown_top_level, NULL);

        //Main loop here
        ei_event_t event;
        ei_mouse_event_t mouse;
        ei_widget_t widget;
        while ((event.type != ei_ev_close)) {
                event.type = ei_ev_none;
                //Update screen
                hw_surface_update_rects(main_surface, NULL);

                hw_surface_lock(main_surface);
                hw_surface_lock(pick_surface);

                //Wait for event
                hw_event_wait_next(&event);
                // 1. Get widget in cursor position
                mouse = event.param.mouse;

                //Move to current pixel
                uint32_t* pixel_pick_surface = (uint32_t*)hw_surface_get_buffer(pick_surface);
                ei_size_t pick_size = hw_surface_get_size(pick_surface);
                pixel_pick_surface += (mouse.where.y * pick_size.width) + (mouse.where.x);

                widget = find_widget(pixel_pick_surface, ei_app_root_widget());
                //printf("Pick_id : %d , Pixel color : %d\n",widget->pick_id, *pixel_pick_surface);


                //Search for event in list
                for (int i = 0; i < linked_event_list_size; ++i) {
                        if (linked_event_list[i]->eventtype == event.type) {
                                if(linked_event_list[i]->widget != NULL){
                                        if(widget == linked_event_list[i]->widget){
                                                linked_event_list[i]->callback(widget, &event, linked_event_list[i]->user_param);
                                        }
                                }else{
                                        if(strcmp(linked_event_list[i]->tag, widget->wclass->name) == 0 ||
                                           strcmp(linked_event_list[i]->tag, "all") == 0){
                                                linked_event_list[i]->callback(widget, &event, linked_event_list[i]->user_param);
                                        }
                                }
                        }
                }
                hw_surface_unlock(main_surface);
                hw_surface_unlock(pick_surface);
        }
}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){
        exit(0);
}

ei_widget_t ei_app_root_widget(void){
        //return frame_root;
        return root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;

}