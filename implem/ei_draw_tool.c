#include "ei_draw_tool.h"

int nb_segments = 8;

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

void assertion_pick_color(ei_widget_t widget, ei_surface_t surface){
        if(widget->pick_color == NULL){
                uint8_t *p = (uint8_t *) &(widget->pick_id);
                widget->pick_color = malloc(sizeof(ei_color_t));
                int ir, ig, ib, ia;
                hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);

                // Assign extracted components to the pick_color
                widget->pick_color->red = p[ir];
                widget->pick_color->green = p[ig];
                widget->pick_color->blue = p[ib];
                widget->pick_color->alpha = 255;
        }
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

void draw_text(ei_string_t text, ei_font_t text_font, ei_color_t text_color, ei_rect_t * screen_location, ei_surface_t surface,
               ei_rect_t* clipper, ei_anchor_t text_anchor) {
        int width = 0;
        int height = 0;

        // Compute the size of the text.
        hw_text_compute_size(text, text_font, &width, &height);
        ei_point_t top_left = screen_location->top_left;
        ei_size_t size = screen_location->size;

        // Adjust the position based on the anchor type.
        switch (text_anchor) {
                case ei_anc_center:
                        top_left.x += size.width / 2 - width / 2;
                        top_left.y += size.height / 2 - height / 2;
                        break;
                case ei_anc_north:
                        top_left.x += size.width / 2 - width / 2;
                        break;
                case ei_anc_south:
                        top_left.x += size.width / 2 - width / 2;
                        top_left.y += size.height - height;
                        break;
                case ei_anc_east:
                        top_left.x += size.width - width;
                        top_left.y += size.height / 2 - height / 2;
                        break;
                case ei_anc_west:
                        top_left.y += size.height / 2 - height / 2;
                        break;
                case ei_anc_northeast:
                        top_left.x += size.width - width;
                        break;
                case ei_anc_northwest:
                        // No change needed, already top-left corner.
                        break;
                case ei_anc_southeast:
                        top_left.x += size.width - width;
                        top_left.y += size.height - height;
                        break;
                case ei_anc_southwest:
                        top_left.y += size.height - height;
                        break;
                default:
                        // Handle default case, assume top-left anchor if not specified.
                        break;
        }
        // Draw the text at the computed position.
        ei_draw_text(surface, &top_left, text, text_font, text_color, clipper);
}

void draw_image_from_surface(ei_surface_t surface, ei_surface_t image, ei_rect_t * where,
                             ei_rect_t* img_rect, ei_anchor_t img_anchor) {

        ei_rect_t dst_rect = hw_surface_get_rect(surface);
        dst_rect.top_left.x = where->top_left.x;
        dst_rect.top_left.y = where->top_left.y;

        switch (img_anchor) {
                case ei_anc_center:
                        dst_rect.top_left.x += where->size.width / 2 - img_rect->size.width / 2;
                        dst_rect.top_left.y += where->size.height / 2 - img_rect->size.height / 2;
                        break;
                case ei_anc_north:
                        dst_rect.top_left.x += where->size.width / 2 - img_rect->size.width / 2;
                        break;
                case ei_anc_south:
                        dst_rect.top_left.x += where->size.width / 2 - img_rect->size.width / 2;
                        dst_rect.top_left.y += where->size.height - img_rect->size.height;
                        break;
                case ei_anc_east:
                        dst_rect.top_left.x += where->size.width - img_rect->size.width;
                        dst_rect.top_left.y += where->size.height / 2 - img_rect->size.height / 2;
                        break;
                case ei_anc_west:
                        dst_rect.top_left.y += where->size.height / 2 - img_rect->size.height / 2;
                        break;
                case ei_anc_northeast:
                        dst_rect.top_left.x += where->size.width - img_rect->size.width;
                        break;
                case ei_anc_northwest:
                        // No change needed, already top-left corner.
                        break;
                case ei_anc_southeast:
                        dst_rect.top_left.x += where->size.width - img_rect->size.width;
                        dst_rect.top_left.y += where->size.height - img_rect->size.height;
                        break;
                case ei_anc_southwest:
                        dst_rect.top_left.y += where->size.height - img_rect->size.height;
                        break;
                default:
                        // Handle default case, assume top-left anchor if not specified.
                        break;
        }

        dst_rect.size.width = img_rect->size.width;
        dst_rect.size.height = img_rect->size.height;

        ei_copy_surface(surface, &dst_rect, image, img_rect, true);
}

void calculate_clipper_without_border(ei_rect_t*	clipper, int border_width){
        clipper->size.width -= border_width * 2;
        clipper->size.height -= border_width * 2;
        clipper->top_left.x += border_width;
        clipper->top_left.y += border_width;
}

void calculate_clipper_with_border(ei_rect_t*	clipper, int border_width){
        clipper->size.width += border_width * 2;
        clipper->size.height += border_width * 2;
        clipper->top_left.x -= border_width;
        clipper->top_left.y -= border_width;
}

void calculate_top_without_corner_radius(ei_point_t *points, ei_rect_t*clipper, ei_size_t size){
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

void arc(ei_point_t center, int radius, double start_angle, double end_angle, int segments, ei_point_t** arc_points) {
        // Allouer de la mémoire pour le tableau de points de l'arc
        *arc_points = malloc(segments * sizeof(ei_point_t));

        double angle_step = (end_angle - start_angle) / segments;

        // Générer les points de l'arc en utilisant la formule paramétrique d'un cercle
        for (int i = 0; i < segments; ++i) {
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
        free(points_arc);
}

ei_point_t* rounded_frame (ei_rect_t rect, int radius, int part) {
        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points = NULL;
        int top_left_x = rect.top_left.x;
        int top_left_y = rect.top_left.y;
        int size_width = rect.size.width;
        int size_height = rect.size.height;

        if (part == 0) {
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

        } else if (part == 1) {
                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 180, 230, points, 0);

                points[nb_segments].x = top_left_x + size_width / 3;
                points[nb_segments].y = top_left_y + size_height / 2;

                points[nb_segments+1].x = top_left_x + size_width * 2 / 3;
                points[nb_segments+1].y = top_left_y + size_height / 2;

                center_corner->x = top_left_x + size_width - radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 40, 90, points, nb_segments+2);

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 90, 180, points, 2*nb_segments+2);

        } else if (part == 2) {
                points = malloc((3*nb_segments+2) * sizeof(ei_point_t));

                center_corner->x = top_left_x + size_width - radius;
                center_corner->y = top_left_y + radius;
                generate_rounded_corner(*center_corner, radius, 0, 50, points, 0);

                points[nb_segments].x = top_left_x + size_width * 2 / 3;
                points[nb_segments].y = top_left_y + size_height / 2;

                points[nb_segments+1].x = top_left_x + size_width / 3;
                points[nb_segments+1].y = top_left_y + size_height / 2;

                center_corner->x = top_left_x + radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 220, 270, points, nb_segments+2);

                center_corner->x = top_left_x +  size_width - radius;
                center_corner->y = top_left_y + size_height - radius;
                generate_rounded_corner(*center_corner, radius, 270, 360, points, 2*nb_segments+2);
        }
        free(center_corner);
        return points;
}