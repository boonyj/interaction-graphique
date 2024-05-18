#include "ei_widget.h"
#include "ei_implementation.h"

static uint32_t pick_counter = 0;

ei_widget_t ei_widget_create(ei_const_string_t class_name,
                             ei_widget_t parent,
                             ei_user_param_t user_data,
                             ei_widget_destructor_t destructor) {
        ei_widgetclass_t* type_widget = ei_widgetclass_from_name(class_name);
        ei_widget_t widget  = type_widget->allocfunc();
        widget->wclass = malloc(sizeof(struct ei_widgetclass_t));
        widget->pick_color = NULL;
        widget->color = malloc(sizeof(ei_color_t));
        widget->geom_params = NULL;

        strcpy(widget->wclass->name, class_name);
        widget->wclass->allocfunc = type_widget->allocfunc;
        widget->wclass->drawfunc = type_widget->drawfunc;
        widget->wclass->releasefunc = type_widget->releasefunc;
        widget->wclass->setdefaultsfunc = type_widget->setdefaultsfunc;
        widget->wclass->geomnotifyfunc = type_widget->geomnotifyfunc;

        widget->parent = parent; // Assign parent

        widget->parent = parent; // Assign parent
        widget->user_data = user_data; // Assign user_data
        widget->destructor = destructor; // Assign destructor

        widget->children_head = NULL; // Initialize children_head
        widget->children_tail = NULL; // Initialize children_tail
        widget->next_sibling = NULL; // Initialize next_sibling

        if (parent != NULL) {
                if (parent->children_head == NULL) {
                        parent->children_head = widget;
                }
                if (parent->children_tail != NULL) {
                        parent->children_tail->next_sibling = widget;
                }
                parent->children_tail = widget;

                /*if (parent->children_head != widget) {
                        ei_widget_t prev_sibling = parent->children_head;
                        while (prev_sibling->next_sibling != NULL) {
                                prev_sibling = prev_sibling->next_sibling;
                        }
                        prev_sibling->next_sibling = widget;
                }*/
        }

        pick_counter+= 1;
        widget->pick_id = pick_counter;

        widget->wclass->setdefaultsfunc(widget);

        return widget;
}



void			ei_widget_destroy		(ei_widget_t		widget){}


bool	 		ei_widget_is_displayed		(ei_widget_t		widget){}


ei_widget_t		ei_widget_pick			(ei_point_t*		where){}

