#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_widget_attributes.h"
#include "ei_button.h"
#include "ei_toplevel.h"
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
                        if (child->widget.wclass->drawfunc != NULL) {
                                unsigned char red = child->widget.pick_color->red;
                                unsigned char green = child->widget.pick_color->green;
                                unsigned char blue = child->widget.pick_color->blue;

                                ei_point_t *points = NULL;
                                size_t points_size;

                                switch (child->relief) {
                                        case ei_relief_none :
                                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                               clipper);
                                                break;
                                        case ei_relief_raised :
                                                child->widget.pick_color->red = light(red);
                                                child->widget.pick_color->green = light(green);
                                                child->widget.pick_color->blue = light(blue);

                                                points = malloc(5 * sizeof(ei_point_t));
                                                points[0].x = clipper->top_left.x;
                                                points[0].y = clipper->top_left.y + child->widget.requested_size.height;
                                                points[1].x = clipper->top_left.x + child->widget.requested_size.width / 3;
                                                points[1].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                points[2].x = clipper->top_left.x + child->widget.requested_size.width * 2 / 3;
                                                points[2].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                points[3].x = clipper->top_left.x + child->widget.requested_size.width;
                                                points[3].y = clipper->top_left.y;
                                                points[4].x = clipper->top_left.x;
                                                points[4].y = clipper->top_left.y;
                                                points_size = 5;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *(child->widget.pick_color), clipper);

                                                child->widget.pick_color->red = dark(red);
                                                child->widget.pick_color->green = dark(green);
                                                child->widget.pick_color->blue = dark(blue);

                                                points[4].x = clipper->top_left.x + child->widget.requested_size.width;
                                                points[4].y = clipper->top_left.y + child->widget.requested_size.height;

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
                                                child->widget.pick_color->red = dark(red);
                                                child->widget.pick_color->green = dark(green);
                                                child->widget.pick_color->blue = dark(blue);

                                                points = malloc(5 * sizeof(ei_point_t));
                                                points[0].x = clipper->top_left.x;
                                                points[0].y = clipper->top_left.y + child->widget.requested_size.height;
                                                points[1].x = clipper->top_left.x + child->widget.requested_size.width / 3;
                                                points[1].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                points[2].x = clipper->top_left.x + child->widget.requested_size.width * 2 / 3;
                                                points[2].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                points[3].x = clipper->top_left.x + child->widget.requested_size.width;
                                                points[3].y = clipper->top_left.y;
                                                points[4].x = clipper->top_left.x;
                                                points[4].y = clipper->top_left.y;
                                                points_size = 5;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *(child->widget.pick_color), clipper);

                                                child->widget.pick_color->red = light(red);
                                                child->widget.pick_color->green = light(green);
                                                child->widget.pick_color->blue = light(blue);

                                                points[4].x = clipper->top_left.x + child->widget.requested_size.width;
                                                points[4].y = clipper->top_left.y + child->widget.requested_size.height;

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

