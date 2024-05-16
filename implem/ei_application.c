#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_placer.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_event.c"
#include "ei_draw_tool.c"
#include "ei_implementation.c"
#include "ei_global.h"

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        // Initialisation of the application
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);
        //pick_surface  = hw_create_window(main_window_size, fullscreen);
        pick_surface  = hw_surface_create(main_surface, main_window_size, false);
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

bool callback_move_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {

        printf("Moving toplevel window!\n");

        // Cast widget to toplevel_t pointer
        toplevel_t *toplevel = (toplevel_t *) widget;

        // Retrieve initial mouse position from user_param
        ei_event_t* initial_event = (ei_event_t*) user_param;
        ei_mouse_event_t initial_mouse = initial_event->param.mouse;
        ei_mouse_event_t mouse = event->param.mouse;

        // Calculate movement since initial position
        int dx = mouse.where.x - initial_mouse.where.x;
        int dy = mouse.where.y - initial_mouse.where.y;

        // Calculate new position of toplevel widget
        int final_x = toplevel->widget.screen_location.top_left.x + dx;
        int final_y = toplevel->widget.screen_location.top_left.y + dy;

        printf("New position: x = %d, y = %d\n", final_x, final_y);

        // Update position of the toplevel widget
        ei_place(&toplevel->widget, NULL, &final_x, &final_y, NULL, NULL, NULL, NULL, NULL, NULL);


        return true;
}


bool callback_move_top_level_end (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        printf("Move toplevel end !\n");
        ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_top_level, event);
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_top_level_end, NULL);
        return true;
}

bool callback_buttondown_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                printf("Clicked !\n");
                ei_event_t* event_tbs = malloc(sizeof(ei_event_t));
                event_tbs->type = event->type;
                event_tbs->param = event->param;
                event_tbs->modifier_mask = event->modifier_mask;
                ei_bind(ei_ev_mouse_move, NULL, "all", callback_move_top_level, event_tbs);
                ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_move_top_level_end, NULL);

                return true;
        } else
                return false;
}

void draw_all_buttons_raised (ei_widget_t widget) {
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                if (strcmp(child->wclass->name, "button") == 0) {
                        button_t *button = (button_t *) child;
                        button->relief = ei_relief_raised;
                        button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                }
                // Recursively draw children of the current child widget
                draw_all_buttons_raised(child);

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
        }
}

ei_widget_t find_widget (uint32_t* pixel_pick_surface, ei_widget_t widget) {
        // Check the widget itself first
        if (widget->pick_id == *pixel_pick_surface) {
                //printf("Name: %s, Pick id : %u, Pixel : %u\n", widget->wclass->name, widget->pick_id, *pixel_pick_surface);
                return widget;
        }

        // Recursively search among the widget's children
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                ei_widget_t found_widget = find_widget(pixel_pick_surface, child);
                if (found_widget != NULL) {
                        return found_widget;
                }
                child = ei_widget_get_next_sibling(child);
        }

        // Return NULL if no matching widget is found
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

        ei_rect_t main_surface_rect = hw_surface_get_rect(main_surface);
        //ei_draw_image(main_surface, &(main_surface_rect.top_left),&main_surface_rect, "misc/sadio.jpg");


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
                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                ei_rect_t *clipper = &(root->children_head->screen_location);
                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);

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