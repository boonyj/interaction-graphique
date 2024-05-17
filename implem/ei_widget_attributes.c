#include "ei_widget_attributes.h"
#include "ei_implementation.h"
#include "ei_global.h"

ei_widgetclass_t*	ei_widget_get_class		(ei_widget_t		widget) {
        return widget->wclass;
}

const ei_color_t*	ei_widget_get_pick_color	(ei_widget_t		widget) {
        if (widget->pick_color != NULL) {
                return widget->pick_color;
        }
        return NULL;
}

ei_widget_t 		ei_widget_get_parent		(ei_widget_t		widget) {
        if (widget != root) {
                return widget->parent;
        }
        return NULL;
}

ei_widget_t 		ei_widget_get_first_child	(ei_widget_t		widget) {
        if (widget->children_head != NULL) {
                return widget->children_head;
        }
        return NULL;
}

ei_widget_t 		ei_widget_get_last_child	(ei_widget_t		widget) {
        if (widget->children_head != NULL) {
                return widget->children_tail;
        }
        return NULL;
}

ei_widget_t 		ei_widget_get_next_sibling	(ei_widget_t		widget) {
        if (widget!= root && widget != ei_widget_get_last_child(widget->parent)) {
                return widget->next_sibling;
        }
        return NULL;
}

void*			ei_widget_get_user_data		(ei_widget_t		widget) {
        return widget->user_data;
}

const ei_size_t*	ei_widget_get_requested_size	(ei_widget_t		widget) {
        return &(widget->requested_size);
}

void	 		ei_widget_set_requested_size	(ei_widget_t		widget,
                                                                 ei_size_t 		requested_size) {
        widget->requested_size.height = requested_size.height;
        widget->requested_size.width = requested_size.width;
}

const ei_rect_t*	ei_widget_get_screen_location	(ei_widget_t		widget) {
        return &(widget->screen_location);
}

const ei_rect_t*	ei_widget_get_content_rect	(ei_widget_t		widget) {
        return widget->content_rect;
}

void	 		ei_widget_set_content_rect	(ei_widget_t		widget,
                                                               const ei_rect_t*	content_rect) {
        widget->content_rect->size.height = content_rect->size.height;
        widget->content_rect->size.width = content_rect->size.width;
        widget->content_rect->top_left.x = content_rect->top_left.x;
        widget->content_rect->top_left.y = content_rect->top_left.y;
}