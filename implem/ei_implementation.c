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

void assertion_color(ei_color_t* child_color, ei_color_t color, int mode){
        if (mode == 0){
                child_color->red = color.red;
                child_color->green = color.green;
                child_color->blue = color.blue;
        } else if (mode == 1){
                child_color->red = light(color.red);
                child_color->green = light(color.green);
                child_color->blue = light(color.blue);
        } else if (mode == 2){
                child_color->red = dark(color.red);
                child_color->green = dark(color.green);
                child_color->blue = dark(color.blue);
        }

}

int nb_segments = 32;

void draw_text(ei_string_t text, ei_font_t text_font, ei_color_t text_color, ei_point_t top_left, ei_size_t size, ei_surface_t surface, ei_rect_t* clipper){
        int width = 0;
        int height = 0;
        hw_text_compute_size(text,text_font, &width, &height);
        ei_point_t where = top_left;
        where.x += size.width/2 - width/2;
        where.y += size.height/2 - height/2;

        ei_draw_text(surface, &where, text, text_font, text_color, clipper);
}

void calculate_clipper_sans_border(ei_rect_t*	clipper, int border_width){
        clipper->size.width -= border_width * 2;
        clipper->size.height -= border_width * 2;
        clipper->top_left.x += border_width;
        clipper->top_left.y += border_width;
}

void calculate_clipper_avec_border(ei_rect_t*	clipper, int border_width){
        clipper->size.width += border_width * 2;
        clipper->size.height += border_width * 2;
        clipper->top_left.x -= border_width;
        clipper->top_left.y -= border_width;
}

void calculate_haut_sans_corner_radius(ei_point_t *points, ei_rect_t*clipper, ei_size_t size){
        points[0].x = clipper->top_left.x;
        points[0].y = clipper->top_left.y + size.height;
        points[1].x = clipper->top_left.x + size.width / 3;
        points[1].y = clipper->top_left.y + size.height / 2;
        points[2].x = clipper->top_left.x + size.width * 2 / 3;
        points[2].y = clipper->top_left.y + size.height / 2;
        points[3].x = clipper->top_left.x + size.width;
        points[3].y = clipper->top_left.y;
        points[4].x = clipper->top_left.x;
        points[4].y = clipper->top_left.y;
}

void draw_frame (frame_t* child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        if (child->widget.wclass->drawfunc != NULL) {
                ei_color_t color = {child->widget.color->red, child->widget.color->green, child->widget.color->blue};

                ei_point_t *points = NULL;
                size_t points_size;

                switch (child->relief) {
                        case ei_relief_none :
                                child->widget.wclass->drawfunc(&(child->widget), surface, pick_surface,
                                                               clipper);
                                break;
                        case ei_relief_raised :
                                assertion_color(child->widget.color, color, 1);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);

                                points_size = 5;

                                ei_draw_polygon(surface, points, points_size,
                                                *child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *child->widget.pick_color, clipper);
                                }

                                assertion_color(child->widget.color, color, 2);


                                points[4].x = clipper->top_left.x + child->widget.screen_location.size.width;
                                points[4].y = clipper->top_left.y + child->widget.screen_location.size.height;

                                ei_draw_polygon(surface, points, points_size,
                                                *child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *child->widget.pick_color, clipper);
                                }

                                calculate_clipper_sans_border(clipper, child->border_width);

                                assertion_color(child->widget.color, color, 0);

                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                               clipper);

                                calculate_clipper_avec_border(clipper, child->border_width);
                                break;
                        case ei_relief_sunken :
                                assertion_color(child->widget.color, color, 2);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);

                                points_size = 5;

                                ei_draw_polygon(surface, points, points_size,
                                                *child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *child->widget.pick_color, clipper);
                                }

                                assertion_color(child->widget.color, color, 1);


                                points[4].x = clipper->top_left.x + child->widget.screen_location.size.width;
                                points[4].y = clipper->top_left.y + child->widget.screen_location.size.height;

                                ei_draw_polygon(surface, points, points_size,
                                                *child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *child->widget.pick_color, clipper);
                                }

                                calculate_clipper_sans_border(clipper, child->border_width);

                                assertion_color(child->widget.color, color, 0);

                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                               clipper);

                                calculate_clipper_avec_border(clipper, child->border_width);
                                break;
                }
                if (child->text != NULL){
                        draw_text(child->text, child->text_font, child->text_color, child->widget.screen_location.top_left,
                                  child->widget.screen_location.size, surface, clipper);
                }
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

