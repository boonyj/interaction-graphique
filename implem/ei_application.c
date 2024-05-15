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
        pick_surface  = hw_create_window(main_window_size, fullscreen);
        //pick_surface  = hw_surface_create(main_surface, main_window_size, false);
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

bool callback_buttondown_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                button_t *button = (button_t *) widget;
                button->relief = ei_relief_sunken;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                return true;
        } else
                return false;
}

void draw_buttons (ei_widget_t widget) {
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                if (strcmp(child->wclass->name, "button") == 0) {
                        button_t *button = (button_t *) child;
                        button->relief = ei_relief_raised;
                        button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                }
                // Recursively draw children of the current child widget
                draw_buttons(child);

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
        }
}

bool callback_buttonup_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                draw_buttons(ei_app_root_widget());
                return true;
        } else
                return false;
}

ei_widget_t find_widget (uint32_t* pixel_pick_surface, ei_widget_t widget) {
        ei_widget_t child = ei_widget_get_first_child(widget);
        ei_widget_t temp;
        while (child != NULL) {
                uint32_t res =  *pixel_pick_surface;
                if (child->pick_id == *pixel_pick_surface) {
                        printf("Name: %s, Pick id : %d, Pixel : %d \n", child->wclass->name, child->pick_id, *pixel_pick_surface);
                        return child;
                }
                // Recursively draw children of the current child widget
                temp = find_widget(pixel_pick_surface, child);

                if (temp != ei_app_root_widget()){
                        return temp;
                }

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
        }

        return ei_app_root_widget();

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
        hw_surface_update_rects(pick_surface, NULL);

        ei_bind(ei_ev_mouse_buttondown, NULL, "button", callback_buttondown_reverse_relief, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, NULL);

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


                //Convert pixel color
                uint8_t* pps_8bit = (uint8_t*) pixel_pick_surface;
                int ir, ig, ib, ia;
                hw_surface_get_channel_indices(pick_surface, &ir, &ig, &ib, &ia);
                uint8_t temp[4]	= { 255, 255, 255, 255 };

                temp[ir] = pps_8bit[ir];
                temp[ig] = pps_8bit[ig];
                temp[ib] = pps_8bit[ib];
                temp[ia] = 255;

                printf("Pich : %d:%d:%d:%d \n",temp[ir],temp[ig],temp[ib],temp[ia]);

                uint8_t* res = malloc(4*sizeof(int8_t));

                res[0] = temp[0];
                res[1] = temp[1];
                res[2] = temp[2];
                res[3] = temp[3];

                printf("Res : %d",*((uint32_t*)res));

                widget = find_widget(pixel_pick_surface, ei_app_root_widget());
                printf("Pick_id : %d , Pixel color : %d\n",widget->pick_id, *(uint32_t*)(res));

                //Search for event in list
                for (int i = 0; i < linked_event_list_size; ++i) {
                        if (linked_event_list[i]->eventtype == event.type) {
                                if (linked_event_list[i]->widget == widget ||
                                    strcmp(linked_event_list[i]->tag, widget->wclass->name) == 0 ||
                                        strcmp(linked_event_list[i]->tag, "all") == 0) {
                                        // Run the callback function(s)
                                        linked_event_list[i]->callback(widget, &event, NULL);
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