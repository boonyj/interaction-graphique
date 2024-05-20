#include "ei_callback_button.h"

ei_widget_t find_last_widget_in_hierarchy(ei_widget_t widget) {
        if (widget == NULL) {
                return NULL;
        }

        ei_widget_t max_pick_id_widget = widget;
        ei_widget_t child = widget->children_head;

        while (child != NULL) {
                ei_widget_t largest_in_subtree = find_last_widget_in_hierarchy(child);
                if (largest_in_subtree != NULL && largest_in_subtree->pick_id > max_pick_id_widget->pick_id) {
                        max_pick_id_widget = largest_in_subtree;
                }
                child = child->next_sibling;
        }

        return max_pick_id_widget;
}

bool callback_buttonup_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                ei_widget_t last_widget = find_last_widget_in_hierarchy(root);
                button_t *button = (button_t *) user_param;
                if (last_widget != NULL) {
                        if (button->widget.pick_id <= last_widget->pick_id) {
                                button->relief = ei_relief_raised;
                                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                        }
                }
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, user_param);
                return true;
        } else
                return false;
}

bool callback_buttondown_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                button_t *button = (button_t *) widget;
                button->relief = ei_relief_sunken;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                return true;
        } else
                return false;
}