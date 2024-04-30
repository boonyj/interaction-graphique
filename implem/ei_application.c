#include "ei_application.h"


ei_widget_t frame_allocfunc (){
}

void frame_releasefunc (ei_widget_t	widget){
}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
}

void frame_setdefaultsfunc(ei_widget_t		widget){
}

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        hw_init();
        ei_surface_t main_window = hw_create_window(main_window_size, false);

        ei_widgetclass_t frame;
        frame.allocfunc =  &frame_allocfunc;
        frame.releasefunc = &frame_releasefunc;
        frame.drawfunc = &frame_drawfunc;
        frame.setdefaultsfunc = &frame_setdefaultsfunc;
        ei_widgetclass_register(&frame);

}

void ei_app_free(void){
        hw_quit();
}

void ei_app_run(void){
        getchar();
}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

ei_widget_t ei_app_root_widget(void){

}

ei_surface_t ei_app_root_surface(void){

}
