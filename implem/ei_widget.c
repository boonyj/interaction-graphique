#include "ei_widget.h"
#include "ei_implementation.h"
#include "ei_global.h"
#include "ei_widget_attributes.h"

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

void			ei_widget_destroy		(ei_widget_t		widget){
        widget->wclass->releasefunc(widget);
        free_widget_and_siblings(&widget, true);
        if(widget->parent){
                if (widget->parent->children_head == widget) {
                        if (widget->next_sibling != NULL) {
                                widget->parent->children_head = widget->next_sibling;
                        } else {
                                widget->parent->children_head = NULL;
                                widget->parent->children_tail = NULL;
                        }
                }  else {
                        ei_widget_t prev = widget->parent->children_head;
                        while (prev && prev->next_sibling != widget) {
                                prev = prev->next_sibling;
                        }
                        if (prev) {
                                prev->next_sibling = widget->next_sibling;
                                if (widget->parent->children_tail == widget) {
                                        widget->parent->children_tail = prev;
                                }
                        }
                }
        }

        if (widget->destructor != NULL) {
                widget->destructor(widget);
        }

        free(widget->color);
        free(widget->pick_color);
        free(widget->wclass);
        free(widget->geom_params->manager);
        free(widget->geom_params);
        widget->geom_params = NULL;
        //hw_surface_free(widget->content_rect);
        free(widget);
}



bool	 		ei_widget_is_displayed		(ei_widget_t		widget){
        if(widget->geom_params != NULL){
                return true;
        }
        return false;
}


ei_widget_t find_widget (uint32_t* pixel_pick_surface, ei_widget_t widget) {
        // Check the widget itself first
        printf("Name: %s, Pick id : %u\n", widget->wclass->name, widget->pick_id);

        if (widget->pick_id == *pixel_pick_surface) {
                //printf("Name: %s, Pick id : %u, Pixel : %u\n", widget->wclass->name, widget->pick_id, *pixel_pick_surface);
                return widget;
        }

        // Recursively search among the widget's children
        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                ei_widget_t found_widget = find_widget(pixel_pick_surface, child);
                if (found_widget != NULL) {
                        return found_widget;
                }
                child = ei_widget_get_next_sibling(child);
        }

        // Return NULL if no matching widget is found
        return NULL;
}

ei_widget_t		ei_widget_pick			(ei_point_t*		where){
        uint32_t* pixel_pick_surface = (uint32_t*)hw_surface_get_buffer(pick_surface);
        ei_size_t pick_size = hw_surface_get_size(pick_surface);
        pixel_pick_surface += (where->y * pick_size.width) + (where->x);
        ei_widget_t widget = find_widget(pixel_pick_surface, root);
        return widget;
}

