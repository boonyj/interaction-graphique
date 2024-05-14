#include "ei_toplevel.h"
#include "ei_button.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"

ei_widget_t toplevel_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct toplevel_t ));

        return widget;
}

void toplevel_releasefunc (ei_widget_t	widget){

}

void toplevel_drawfunc (ei_widget_t		widget,
                      ei_surface_t		surface,
                      ei_surface_t		pick_surface,
                      ei_rect_t*		clipper){
        ei_fill(surface,&((toplevel_t*) widget)->color,clipper);
}

void toplevel_setdefaultsfunc(ei_widget_t		widget){
        toplevel_t * toplevel = (toplevel_t*) widget;
        toplevel->widget = *widget;
        toplevel->title = NULL;
        toplevel->title_font = ei_default_font;
        toplevel->title_color = ei_font_default_color;

}

ei_widgetclass_t* create_toplevel_class() {
        ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));
        strcpy(toplevel->name, "toplevel");
        toplevel->allocfunc =  &toplevel_allocfunc;
        toplevel->releasefunc = &toplevel_releasefunc;
        toplevel->drawfunc = &toplevel_drawfunc;
        toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;

        return toplevel;
}