void generate_rounded_corner(ei_point_t center_corner, int radius, float start_angle, float end_angle,
                             ei_point_t *points, int start_pos) {
        ei_point_t *points_arc = NULL;

        arc(center_corner, radius, start_angle, end_angle, nb_segments, &points_arc);

        for (int i = 0; i < nb_segments; ++i) {
                points[start_pos + i].x = points_arc[i].x;
                points[start_pos + i].y = points_arc[i].y;
        }
}

ei_point_t* rounded_frame (ei_rect_t rect, int radius, int partie) {
        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points = NULL;
        int top_left_x = rect.top_left.x;
        int top_left_y = rect.top_left.y;
        int size_width = rect.size.width;
        int size_height = rect.size.height;

        if (partie == 0) {
                points = malloc(4*nb_segments * sizeof(ei_point_t));

                center_corner->x = top_left_x +  size_width  - radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 0, 90, points,0);

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 90, 180, points, nb_segments);

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 180, 270, points, 2*nb_segments);

                center_corner->x = top_left_x +  size_width  - radius;
                center_corner->y = top_left_y +  size_height  - radius;
                generate_rounded_corner(*center_corner, radius, 270, 360, points, 3*nb_segments);

        } else if (partie == 1) {
                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 180, 225, points, 0);

                points[nb_segments].x = top_left_x + size_width / 3;
                points[nb_segments].y = top_left_y + size_height / 2;

                points[nb_segments+1].x = top_left_x + size_width * 2 / 3;
                points[nb_segments+1].y = top_left_y + size_height / 2;

                center_corner->x = top_left_x + size_width - radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 45, 90, points, nb_segments+2);

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 90, 180, points, 2*nb_segments+2);

        } else if (partie == 2) {
                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));

                center_corner->x = top_left_x + size_width - radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 0, 45, points, 0);

                points[nb_segments].x = top_left_x + size_width * 2 / 3;
                points[nb_segments].y = top_left_y + size_height / 2;

                points[nb_segments+1].x = top_left_x + size_width / 3;
                points[nb_segments+1].y = top_left_y + size_height / 2;

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 225, 270, points, nb_segments+2);

                center_corner->x = top_left_x +  size_width - radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 270, 360, points, 2*nb_segments+2);
        }
        return points;
}

void draw_button (button_t * child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        // Call the draw function for each child widget
        if (child->border_width != 0) {
                if (child->widget.wclass->drawfunc != NULL) {
                        ei_color_t color = {child->widget.color->red, child->widget.color->green, child->widget.color->blue};

                        switch (child->relief) {
                                case ei_relief_none :
                                        assertion_color(child->widget.color, color, 2);

                                        if (child->corner_radius == 0) {
                                                child->widget.wclass->drawfunc(&(child->widget), surface, pick_surface,
                                                                               clipper);

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);
                                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                               clipper);
                                        } else {
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 0);

                                                size_t points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                                case ei_relief_raised :
                                        assertion_color(child->widget.color, color, 1);

                                        if (child->corner_radius == 0){
                                                ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);
                                                size_t points_size = 5;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                assertion_color(child->widget.color, color, 2);


                                                points[4].x = clipper->top_left.x + child->widget.screen_location.size.width;
                                                points[4].y = clipper->top_left.y + child->widget.screen_location.size.height;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *child->widget.pick_color, clipper);

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                               clipper);
                                        } else {
                                                // Partie haute
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                size_t points_size = 3*nb_segments+2;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Partie basse
                                                assertion_color(child->widget.color, color, 2);

                                                points = rounded_frame(*clipper, child->corner_radius, 2);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Draw inside button
                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                                case ei_relief_sunken :
                                        assertion_color(child->widget.color, color, 2);

                                        if (child->corner_radius == 0){
                                                ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);
                                                size_t points_size = 5;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                assertion_color(child->widget.color, color, 1);

                                                points += 4;
                                                points->x = clipper->top_left.x +
                                                            child->widget.screen_location.size.width;
                                                points->y = clipper->top_left.y +
                                                            child->widget.screen_location.size.height;
                                                points -= 4;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                                                               clipper);
                                        } else {
                                                // Partie haute
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                size_t points_size = 3*nb_segments+2;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Partie basse
                                                assertion_color(child->widget.color, color, 1);

                                                points = rounded_frame(*clipper, child->corner_radius, 2);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Draw inside button
                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                        }
                        if (child->text != NULL){
                                draw_text(child->text, child->text_font, child->text_color, child->widget.screen_location.top_left,
                                          child->widget.screen_location.size, surface, clipper);
                        }
                }
        }
}

