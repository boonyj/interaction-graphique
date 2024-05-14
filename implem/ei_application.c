#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_event.c"

#include "ei_implementation.c"

ei_surface_t main_surface = NULL;
ei_surface_t pick_surface = NULL;
ei_widget_t root = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        // Initialisation of the application
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);
        pick_surface  = hw_surface_create(main_surface, main_window_size, false);
        // Creation of widget class frame (to be registered later)
        ei_widgetclass_t* frame_class = create_frame_class();
        ei_widgetclass_t* button_class = create_button_class();
        ei_widgetclass_t* toplevel_class = create_toplevel_class();

        // Register the widget class frame (to be used later)
        ei_widgetclass_register(frame_class);
        ei_widgetclass_register(button_class);
        ei_widgetclass_register(toplevel_class);

        // Can't two root at the same moment (button_root to test button and  frame_root to test_root)
        root = ei_widget_create("frame", NULL, NULL, NULL);

        // Creation of geometry manager
        ei_geometrymanager_t* placeur = create_placeur();

        // Register placeur (to be used later)
        ei_geometrymanager_register(placeur);
}

void ei_app_free(void){
        hw_quit();
}

bool callback_button_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup) {
                button_t *button = (button_t *) widget;

                if(button->relief == ei_relief_sunken){
                        button->relief = ei_relief_raised;
                }else{
                        button->relief = ei_relief_sunken;
                }
                draw_button(button, main_surface, NULL, &(button->widget.screen_location));

                return true;
        } else
                return false;
}

ei_widget_t find_widget (uint32_t* pixel_pick_surface, ei_widget_t widget) {
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                if (child->pick_id == *pixel_pick_surface) {
                        return child;
                }
                // Recursively draw children of the current child widget
                find_widget(pixel_pick_surface, child);

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
        }
        return NULL;
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

        uint32_t* pixel_pick_surface = (uint32_t*)hw_surface_get_buffer(pick_surface);

        hw_surface_unlock(main_surface);
        hw_surface_unlock(pick_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);

        ei_bind(ei_ev_mouse_buttondown, NULL, "button", callback_button_reverse_relief, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "button", callback_button_reverse_relief, NULL);

        //Main loop here
        ei_event_t event;
        ei_mouse_event_t mouse;
        ei_widget_t widget;
        while ((event.type != ei_ev_close)) {
                event.type = ei_ev_none;
                //Update screen
                hw_surface_update_rects(main_surface, NULL);
                //Wait for event
                hw_event_wait_next(&event);
                // 1. Get widget in cursor position
                widget = NULL;

                mouse = event.param.mouse;

                ei_size_t pick_size = hw_surface_get_size(pick_surface);
                pixel_pick_surface += (mouse.where.y * pick_size.width) + (mouse.where.x);
                printf("Color %p at position %d\n", pixel_pick_surface, *pixel_pick_surface);

                //widget = find_widget(pixel_pick_surface, ei_app_root_widget());

                //Search for event in list
                if (widget != NULL) {
                        for (int i = 0; i < linked_event_list_size; ++i) {
                                if (linked_event_list[i]->eventtype == event.type) {
                                        if (linked_event_list[i]->widget == widget ||
                                            strcmp(linked_event_list[i]->tag, widget->wclass->name) == 0) {
                                                // Run the callback function(s)
                                                linked_event_list[i]->callback(widget, &event, NULL);
                                        }
                                }
                        }
                }

                /*// Check if cursor is in a widget (outermost widget)
                if (mouse.where.x <= (children->screen_location.top_left.x + children->screen_location.size.width) &&
                    mouse.where.x >= children->screen_location.top_left.x) {
                        if (mouse.where.y <= (children->screen_location.top_left.y +
                                              children->screen_location.size.height) &&
                            mouse.where.y >= children->screen_location.top_left.y) {
                                widget = children;
                        } else {
                                widget = NULL;
                        }
                }
                children = children->next_sibling;*/
        }
}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

ei_widget_t ei_app_root_widget(void){
        //return frame_root;
        return root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;

}