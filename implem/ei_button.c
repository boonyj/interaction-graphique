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
        if (surface != NULL) {
                ei_fill(surface, widget->color, clipper);
        }
        if (pick_surface != NULL) {
                ei_fill(pick_surface, widget->pick_color, clipper);
        }
}

void button_setdefaultsfunc(ei_widget_t		widget){
        button_t * button = (button_t*) widget;
        button->widget = *widget;
        button->relief = ei_relief_none;
        button->border_width = 0;
        button->corner_radius = 0;
        button->text_color = ei_font_default_color;
        button->text_anchor = ei_anc_none;
        button->text_font = ei_default_font;
        button->text = NULL;
        button->widget.requested_size.width = 100;
        button->widget.requested_size.height = 48;

}

ei_widgetclass_t* create_button_class() {
        ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
        strcpy(button->name, "button");
        button->allocfunc =  &button_allocfunc;
        button->releasefunc = &button_releasefunc;
        button->drawfunc = &button_drawfunc;
        button->setdefaultsfunc = &button_setdefaultsfunc;

        return button;
}
