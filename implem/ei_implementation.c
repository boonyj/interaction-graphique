#include "ei_implementation.h"
#include "ei_widget_attributes.h"

void ei_impl_widget_draw_children(ei_widget_t widget,
                                  ei_surface_t surface,
                                  ei_surface_t pick_surface,
                                  ei_rect_t* clipper) {
        // Iterate through all children of the widget
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                clipper = &(child->screen_location);
                child->wclass->drawfunc(child,surface,pick_surface,clipper);

                // Recursively draw children of the current child widget
                ei_impl_widget_draw_children(child, surface, pick_surface, clipper);

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
        }
}


uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color) {
        int ir, ig, ib, ia;
        hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

        // Construct the 32-bit integer representation of the color
        uint32_t pixel = 0;
        pixel |= (color.red   << (ir * 8));
        pixel |= (color.green << (ig * 8));
        pixel |= (color.blue  << (ib * 8));

        return pixel;
}


