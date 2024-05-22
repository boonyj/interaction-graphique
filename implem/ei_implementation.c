#include "ei_implementation.h"

void ei_impl_widget_draw_children(ei_widget_t widget,
                                  ei_surface_t surface,
                                  ei_surface_t pick_surface,
                                  ei_rect_t* clipper) {
        // Iterate through all children of the widget
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                if (child->wclass != NULL) {
                        clipper = &(child->screen_location);
                        child->wclass->drawfunc(child,surface,pick_surface,clipper);
                }
                ei_impl_widget_draw_children(child, surface, pick_surface, clipper);
                child = ei_widget_get_next_sibling(child);
        }
}

uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color) {
        int ir, ig, ib, ia;
        hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

        uint32_t pixel = 0;
        pixel |= (color.red   << (ir * 8));
        pixel |= (color.green << (ig * 8));
        pixel |= (color.blue  << (ib * 8));

        return pixel;
}

void free_widget_and_siblings(ei_widget_t* widget, bool is_root) {
        if (widget == NULL || *widget == NULL) return;

        ei_widget_t child = (*widget)->children_head;
        while (child != NULL) {
                ei_widget_t next_sibling = child->next_sibling;
                free_widget_and_siblings(&child, false);
                child = next_sibling;
        }

        if (!is_root) {
                (*widget)->wclass->releasefunc(*widget);
                free((*widget)->wclass);
                free((*widget)->geom_params->manager);
                free((*widget)->geom_params);
                free((*widget)->color);
                free((*widget)->pick_color);
                if ((*widget)->content_rect != NULL) {
                        free((*widget)->content_rect);
                }
                (*widget)->geom_params = NULL;
                free(*widget);
                *widget = NULL;
        }
}