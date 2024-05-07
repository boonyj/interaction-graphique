#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_widget_attributes.h"

unsigned char dark(unsigned char colorComponent) {
        double darkComponent = colorComponent * 0.6;

        if (darkComponent < 0) {
                darkComponent = 0;
        }

        return (unsigned char)darkComponent;
}

unsigned char light(unsigned char colorComponent) {
        double lightComponent = colorComponent * 1.4;

        if (lightComponent < 0) {
                lightComponent = 0;
        }

        return (unsigned char)lightComponent;
}

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                                         ei_surface_t		surface,
                                                         ei_surface_t		pick_surface,
                                                         ei_rect_t*		clipper){
        frame_t* child = (frame_t*) ei_widget_get_first_child(widget);
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->border_width != 0) {
                        if(child->relief != 0) {
                                if (child->widget.wclass->drawfunc != NULL) {
                                        switch (child->relief) {
                                                case ei_relief_none :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                        break;

                                                case ei_relief_raised :
                                                        unsigned char red = child->widget.pick_color->red;
                                                        unsigned char green = child->widget.pick_color->green;
                                                        unsigned char blue = child->widget.pick_color->blue;
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);

                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                        break;

                                                case ei_relief_sunken :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL, clipper);
                                                        break;
                                        }
                                }
                        }
                }
                // Move to the next child
                child = (frame_t *) child->widget.next_sibling;
        }
}

uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color){
        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        *pixel_ptr = (color.alpha << 24) | // Alpha component
                     (color.red << 16) |   // Red component
                     (color.green << 8) |  // Green component
                     color.blue;           // Blue component

        return *pixel_ptr;
}


