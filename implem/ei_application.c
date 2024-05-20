#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_global.h"
#include "ei_callback_toplevel.h"
#include "ei_entry_class.h"
#include "ei_callback_button.h"

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
        ei_widgetclass_t* entry_class = create_entry_class();

        // Register the widget class frame (to be used later)
        ei_widgetclass_register(frame_class);
        ei_widgetclass_register(button_class);
        ei_widgetclass_register(toplevel_class);
        ei_widgetclass_register(entry_class);


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

void exit_button_press(ei_widget_t widget, ei_linked_event_t *head, ei_event_t *event, bool *exit_button_handled) {
        if (widget->parent != NULL && strcmp(widget->parent->wclass->name, "toplevel") == 0) {
                if (widget->pick_id == widget->parent->pick_id + 1) {
                        if ((*event).type == ei_ev_mouse_buttonup) {
                                while (head != NULL) {
                                        if (head->eventtype == ei_ev_mouse_buttonup) {
                                                if (head->widget == widget) {
                                                        head->callback(widget, event, widget->user_data);
                                                        (*exit_button_handled) = true;
                                                        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                                                        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget->next_sibling);
                                                        break;
                                                }
                                        }
                                        head = head->next;
                                }
                        } else if ((*event).type == ei_ev_mouse_buttondown) {
                                while (head != NULL) {
                                        if (head->eventtype == ei_ev_mouse_buttondown) {
                                                if (head->widget == widget) {
                                                        head->callback(widget, event, widget->user_data);
                                                        break;
                                                }
                                        }
                                        head = head->next;
                                }
                        }
                }
        }
}

void draw_all_widgets(ei_rect_t *clipper) {
        if (root->wclass != NULL) {
                if (root->wclass->drawfunc != NULL) {
                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                }
        }
        clipper = &(root->children_head->screen_location);
        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
}

void get_event(ei_rect_t *clipper, ei_event_t *event, struct ei_impl_widget_t *widget, ei_linked_event_t *head) {
        head = linked_event_list;
        //Search for event in list
        while (head != NULL) {
                if (head->eventtype == (*event).type) {
                        if(head->widget != NULL){
                                if(widget == head->widget){
                                        head->callback(widget, event, widget->user_data);
                                        draw_all_widgets(clipper);
                                }
                        }else{
                                if(strcmp(head->tag, widget->wclass->name) == 0 ||
                                   strcmp(head->tag, "all") == 0){
                                        head->callback(widget, event, head->user_param);
                                }
                        }
                }
                head = head->next;
        }
}

void get_keydown_event(struct ei_impl_widget_t *widget, ei_linked_event_t *head, ei_rect_t **clipper,
                       ei_event_t *event) {
        while (head != NULL) {
                if (head->eventtype == (*event).type) {
                        head->callback(widget, event, head->user_param);
                        if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        (*clipper) = &(root->children_head->screen_location);
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, (*clipper));
                }
                head = head->next;
        }
}

void ei_app_run(void) {
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

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

        // Bind the callback function to the mouse button down event on the frame widget
        ei_bind(ei_ev_mouse_buttondown, NULL, "button", callback_buttondown_reverse_relief, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "toplevel", callback_buttondown_top_level, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "all", callback_toplevel_move_front, NULL);

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

                ei_linked_event_t* head = linked_event_list;

                if (event.type != ei_ev_keydown && event.type != ei_ev_keyup) {
                        //Move to current pixel
                        widget = ei_widget_pick(&(mouse.where));
                        // Flag to check if toplevel exit button's button up callback has been executed
                        bool exit_button_handled = false;

                        // Special case for toplevel exit button
                        exit_button_press(widget, head, &event, &exit_button_handled);

                        if (!exit_button_handled) {
                                get_event(clipper, &event, widget, head);
                        }
                } else if (event.type == ei_ev_exposed){
                        if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                }
                else {
                        get_keydown_event(widget, head, &clipper, &event);
                }

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
        ei_widget_destroy(root);
        exit(0);
}

ei_widget_t ei_app_root_widget(void){
        return root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;
}