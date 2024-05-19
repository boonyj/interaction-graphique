#ifndef PROJETC_IG_EI_ENTRY_CLASS_H
#define PROJETC_IG_EI_ENTRY_CLASS_H

#include "ei_implementation.h"

typedef struct entry_t {
        ei_impl_widget_t widget;
        int requested_char_size;
        int border_width;
        ei_color_t text_color;
        ei_font_t text_font;
        char* text;
        bool in_focus;
} entry_t;

ei_widget_t entry_allocfunc ();

void entry_releasefunc (ei_widget_t	widget);

void entry_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

void entry_setdefaultsfunc(ei_widget_t		widget);

void entry_geomnotifyfunc(ei_widget_t		widget);

ei_widgetclass_t* create_entry_class();

#endif //PROJETC_IG_EI_ENTRY_CLASS_H
