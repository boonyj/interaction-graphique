#ifndef PROJETC_IG_EI_DRAW_TOOL_H
#define PROJETC_IG_EI_DRAW_TOOL_H

#include <math.h>
#include "ei_implementation.h"

#define PI 3.14159265
extern int nb_segments;

unsigned char dark(unsigned char colorComponent);

unsigned char light(unsigned char colorComponent);

void assertion_color(ei_color_t* child_color, ei_color_t color, int mode);

void draw_text(ei_string_t text, ei_font_t text_font, ei_color_t text_color, ei_point_t top_left, ei_size_t size, ei_surface_t surface, ei_rect_t* clipper);

void calculate_clipper_sans_border(ei_rect_t*	clipper, int border_width);

void calculate_clipper_avec_border(ei_rect_t*	clipper, int border_width);

void calculate_haut_sans_corner_radius(ei_point_t *points, ei_rect_t*clipper, ei_size_t size);

void arc(ei_point_t center, int radius, double start_angle, double end_angle, int nb_segments, ei_point_t** arc_points) ;

void generate_rounded_corner(ei_point_t center_corner, int radius, float start_angle, float end_angle,
                             ei_point_t *points, int start_pos);

ei_point_t* rounded_frame (ei_rect_t rect, int radius, int partie);

#endif //PROJETC_IG_EI_DRAW_TOOL_H
