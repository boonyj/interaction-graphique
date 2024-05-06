#include "ei_frame.h"

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

        uint32_t* pixel_ptr = (uint32_t*)hw_surface_get_buffer(surface);
        ei_size_t size = hw_surface_get_size(surface);

        for (int i = 0; i < size.width * size.height; i++) { // Iterate over each pixel in the buffer
                *pixel_ptr++ = (widget->pick_color->alpha << 24) | // Alpha component
                               (widget->pick_color->red << 16) |   // Red component
                               (widget->pick_color->green << 8) |  // Green component
                               widget->pick_color->blue;           // Blue component
        }


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