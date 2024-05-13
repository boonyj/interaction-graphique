#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_toplevel.h"

ei_surface_t main_surface = NULL;
ei_widget_t root = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        // Initialisation of the application
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);

        // Creation of widget class frame (to be registered later)
        ei_widgetclass_t* frame_class = create_frame_class();
        ei_widgetclass_t* button_class = create_button_class();
        ei_widgetclass_t* toplevel_class = create_toplevel_class();

        // Register the widget class frame (to be used later)
        ei_widgetclass_register(frame_class);
        ei_widgetclass_register(button_class);
        ei_widgetclass_register(toplevel_class);

        // Can't two root at the same moment (button_root to test button and  frame_root to test_root)
        //frame_root = ei_widget_create("frame", NULL, NULL, NULL);
        root = ei_widget_create("toplevel", NULL, NULL, NULL);

        // Creation of geometry manager
        ei_geometrymanager_t* placeur = create_placeur();

        // Register placeur (to be used later)
        ei_geometrymanager_register(placeur);
}

void ei_app_free(void){
        hw_quit();
}

void ei_app_run(void){
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

        hw_surface_lock(main_surface);

        // Call the draw function for the root widget to draw the entire widget hierarchy
        if (root_widget->wclass != NULL) {
                if (root_widget->wclass->drawfunc != NULL) {
                        root_widget->wclass->drawfunc(root_widget, main_surface, NULL, NULL);
                }
        }
        ei_rect_t* clipper = &(root_widget->children_head->screen_location);
        ei_impl_widget_draw_children(root_widget, main_surface, NULL, clipper);

        hw_surface_unlock(main_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);
        getchar();

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
