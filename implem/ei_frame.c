#include "ei_frame.h"
#include "ei_draw.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = calloc(1,sizeof(struct frame_t));
        return widget;
}

void frame_releasefunc (ei_widget_t	widget){
}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        if (surface != NULL) {
                ei_fill(surface, widget->color, clipper);
        }
        if (pick_surface != NULL) {
                ei_fill(pick_surface, widget->pick_color, clipper);
        }
}

void frame_setdefaultsfunc(ei_widget_t		widget){
        frame_t* frame = (frame_t*) widget;
        frame->widget = *widget;
        frame->relief = ei_relief_none;
        frame->border_width = 0;
}

ei_widgetclass_t* create_frame_class() {
        ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(frame->name, "frame");
        frame->allocfunc =  &frame_allocfunc;
        frame->releasefunc = &frame_releasefunc;
        frame->drawfunc = &frame_drawfunc;
        frame->setdefaultsfunc = &frame_setdefaultsfunc;

        return frame;
}