#include "ei_frame.h"
#include "ei_draw.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct frame_t));

        return widget;
}

void frame_releasefunc (ei_widget_t	widget){

}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        ei_fill(surface,widget->pick_color,clipper);
}

void frame_setdefaultsfunc(ei_widget_t		widget){
}

ei_widgetclass_t* create_frame() {
        ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(frame->name, "frame");
        frame->allocfunc =  &frame_allocfunc;
        frame->releasefunc = &frame_releasefunc;
        frame->drawfunc = &frame_drawfunc;
        frame->setdefaultsfunc = &frame_setdefaultsfunc;

        return frame;
}