void draw_toplevel (toplevel_t * child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper) {

        ei_color_t color = {child->widget.color->red, child->widget.color->green, child->widget.color->blue};

        assertion_color(child->widget.color, color, 2);

        int width = 0;
        int height = 0;
        hw_text_compute_size(child->title,child->title_font, &width, &height);

        ei_rect_t* clipper_title = malloc(sizeof (ei_rect_t));
        clipper->size.width += child->border_width*2;
        clipper->size.height += child->border_width*2+height;
        clipper->top_left.x -= child->border_width;
        clipper->top_left.y -= child->border_width +height;


        clipper_title->top_left.x = clipper->top_left.x;
        clipper_title->top_left.y = clipper->top_left.y;

        clipper_title->size.width = clipper->size.width;
        clipper_title->size.height = height;


        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points = NULL;
        int radius = 15;
        points = malloc((2*nb_segments + 2) * sizeof(ei_point_t));

        center_corner->x = clipper_title->top_left.x + clipper_title->size.width;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[0].x = center_corner->x;
        points[0].y = center_corner->y;

        center_corner->x = clipper_title->top_left.x +  clipper_title->size.width  - radius;
        center_corner->y = clipper_title->top_left.y + radius;
        generate_rounded_corner(*center_corner, radius, 0, 90, points,1);

        center_corner->x = clipper_title->top_left.x + radius;
        center_corner->y = clipper_title->top_left.y + radius;
        generate_rounded_corner(*center_corner, radius, 90, 180, points,nb_segments + 1);

        center_corner->x = clipper_title->top_left.x ;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[nb_segments*2 + 1].x = center_corner->x;
        points[nb_segments*2 + 1].y = center_corner->y;

        size_t points_size = 2*nb_segments + 2;
        ei_draw_polygon(surface, points, points_size,
                        *child->widget.color, clipper);
        ei_draw_polygon(pick_surface, points, points_size,
                        *child->widget.pick_color, clipper);

        ei_rect_t* clipper_content = malloc(sizeof (ei_rect_t));
        clipper_content->top_left.x = clipper->top_left.x;
        clipper_content->top_left.y = clipper->top_left.y + height;

        clipper_content->size.width = clipper->size.width;
        clipper_content->size.height = clipper->size.height - height;

        child->widget.wclass->drawfunc(&(child->widget), surface, pick_surface,
                                       clipper_content);
        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                       clipper_content);


        calculate_clipper_sans_border(clipper_content, child->border_width);
        assertion_color(child->widget.color, color, 0);

        child->widget.wclass->drawfunc(&(child->widget), surface, NULL,
                                       clipper_content);


        ei_point_t where = child->widget.screen_location.top_left;
        where.x += 30;
        ei_draw_text(surface, &where, child->title, child->title_font, child->title_color, clipper);
        calculate_clipper_avec_border(clipper, child->border_width);
        child->widget.screen_location.top_left.y += height + 2*child->border_width;

}

void ei_impl_widget_draw_children(ei_widget_t widget,
                                  ei_surface_t surface,
                                  ei_surface_t pick_surface,
                                  ei_rect_t* clipper) {
        // Iterate through all children of the widget
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                clipper = &(child->screen_location);
                // Determine the widget type and call the appropriate drawing function
                if (strcmp(child->wclass->name, "frame") == 0) {
                        draw_frame((frame_t*)child, surface, pick_surface, clipper);
                } else if (strcmp(child->wclass->name, "button") == 0) {
                        draw_button((button_t*)child, surface, pick_surface, clipper);
                } else if (strcmp(child->wclass->name, "toplevel") == 0) {
                        draw_toplevel((toplevel_t*)child, surface, pick_surface, clipper);
                }

                // Recursively draw children of the current child widget
                ei_impl_widget_draw_children(child, surface, pick_surface, clipper);

                // Move to the next sibling
                child = ei_widget_get_next_sibling(child);
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


