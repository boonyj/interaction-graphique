#include "ei_implementation.h"


void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                                         ei_surface_t		surface,
                                                         ei_surface_t		pick_surface,
                                                         ei_rect_t*		clipper){
        ei_widget_t child = widget->children_head;
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->wclass->drawfunc != NULL) {
                        child->wclass->drawfunc(child, surface, NULL, clipper);
                }
                if (child->geom_params->border_width != NULL) {
                        if(child->geom_params->relief != NULL) {
                                switch (*(child->geom_params->relief)) {
                                        case ei_relief_none :
                                                break;
                                        case ei_relief_raised :
                                                break;
                                        case ei_relief_sunken :
                                                break;
                                }
                        }
                }
                // Move to the next child
                child = child->next_sibling;
        }
}

uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color){

}


