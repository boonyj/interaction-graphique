#include "ei_application.h"
#include "ei_app_run_tool.h"
#include "ei_global.h"
#include "ei_button.h"
#include "ei_frame.h"
#include "ei_toplevel.h"
#include "ei_entry.h"
#include "ei_placeur.h"
#include "ei_callback_button.h"
#include "ei_callback_toplevel.h"

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        // Initialisation of the application
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);
        //pick_surface  = hw_create_window(main_window_size, fullscreen);
        pick_surface  = hw_surface_create(main_surface, main_window_size, false);
        root_size = malloc(sizeof(ei_size_t));
        root_size->width = main_window_size.width;
        root_size->height = main_window_size.height;

        // Creation of widget classes
        ei_widgetclass_t* frame_class = create_frame_class();
        ei_widgetclass_t* button_class = create_button_class();
        ei_widgetclass_t* toplevel_class = create_toplevel_class();
        ei_widgetclass_t* entry_class = create_entry_class();

        // Register the widget classes
        ei_widgetclass_register(frame_class);
        ei_widgetclass_register(button_class);
        ei_widgetclass_register(toplevel_class);
        ei_widgetclass_register(entry_class);

        // Creation of geometry manager
        ei_geometrymanager_t* placeur_mng = create_placeur_mng();
        ei_geometrymanager_register(placeur_mng);

        root = ei_widget_create("frame", NULL, NULL, NULL);
}


void ei_app_run(void) {
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

        run_all_ei_place(root_widget);

        hw_surface_lock(main_surface);
        hw_surface_lock(pick_surface);

        // Call the draw function for the root widget to draw the entire widget hierarchy
        ei_rect_t *clipper = &(root_widget->children_head->screen_location);
        draw_all_widgets(clipper);

        hw_surface_unlock(main_surface);
        hw_surface_unlock(pick_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);
        //hw_surface_update_rects(pick_surface, NULL);

        // Bind callback functions that are used in every application
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", callback_buttondown_reverse_relief, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_buttondown_top_level, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "all", callback_toplevel_move_front, NULL);

        // Main loop
        ei_event_t event = (ei_event_t){};
        ei_mouse_event_t mouse;
        ei_widget_t widget;
        while ((event.type != ei_ev_close)) {
                event.type = ei_ev_none;
                // Update screen
                hw_surface_update_rects(main_surface, NULL);
                //hw_surface_update_rects(pick_surface, NULL);

                //clear_invalidated_rects();
                hw_surface_lock(main_surface);
                hw_surface_lock(pick_surface);

                //Wait for event
                hw_event_wait_next(&event);

                // Get widget in cursor position
                mouse = event.param.mouse;
                ei_linked_event_t* head = linked_event_list;

                if (event.type == ei_ev_exposed){
                        if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                } else if (event.type == ei_ev_app){
                        run_ev_app_event(widget,linked_event_list,&clipper,&event);
                } else  if (event.type != ei_ev_keydown && event.type != ei_ev_keyup) {
                        // Move to current pixel
                        widget = ei_widget_pick(&(mouse.where));
                        // Flag to check if toplevel exit button's button up callback has been executed
                        bool exit_button_handled = false;

                        // Special case for toplevel exit button
                        exit_toplevel_button_press(widget, head, &event, &exit_button_handled);

                        if (!exit_button_handled) {
                                get_event(clipper, &event, widget, head);
                        }
                }
                else {
                        get_keydown_event(widget, head, &clipper, &event);
                }

                hw_surface_unlock(main_surface);
                hw_surface_unlock(pick_surface);
        }
}

void ei_app_invalidate_rect(const ei_rect_t* rect) {
        ei_linked_rect_t* new_node = (ei_linked_rect_t*)malloc(sizeof(ei_linked_rect_t));
        if (new_node == NULL) {
                // Handle memory allocation failure
                return;
        }

        // Copy the rectangle data into the new node
        new_node->rect = *rect;
        new_node->next = NULL;

        // Add the new node to the front of the list (or handle differently if maintaining order is important)
        if (invalidated_rects_head == NULL) {
                invalidated_rects_head = new_node;
        } else {
                ei_linked_rect_t* current = invalidated_rects_head;
                while (current->next != NULL) {
                        current = current->next;
                }
                current->next = new_node;
        }
}


void ei_app_free(void){
        free_event_list();
        hw_surface_unlock(main_surface);
        hw_surface_unlock(pick_surface);
        hw_surface_free(main_surface);
        hw_surface_free(pick_surface);
        free(root_size);
        ei_widget_destroy(root);
        hw_quit();
}

void ei_app_quit_request(void){
        ei_app_free();
        exit(0);
}

ei_widget_t ei_app_root_widget(void){
        return root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;
}