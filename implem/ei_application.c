#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_frame.h"

ei_surface_t main_surface = NULL;
ei_widget_t frame_root = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        hw_init();
        main_surface  = hw_create_window(main_window_size, fullscreen);

        ei_widgetclass_t* frame_class = create_frame();
        ei_widgetclass_register(frame_class);
        frame_root = ei_widget_create("frame", NULL, NULL, NULL);
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
                root_widget->wclass->drawfunc(root_widget, main_surface, NULL, NULL);

        }

/*        frame_t* frame = (frame_t*)frame_w;

        ei_widget_t child = frame->widget->children_head;
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->wclass->drawfunc != NULL) {
                        child->wclass->drawfunc(child, main_surface, NULL, NULL);
                }
                // Move to the next child
                child = child->next_sibling;
        }*/


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
        return frame_root;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;

}
