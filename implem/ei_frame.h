#ifndef PROJETC_IG_EI_FRAME_H
#define PROJETC_IG_EI_FRAME_H

#include "ei_implementation.h"

typedef struct frame_t {
        ei_impl_widget_t widget;
        int border_width;
        ei_relief_t relief;
        ei_color_t text_color;
        ei_font_t text_font;
        ei_string_t text;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t img_rect;
        ei_anchor_t img_anchor;
} frame_t;

ei_widget_t frame_allocfunc ();

void frame_releasefunc (ei_widget_t	widget);

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

void frame_setdefaultsfunc(ei_widget_t		widget);

ei_widgetclass_t* create_frame_class();

#endif //PROJETC_IG_EI_FRAME_H