ei_point_t* rounded_frame (ei_rect_t rect, int radius, int partie) {
        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points_arc = NULL;
        ei_point_t *points = NULL;
        int nb_segments = 32;
        if (partie == 0) {
                center_corner->x = rect.top_left.x +  rect.size.width  - radius;
                center_corner->y = rect.top_left.y + radius;

                points = malloc(4*nb_segments * sizeof(ei_point_t));
                arc(*center_corner, radius, 0, 90, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i].x = points_arc[i].x;
                        points[i].y = points_arc[i].y;
                }

                center_corner->x = rect.top_left.x + radius;
                center_corner->y = rect.top_left.y + radius;
                arc(*center_corner, radius, 90, 180, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + nb_segments].x = points_arc[i].x;
                        points[i + nb_segments].y = points_arc[i].y;
                }

                center_corner->x = rect.top_left.x + radius;
                center_corner->y = rect.top_left.y + rect.size.height - radius;
                arc(*center_corner, radius, 180, 270, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + 2*nb_segments].x = points_arc[i].x;
                        points[i + 2*nb_segments].y = points_arc[i].y;
                }

                center_corner->x = rect.top_left.x +  rect.size.width  - radius;
                center_corner->y = rect.top_left.y +  rect.size.height  - radius;
                arc(*center_corner, radius, 270, 360, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + 3*nb_segments].x = points_arc[i].x;
                        points[i + 3*nb_segments].y = points_arc[i].y;
                }
        } else if (partie == 1) {
                center_corner->x = rect.top_left.x + radius;
                center_corner->y = rect.top_left.y + rect.size.height - radius;

                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));
                arc(*center_corner, radius, 180, 225, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i].x = points_arc[i].x;
                        points[i].y = points_arc[i].y;
                }

                points[nb_segments].x = rect.top_left.x + rect.size.width / 3;
                points[nb_segments].y = rect.top_left.y + rect.size.height / 2;

                points[nb_segments+1].x = rect.top_left.x + rect.size.width * 2 / 3;
                points[nb_segments+1].y = rect.top_left.y + rect.size.height / 2;

                center_corner->x = rect.top_left.x + rect.size.width - radius;
                center_corner->y = rect.top_left.y + radius;

                arc(*center_corner, radius, 45, 90, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + nb_segments+2].x = points_arc[i].x;
                        points[i + nb_segments+2].y = points_arc[i].y;
                }

                center_corner->x = rect.top_left.x + radius;
                center_corner->y = rect.top_left.y + radius;

                arc(*center_corner, radius, 90, 180, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + 2*nb_segments+2].x = points_arc[i].x;
                        points[i + 2*nb_segments+2].y = points_arc[i].y;
                }
        } else if (partie == 2) {
                center_corner->x = rect.top_left.x + rect.size.width - radius;
                center_corner->y = rect.top_left.y + radius;

                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));
                arc(*center_corner, radius, 0, 45, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i].x = points_arc[i].x;
                        points[i].y = points_arc[i].y;
                }

                points[nb_segments].x = rect.top_left.x + rect.size.width * 2 / 3;
                points[nb_segments].y = rect.top_left.y + rect.size.height / 2;

                points[nb_segments+1].x = rect.top_left.x + rect.size.width / 3;
                points[nb_segments+1].y = rect.top_left.y + rect.size.height / 2;

                center_corner->x = rect.top_left.x + radius;
                center_corner->y = rect.top_left.y + rect.size.height - radius;
                arc(*center_corner, radius, 225, 270, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + nb_segments+2].x = points_arc[i].x;
                        points[i + nb_segments+2].y = points_arc[i].y;
                }

                center_corner->x = rect.top_left.x +  rect.size.width - radius;
                center_corner->y = rect.top_left.y + rect.size.height - radius;

                arc(*center_corner, radius, 270, 360, nb_segments, &points_arc);
                for (int i = 0; i < nb_segments; ++i) {
                        points[i + 2*nb_segments+2].x = points_arc[i].x;
                        points[i + 2*nb_segments+2].y = points_arc[i].y;
                }
        }
        return points;
}

