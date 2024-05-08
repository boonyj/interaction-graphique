#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_widget_attributes.h"
#include "ei_button.h"
#include <math.h>

#define PI 3.14159265

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

void draw_frame (frame_t* child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->border_width != 0) {
                        if (child->relief != 0) {
                                if (child->widget.wclass->drawfunc != NULL) {
                                        switch (child->relief) {
                                                case ei_relief_none :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;

                                                case ei_relief_raised :
                                                        unsigned char red = child->widget.pick_color->red;
                                                        unsigned char green = child->widget.pick_color->green;
                                                        unsigned char blue = child->widget.pick_color->blue;
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                        points->x = clipper->top_left.x;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width / 3;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height / 2;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width * 2 / 3;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height / 2;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width;
                                                        points->y = clipper->top_left.y;
                                                        points++;
                                                        points->x = clipper->top_left.x;
                                                        points->y = clipper->top_left.y;
                                                        points -= 4;
                                                        size_t points_size = 5;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        child->widget.pick_color->red = light(red);
                                                        child->widget.pick_color->green = light(green);
                                                        child->widget.pick_color->blue = light(blue);

                                                        points += 4;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height;
                                                        points -= 4;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;

                                                case ei_relief_sunken :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;
                                        }
                                }
                        }
                }
                // Move to the next child
                child = (frame_t *) child->widget.next_sibling;
        }
}


void arc(ei_point_t center, int radius, double start_angle, double end_angle, int nb_segments, ei_point_t** arc_points) {
        // Allouer de la mémoire pour le tableau de points de l'arc
        *arc_points = malloc(nb_segments * sizeof(ei_point_t));

        double angle_step = (end_angle - start_angle) / nb_segments;

        // Générer les points de l'arc en utilisant la formule paramétrique d'un cercle
        for (int i = 0; i < nb_segments; ++i) {
                double angle = start_angle + angle_step * i;
                double x = center.x + radius * cos(angle * PI / 180);
                double y = center.y - radius * sin(angle * PI / 180); // Soustraction car l'axe y est orienté vers le bas sur les écrans
                (*arc_points)[i].x = (int)x;
                (*arc_points)[i].y = (int)y;
        }
}

void draw_button (button_t * child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->border_width != 0) {
                        if (child->relief != 0) {
                                if (child->widget.wclass->drawfunc != NULL) {
                                        switch (child->relief) {
                                                case ei_relief_none :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;

                                                case ei_relief_raised :
                                                        unsigned char red = child->widget.pick_color->red;
                                                        unsigned char green = child->widget.pick_color->green;
                                                        unsigned char blue = child->widget.pick_color->blue;
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        ei_point_t *points = malloc(8 * sizeof(ei_point_t));
                                                        points->x = clipper->top_left.x;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width / 3;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height / 2;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width * 2 / 3;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height / 2;
                                                        points++;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width;
                                                        points->y = clipper->top_left.y;
                                                        points++;
                                                        points->x = clipper->top_left.x;
                                                        points->y = clipper->top_left.y;
                                                        points -= 4;
                                                        size_t points_size = 5;


                                                        //test to draw the radius of top left corner
                                                        /*ei_point_t *center_corner_left = malloc( sizeof(ei_point_t));
                                                        center_corner_left->x = clipper->top_left.x + child->corner_radius;
                                                        center_corner_left->y = clipper->top_left.y + child->corner_radius;
                                                        ei_point_t *points_arc = malloc(10 * sizeof(ei_point_t));

                                                        arc(*center_corner_left, child->corner_radius, 90, 180, 4, &points_arc);
                                                        points++;
                                                        points->x = points_arc->x;
                                                        points->y = points_arc->y;
                                                        points_arc++;
                                                        points++;
                                                        points->x = points_arc->x;
                                                        points->y = points_arc->y;
                                                        points_arc++;
                                                        points++;
                                                        points->x = points_arc->x;
                                                        points->y = points_arc->y;
                                                        points_arc++;
                                                        points++;
                                                        points->x = points_arc->x;
                                                        points->y = points_arc->y;
                                                        points_arc++;
                                                        points -= 7;
                                                        size_t points_size = 8;*/

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        child->widget.pick_color->red = light(red);
                                                        child->widget.pick_color->green = light(green);
                                                        child->widget.pick_color->blue = light(blue);

                                                        points += 4;
                                                        points->x = clipper->top_left.x +
                                                                    child->widget.requested_size.width;
                                                        points->y = clipper->top_left.y +
                                                                    child->widget.requested_size.height;
                                                        points -= 4;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;

                                                case ei_relief_sunken :
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                        break;
                                        }
                                }
                        }
                }
                // Move to the next child
                child = (button_t *) child->widget.next_sibling;
        }

}

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                                         ei_surface_t		surface,
                                                         ei_surface_t		pick_surface,
                                                         ei_rect_t*		clipper){
        ei_widget_t type = ei_widget_get_first_child(widget);
        if (strcmp(type->wclass->name, "frame") == 0){
                frame_t* child = (frame_t*) ei_widget_get_first_child(widget);
                draw_frame(child, surface, pick_surface, clipper);
        } else if (strcmp(type->wclass->name, "button") == 0){
                button_t* child = (button_t*) ei_widget_get_first_child(widget);
                draw_button(child, surface, pick_surface, clipper);
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


