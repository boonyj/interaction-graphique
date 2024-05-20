#ifndef PROJETC_IG_EI_BUTTON_H
#define PROJETC_IG_EI_BUTTON_H

#include "ei_implementation.h"
#include "ei_draw_tool.h"
#include "ei_widget_configure.h"

typedef struct button_t {
        ei_impl_widget_t widget;
        int border_width;
        ei_relief_t relief;
        int corner_radius;
        ei_color_t text_color;
        ei_font_t text_font;
        ei_string_t text;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t img_rect;
        ei_anchor_t img_anchor;
        ei_callback_t	callback;
        ei_user_param_t	user_param;
} button_t;

ei_widget_t button_allocfunc ();

void button_releasefunc (ei_widget_t	widget);

void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

void button_setdefaultsfunc(ei_widget_t		widget);

void button_geomnotifyfunc(ei_widget_t		widget);

ei_widgetclass_t* create_button_class();

#endif //PROJETC_IG_EI_BUTTON_H