void draw_button (button_t * child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->border_width != 0) {
                        if (child->widget.wclass->drawfunc != NULL) {
                                unsigned char red = child->widget.pick_color->red;
                                unsigned char green = child->widget.pick_color->green;
                                unsigned char blue = child->widget.pick_color->blue;
                                switch (child->relief) {
                                        case ei_relief_none :
                                                if (child->corner_radius == 0) {

                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);

                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;
                                                        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                                       clipper);
                                                } else {
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        int nb_segments = 32;

                                                        ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 0);

                                                        size_t points_size = 4*nb_segments;
                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;

                                                        points = rounded_frame(*clipper, child->corner_radius, 0);

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);
                                                }
                                                break;
                                        case ei_relief_raised :
                                                if (child->corner_radius == 0){
                                                        child->widget.pick_color->red = light(red);
                                                        child->widget.pick_color->green = light(green);
                                                        child->widget.pick_color->blue = light(blue);

                                                        ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                        points[0].x = clipper->top_left.x;
                                                        points[0].y = clipper->top_left.y + child->widget.requested_size.height;
                                                        points[1].x = clipper->top_left.x + child->widget.requested_size.width / 3;
                                                        points[1].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                        points[2].x = clipper->top_left.x + child->widget.requested_size.width * 2 / 3;
                                                        points[2].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                        points[3].x = clipper->top_left.x + child->widget.requested_size.width;
                                                        points[3].y = clipper->top_left.y;
                                                        points[4].x = clipper->top_left.x;
                                                        points[4].y = clipper->top_left.y;
                                                        size_t points_size = 5;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        points[4].x = clipper->top_left.x + child->widget.requested_size.width;
                                                        points[4].y = clipper->top_left.y + child->widget.requested_size.height;

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
                                                } else {
                                                        int nb_segments = 32;

                                                        // Partie haute
                                                        child->widget.pick_color->red = light(red);
                                                        child->widget.pick_color->green = light(green);
                                                        child->widget.pick_color->blue = light(blue);

                                                        ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                        size_t points_size = 3*nb_segments+2;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        // Partie basse
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        points = rounded_frame(*clipper, child->corner_radius, 2);

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        // Draw inside button
                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;

                                                        points = rounded_frame(*clipper, child->corner_radius, 0);

                                                        points_size = 4*nb_segments;
                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                }
                                                if (child->text != NULL){
                                                        int width = 0;
                                                        int height = 0;
                                                        hw_text_compute_size(child->text,child->text_font, &width, &height);
                                                        ei_point_t where = child->widget.screen_location.top_left;
                                                        where.x += child->widget.screen_location.size.width/2 - width/2;
                                                        where.y += child->widget.screen_location.size.height/2 - height/2;

                                                        ei_draw_text(surface, &where, child->text, child->text_font, child->text_color, clipper);
                                                }
                                                break;
                                        case ei_relief_sunken :
                                                if (child->corner_radius == 0){
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                        points[0].x = clipper->top_left.x;
                                                        points[0].y = clipper->top_left.y + child->widget.requested_size.height;
                                                        points[1].x = clipper->top_left.x + child->widget.requested_size.width / 3;
                                                        points[1].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                        points[2].x = clipper->top_left.x + child->widget.requested_size.width * 2 / 3;
                                                        points[2].y = clipper->top_left.y + child->widget.requested_size.height / 2;
                                                        points[3].x = clipper->top_left.x + child->widget.requested_size.width;
                                                        points[3].y = clipper->top_left.y;
                                                        points[4].x = clipper->top_left.x;
                                                        points[4].y = clipper->top_left.y;
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
                                                } else {
                                                        int nb_segments = 32;

                                                        red = child->widget.pick_color->red;
                                                        green = child->widget.pick_color->green;
                                                        blue = child->widget.pick_color->blue;

                                                        // Partie haute
                                                        child->widget.pick_color->red = dark(red);
                                                        child->widget.pick_color->green = dark(green);
                                                        child->widget.pick_color->blue = dark(blue);

                                                        ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                        size_t points_size = 3*nb_segments+2;

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        // Partie basse
                                                        child->widget.pick_color->red = light(red);
                                                        child->widget.pick_color->green = light(green);
                                                        child->widget.pick_color->blue = light(blue);

                                                        points = rounded_frame(*clipper, child->corner_radius, 2);

                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);

                                                        // Draw inside button
                                                        clipper->size.width -= child->border_width * 2;
                                                        clipper->size.height -= child->border_width * 2;
                                                        clipper->top_left.x += child->border_width;
                                                        clipper->top_left.y += child->border_width;
                                                        child->widget.pick_color->red = red;
                                                        child->widget.pick_color->green = green;
                                                        child->widget.pick_color->blue = blue;

                                                        points = rounded_frame(*clipper, child->corner_radius, 0);

                                                        points_size = 4*nb_segments;
                                                        ei_draw_polygon(surface, points, points_size,
                                                                        *(child->widget.pick_color), clipper);
                                                        if (child->text != NULL){
                                                                //ei_draw_text(surface,&clipper->top_left, child->text, NULL, child->text_color, clipper);
                                                        }
                                                }
                                                break;
                                }
                        }
                }
                // Move to the next child
                child = (button_t *) child->widget.next_sibling;
        }
}

