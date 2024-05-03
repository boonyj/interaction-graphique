#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_parser.h"

typedef struct {
        ei_widget_t widget;
} frame_t;

ei_widget_t frame_allocfunc (){
        // Allocate the memory
        // Initialize the memory to 0 (already done by calloc)
        ei_widget_t* widget = (ei_widget_t*) calloc(1, (unsigned long) sizeof(frame_t));

        return widget;
}

void frame_releasefunc (ei_widget_t	widget){

}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        // Cast widget to the appropriate type if necessary
        frame_t* frame = (frame_t*)widget;

        ei_widget_t child = frame->widget->children_head;
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->wclass->drawfunc != NULL) {
                        child->wclass->drawfunc(child, surface, pick_surface, clipper);
                }
                // Move to the next child
                child = child->next_sibling;
        }
}

void frame_setdefaultsfunc(ei_widget_t		widget){
}

ei_surface_t main_surface = NULL;
ei_widget_t frame_w = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        hw_init();
        main_surface  = hw_create_window(main_window_size, false);

        ei_widgetclass_t frame;
        frame.allocfunc =  &frame_allocfunc;
        frame.releasefunc = &frame_releasefunc;
        frame.drawfunc = &frame_drawfunc;
        frame.setdefaultsfunc = &frame_setdefaultsfunc;
        ei_widgetclass_register(&frame);
        frame_w = ei_widget_create("frame", NULL, NULL, NULL);

}

void ei_app_free(void){
        hw_quit();
}

void ei_app_run(void){
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

        // Get the clipper rectangle
        ei_rect_t clipper = hw_surface_get_rect(main_surface);

        hw_surface_lock(main_surface);

        // Call the draw function for the root widget to draw the entire widget hierarchy
        if (root_widget->wclass->drawfunc != NULL) {
                root_widget->wclass->drawfunc(root_widget, main_surface, NULL, &clipper);
        }

        hw_surface_unlock(main_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);


}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

ei_widget_t ei_app_root_widget(void){
        return frame_w;
}

ei_surface_t ei_app_root_surface(void){
        return main_surface;

}
