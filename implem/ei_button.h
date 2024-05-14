#ifndef PROJETC_IG_EI_BUTTON_H
#define PROJETC_IG_EI_BUTTON_H

#include "ei_implementation.h"

typedef struct button_t {
        ei_impl_widget_t widget;
        ei_color_t color;
        int border_width;
        ei_relief_t relief;
        int corner_radius;
        ei_color_t text_color;
        ei_font_t text_font;
        ei_string_t text;
        ei_anchor_t text_anchor;
} button_t;

ei_widget_t button_allocfunc ();

void button_releasefunc (ei_widget_t	widget);

void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

void button_setdefaultsfunc(ei_widget_t		widget);

ei_widgetclass_t* create_button_class();

#endif //PROJETC_IG_EI_BUTTON_H
