#ifndef PROJETC_IG_EI_FRAME_H
#define PROJETC_IG_EI_FRAME_H

#include "ei_application.h"
#include "ei_implementation.h"

typedef struct frame_t {
        ei_widget_t widget;
} frame_t;

ei_widget_t frame_allocfunc ();

void frame_releasefunc (ei_widget_t	widget);

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

void frame_setdefaultsfunc(ei_widget_t		widget);

ei_widgetclass_t* create_frame();

#endif //PROJETC_IG_EI_FRAME_H
