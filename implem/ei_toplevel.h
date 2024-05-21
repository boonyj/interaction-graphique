#ifndef PROJETC_IG_TOPLEVEL_H
#define PROJETC_IG_TOPLEVEL_H

#include "ei_implementation.h"
#include "ei_draw_tool.h"
#include "ei_placer.h"

typedef struct toplevel_t {
        ei_impl_widget_t widget;
        int border_width;
        ei_string_t title;
        ei_color_t title_color;
        ei_font_t title_font;
        bool closable;
        ei_axis_set_t resizable;
        ei_size_t min_size;
} toplevel_t;

ei_widget_t toplevel_allocfunc ();

void toplevel_releasefunc (ei_widget_t	widget);

void toplevel_drawfunc (ei_widget_t		widget,
                      ei_surface_t		surface,
                      ei_surface_t		pick_surface,
                      ei_rect_t*		clipper);

void toplevel_setdefaultsfunc(ei_widget_t		widget);

void toplevel_geomnotifyfunc(ei_widget_t		widget);

ei_widgetclass_t* create_toplevel_class();

#endif //PROJETC_IG_TOPLEVEL_H
