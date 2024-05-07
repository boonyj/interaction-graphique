#include "ei_frame.h"
#include "ei_draw.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = calloc(1,sizeof(struct frame_t));
        return widget;
}

void frame_releasefunc (ei_widget_t	widget){
        free(widget->pick_color);
        free(widget->pick_color);

        widget = NULL;
}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        ei_fill(surface,widget->pick_color,clipper);

        // //Test polyline (à décommenter pour tester)
        // // START TEST POLYLINE
        //   ei_point_t* points = malloc(2*sizeof(ei_point_t));
        //   points->x = 20;
        //   points->y = 20;
        //   points++;
        //   points->x = 150;
        //   points->y = 200;
        //   points--;
        //   size_t array_size = 2;
        //   ei_draw_polyline(surface,points,array_size,*(widget->pick_color),clipper);
        // // END TEST POLYLINE
}

void frame_setdefaultsfunc(ei_widget_t		widget){
        frame_t* frame = (frame_t*) widget;
        frame->widget = *widget;
        frame->relief = ei_relief_none;
        frame->border_width = 0;
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