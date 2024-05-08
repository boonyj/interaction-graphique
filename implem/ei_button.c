#include "ei_button.h"

ei_widget_t button_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct button_t));
        return widget;
}

void button_releasefunc (ei_widget_t	widget){

}

void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        ei_fill(surface,widget->pick_color,clipper);
}

void button_setdefaultsfunc(ei_widget_t		widget){
        button_t * button = (button_t*) widget;
        button->widget = *widget;
        button->relief = ei_relief_none;
        button->border_width = 0;
        button->corner_radius = 0;
        button->text_color = ei_font_default_color;
}

ei_widgetclass_t* create_button() {
        ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
        strcpy(button->name, "button");
        button->allocfunc =  &button_allocfunc;
        button->releasefunc = &button_releasefunc;
        button->drawfunc = &button_drawfunc;
        button->setdefaultsfunc = &button_setdefaultsfunc;

        return button;
}