void draw_toplevel (toplevel_t * child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper) {
        unsigned char red = child->widget.pick_color->red;
        unsigned char green = child->widget.pick_color->green;
        unsigned char blue = child->widget.pick_color->blue;
        child->widget.pick_color->red = dark(red);
        child->widget.pick_color->green = dark(green);
        child->widget.pick_color->blue = dark(blue);

        int width = 0;
        int height = 0;
        hw_text_compute_size(child->title,child->title_font, &width, &height);

        ei_rect_t* clipper_title = malloc(sizeof (ei_rect_t));
        clipper_title->top_left.x = clipper->top_left.x;
        clipper_title->top_left.y = clipper->top_left.y;

        clipper_title->size.width = clipper->size.width;
        clipper_title->size.height = height;
        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points_arc = NULL;
        ei_point_t *points = NULL;
        int nb_segments = 32;
        int radius = 15;
        points = malloc((2*nb_segments + 2) * sizeof(ei_point_t));

        center_corner->x = clipper_title->top_left.x + clipper_title->size.width;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[0].x = center_corner->x;
        points[0].y = center_corner->y;

        center_corner->x = clipper_title->top_left.x +  clipper_title->size.width  - radius;
        center_corner->y = clipper_title->top_left.y + radius;

        arc(*center_corner, radius, 0, 90, nb_segments, &points_arc);
        for (int i = 0; i < nb_segments; ++i) {
                points[i+1].x = points_arc[i].x;
                points[i+1].y = points_arc[i].y;
        }

        center_corner->x = clipper_title->top_left.x + radius;
        center_corner->y = clipper_title->top_left.y + radius;
        arc(*center_corner, radius, 90, 180, nb_segments, &points_arc);
        for (int i = 0; i < nb_segments; ++i) {
                points[i + nb_segments + 1].x = points_arc[i].x;
                points[i + nb_segments + 1].y = points_arc[i].y;
        }

        center_corner->x = clipper_title->top_left.x ;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[nb_segments*2 + 1].x = center_corner->x;
        points[nb_segments*2 + 1].y = center_corner->y;
        size_t points_size = 2*nb_segments + 2;

        ei_draw_polygon(surface, points, points_size,
                        *(child->widget.pick_color), clipper);

        ei_rect_t* clipper_content = malloc(sizeof (ei_rect_t));
        clipper_content->top_left.x = clipper->top_left.x;
        clipper_content->top_left.y = clipper->top_left.y + height;

        clipper_content->size.width = clipper->size.width;
        clipper_content->size.height = clipper->size.height - height;

        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                       clipper_content);

        clipper_content->size.width -= child->border_width * 2;
        clipper_content->size.height -= child->border_width * 2;
        clipper_content->top_left.x += child->border_width;
        clipper_content->top_left.y += child->border_width;
        child->widget.pick_color->red = red;
        child->widget.pick_color->green = green;
        child->widget.pick_color->blue = blue;
        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                       clipper_content);


        ei_point_t where = child->widget.screen_location.top_left;
        where.x += 30;
        ei_draw_text(surface, &where, child->title, child->title_font, child->title_color, clipper);

}

void		ei_impl_widget_draw_children	(ei_widget_t		widget,
                                                         ei_surface_t		surface,
                                                         ei_surface_t		pick_surface,
                                                         ei_rect_t*		clipper){
        ei_widget_t type = (ei_widget_get_first_child(widget));
        while (type != NULL) {
                clipper = &(type->screen_location);
                if (strcmp(type->wclass->name, "frame") == 0) {
                        draw_frame((frame_t *) type, surface, pick_surface, clipper);
                } else if (strcmp(type->wclass->name, "button") == 0) {
                        draw_button((button_t *) type, surface, pick_surface, clipper);
                } else if (strcmp(type->wclass->name, "toplevel") == 0) {
                        draw_toplevel((toplevel_t *) type, surface, pick_surface, clipper);
                }
                type = ei_widget_get_first_child(type);
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


