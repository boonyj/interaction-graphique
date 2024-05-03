#include "ei_widget.h"
#include "ei_implementation.h"

ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                                            ei_widget_t		parent,
                                                            ei_user_param_t	user_data,
                                                            ei_widget_destructor_t destructor){
        ei_widget_t widget = malloc(sizeof(ei_widget_t));
        widget->wclass = malloc(sizeof(ei_widgetclass_t*));
        widget->pick_color = malloc(sizeof(ei_color_t*));
        widget->parent = malloc(sizeof(ei_impl_widget_t*));
        widget->user_data = malloc(sizeof(ei_user_param_t));
        strcpy(widget->wclass->name, class_name);
        widget->parent = parent;
        widget->user_data = user_data;
        widget->destructor = destructor;
        widget->children_head = NULL;
        widget->children_tail = NULL;
        widget->next_sibling = NULL;

        return widget;
}


void			ei_widget_destroy		(ei_widget_t		widget){}


bool	 		ei_widget_is_displayed		(ei_widget_t		widget){}


ei_widget_t		ei_widget_pick			(ei_point_t*		where){}

