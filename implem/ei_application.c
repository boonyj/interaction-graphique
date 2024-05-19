#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_event.c"
#include "ei_draw_tool.c"
#include "ei_implementation.c"
#include "ei_global.h"
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

        // Can't two root at the same moment (button_root to test button and frame_root to test_root)
        root = ei_widget_create("frame", NULL, NULL, NULL);
}

void ei_app_free(void){
        hw_quit();
}

bool callback_buttonup_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                button_t *button = (button_t *) user_param;
                button->relief = ei_relief_raised;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                return true;
        } else
                return false;
}

bool callback_buttondown_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                button_t *button = (button_t *) widget;
                button->relief = ei_relief_sunken;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                return true;
        } else
                return false;
}

// Function to clear the invalidated rectangles list (for cleanup purposes)
void clear_invalidated_rects() {
        ei_linked_rect_t* current = invalidated_rects_head;
        while (current != NULL) {
                ei_linked_rect_t* next = current->next;
                free(current);
                current = next;
        }
        invalidated_rects_head = NULL;
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
        // Bind the callback function to the mouse button down event on the frame widget
        //ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_move_toplevel, toplevel_widget);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_buttondown_top_level, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_toplevel_move_front, NULL);

        //Main loop here
        ei_event_t event;
        ei_mouse_event_t mouse;
        ei_widget_t widget;
        while ((event.type != ei_ev_close)) {
                event.type = ei_ev_none;
                //Update screen
                hw_surface_update_rects(main_surface, NULL);
                clear_invalidated_rects();
                hw_surface_lock(main_surface);
                hw_surface_lock(pick_surface);

                //Wait for event
                hw_event_wait_next(&event);
                // 1. Get widget in cursor position
                mouse = event.param.mouse;

                if (event.type != ei_ev_keydown && event.type != ei_ev_keyup) {
                        //Move to current pixel
                        widget = ei_widget_pick(&(mouse.where));
                        // Flag to check if toplevel exit button's button up callback has been executed
                        bool exit_button_handled = false;

                        // Special case for toplevel exit button
                        if (widget->parent != NULL) {
                                if (widget->pick_id == widget->parent->pick_id + 1) {
                                        if (event.type == ei_ev_mouse_buttonup) {
                                                for (int i = 0; i < linked_event_list_size; ++i) {
                                                        if (linked_event_list[i]->eventtype == ei_ev_mouse_buttonup) {
                                                                if (linked_event_list[i]->widget == widget) {
                                                                        linked_event_list[i]->callback(widget, &event,widget->user_data);
                                                                        exit_button_handled = true;
                                                                        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                                                                        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget->next_sibling);
                                                                        break;
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }

                        if (!exit_button_handled) {
                                //Search for event in list
                                for (int i = 0; i < linked_event_list_size; ++i) {
                                        if (linked_event_list[i]->eventtype == event.type) {
                                                if(linked_event_list[i]->widget != NULL){
                                                        if(widget == linked_event_list[i]->widget){
                                                                linked_event_list[i]->callback(widget, &event, widget->user_data);
                                                                if (root->wclass != NULL) {
                                                                        if (root->wclass->drawfunc != NULL) {
                                                                                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                                                        }
                                                                }
                                                                clipper = &(root->children_head->screen_location);
                                                                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                                                        }
                                                }else{
                                                        if(strcmp(linked_event_list[i]->tag, widget->wclass->name) == 0 ||
                                                           strcmp(linked_event_list[i]->tag, "all") == 0){
                                                                linked_event_list[i]->callback(widget, &event, linked_event_list[i]->user_param);
                                                        }
                                                }
                                        }
                                }
                        }
                } else {
                        for (int i = 0; i < linked_event_list_size; ++i) {
                                if (linked_event_list[i]->eventtype == event.type) {
                                        linked_event_list[i]->callback(widget, &event, widget->user_data);
                                        if (root->wclass != NULL) {
                                                if (root->wclass->drawfunc != NULL) {
                                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                                }
                                        }
                                        clipper = &(root->children_head->screen_location);
                                        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                                }
                        }
                }
                //printf("Pick_id : %d , Pixel color : %d\n",widget->pick_id, *pixel_pick_surface);

                /*// Flag to check if a specific widget's callback has been executed
                bool specific_widget_handled = false;

                // First, handle events for the specific widget
                for (int i = 0; i < linked_event_list_size; ++i) {
                        if (linked_event_list[i]->eventtype == event.type) {
                                if (linked_event_list[i]->widget != NULL && widget == linked_event_list[i]->widget) {
                                        if (strcmp(widget->wclass->name, "button") ==0){
                                                button_t* b= (button_t *) widget;
                                                ei_event_t event2 = {ei_ev_mouse_buttondown, 0, 'a'};
                                                callback_buttondown_reverse_relief (widget, &event2, NULL);
                                                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                                                b->callback(widget, &event, widget->user_data);
                                                if(b->widget.parent->wclass != NULL ){
                                                        ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);

                                                }
                                                if (root->wclass != NULL) {
                                                        if (root->wclass->drawfunc != NULL) {
                                                                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                                        }
                                                }
                                                ei_rect_t *clipper = &(root->children_head->screen_location);
                                                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                                        } else {
                                                linked_event_list[i]->callback(widget, &event, widget->user_data);
                                        }
                                        specific_widget_handled = true;
                                        break;  // Exit the loop once a specific widget's callback is found and executed
                                }
                        }
                }

                // If no specific widget's callback was found, handle events for the "all" tag and other matching tags
                if (!specific_widget_handled) {
                        for (int i = 0; i < linked_event_list_size; ++i) {
                                if (linked_event_list[i]->eventtype == event.type) {
                                        if (linked_event_list[i]->widget == NULL) {
                                                if (strcmp(linked_event_list[i]->tag, widget->wclass->name) == 0 ||
                                                    strcmp(linked_event_list[i]->tag, "all") == 0) {
                                                        linked_event_list[i]->callback(widget, &event, linked_event_list[i]->user_param);
                                                }
                                        }
                                }
                        }
                }*/

                hw_surface_unlock(main_surface);
                hw_surface_unlock(pick_surface);
        }
}


void ei_app_invalidate_rect(const ei_rect_t* rect) {
        // Allocate memory for a new node
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



void ei_app_quit_request(void){
        exit(0);
}

ei_widget_t ei_app_root_widget(void){
        return root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;
}