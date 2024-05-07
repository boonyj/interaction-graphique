#include "ei_implementation.h"
#include "ei_frame.h"

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                                         ei_surface_t		surface,
                                                         ei_surface_t		pick_surface,
                                                         ei_rect_t*		clipper){
        frame_t* child = (frame_t*) widget->children_head;
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->border_width != 0) {
                        if(child->relief != 0) {
                                switch (child->relief) {
                                        case ei_relief_none :
                                                if (child->widget.wclass->drawfunc != NULL) {
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                }
                                                break;
                                        case ei_relief_raised :
                                                if (child->widget.wclass->drawfunc != NULL) {
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                }
                                                break;
                                        case ei_relief_sunken :
                                                if (child->widget.wclass->drawfunc != NULL) {
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                }
                                                break;
                                }
                        }
                }
                // Move to the next child
                child = (frame_t *) child->widget.next_sibling;
        }
}

uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color){

}


