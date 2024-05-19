#include "ei_callback_functions.h"
#include "ei_widget_attributes.h"
#include "ei_placer.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_global.h"
#include "ei_toplevel.h"

//Callbacks for toplevel movement

bool callback_move_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        // Only proceed if the event is a mouse button down event
        if (event->type == ei_ev_mouse_move) {
                // Cast the widget to a toplevel widget
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                ei_mouse_event_t initial_mouse = initial_event_bind->event->param.mouse;
                ei_mouse_event_t mouse = event->param.mouse;

                ei_widget_t toplevel = (ei_widget_t) initial_event_bind->widget;

                // Check if the clicked widget is the toplevel widget
                if (widget == toplevel) {
                        // Get the current mouse position
                        ei_point_t mouse_position = event->param.mouse.where;

                        // Calculate the offset between the mouse position and the toplevel position
                        int dx = mouse_position.x - initial_mouse.where.x;
                        int dy = mouse_position.y - initial_mouse.where.y;
                        ei_place_xy(widget, dx, dy);
                        run_all_ei_place(widget);

                        //ei_place(widget->children_head->next_sibling->next_sibling, NULL, &dx, &dy, NULL, NULL, NULL, NULL, NULL, NULL);

                       if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        ei_rect_t *clipper = &(root->children_head->screen_location);
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                        // Return true to indicate that the event was handled
                        return true;
                }
        } else if(event->type == ei_ev_mouse_buttonup) {
                root->screen_location.top_left.x = 0;
                root->screen_location.top_left.y = 0;
                ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_toplevel, user_param);
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_toplevel, NULL);
                return true;
        }
        // Return false if the event was not handled
        return false;
}

bool callback_buttondown_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                toplevel_t* toplevel = (toplevel_t *) widget;
                int width = 0;
                int height = 0;
                hw_text_compute_size(toplevel->title,toplevel->title_font, &width, &height);
                if( event->param.mouse.where.x > widget->screen_location.top_left.x
                        && event->param.mouse.where.x < widget->screen_location.top_left.x +widget->screen_location.size.width
                        && event->param.mouse.where.y > widget->screen_location.top_left.y - height
                        && event->param.mouse.where.y < widget->screen_location.top_left.y  ){
                        ei_event_t* event_tbs = malloc(sizeof(ei_event_t));
                        event_tbs->type = event->type;
                        event_tbs->param = event->param;
                        event_tbs->modifier_mask = event->modifier_mask;
                        ei_event_bind_widget_t* param = malloc(sizeof(ei_event_bind_widget_t));
                        param->event = event_tbs;
                        root->screen_location.top_left.x = widget->screen_location.top_left.x;
                        root->screen_location.top_left.y = widget->screen_location.top_left.y;
                        param->widget = widget;
                        ei_bind(ei_ev_mouse_move, NULL, "all", callback_move_toplevel, param);
                        ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_move_toplevel, NULL);
                }

                return true;
        } else
                return false;
}

//Callbacks for toplevel resizing

bool callback_move_resizing_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        // Only proceed if the event is a mouse button down event
        if (event->type == ei_ev_mouse_move) {
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                ei_mouse_event_t initial_mouse = initial_event_bind->event->param.mouse;

                toplevel_t* tl = (toplevel_t*) initial_event_bind->widget;
                ei_widget_t toplevel = &tl->widget;

                bool is_resizable = (tl->resizable == ei_axis_x || tl->resizable == ei_axis_y || tl->resizable == ei_axis_both);

                if(is_resizable) {
                        // Get the current mouse position
                        ei_point_t mouse_position = event->param.mouse.where;

                        // Calculate the offset between the mouse position and the toplevel position
                        int dx = mouse_position.x - initial_mouse.where.x;
                        int dy = mouse_position.y - initial_mouse.where.y;
                        // ei_place_xy(widget, dx, dy);

                        toplevel->screen_location.size.width += (tl->resizable != ei_axis_y) ? dx : 0;
                        toplevel->screen_location.size.height += (tl->resizable != ei_axis_x) ? dy : 0;
                        toplevel->content_rect->size.width += (tl->resizable != ei_axis_y) ? dx : 0;
                        toplevel->content_rect->size.height += (tl->resizable != ei_axis_x) ? dy : 0;

                        //Avoid accumulations
                        initial_event_bind->event->param.mouse.where.x += dx;
                        initial_event_bind->event->param.mouse.where.y += dy;

                        placeur_param* placeur = (placeur_param*)toplevel->geom_params;
                        placeur->x = toplevel->screen_location.top_left.x;
                        placeur->y = toplevel->screen_location.top_left.y;

                        run_all_ei_place(toplevel);
                }

                if (root->wclass != NULL) {
                        if (root->wclass->drawfunc != NULL) {
                                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                        }
                }
                ei_rect_t *clipper = &(root->children_head->screen_location);
                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                // Return true to indicate that the event was handled
                return true;
        }else if (event->type == ei_ev_mouse_buttonup) {
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                button_t* button = (button_t*) initial_event_bind->widget->children_head->next_sibling;
                button->relief = ei_relief_raised;
                button->widget.wclass->drawfunc(&button->widget,main_surface,pick_surface,&button->widget.screen_location);
                ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_resizing_toplevel, user_param);
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_resizing_toplevel, user_param);
                return true;
        }

        // Return false if the event was not handled
        return false;
}

bool callback_buttondown_resize_toplevel_start (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
    if (event->type == ei_ev_mouse_buttondown) {
        ei_event_t* event_tbs = malloc(sizeof(ei_event_t));
        event_tbs->type = event->type;
        event_tbs->param = event->param;
        event_tbs->modifier_mask = event->modifier_mask;
        ei_event_bind_widget_t* param = malloc(sizeof(ei_event_bind_widget_t));
        param->event = event_tbs;
        param->widget = widget->parent;

        ei_bind(ei_ev_mouse_move, NULL, "all", callback_move_resizing_toplevel, param);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", callback_move_resizing_toplevel, param);
        return true;
    } else
        return false;
}

//Extra functions

void run_all_ei_place(ei_widget_t widget) {
    if (widget == NULL) {
            return;
    }

    ei_place(widget,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    ei_widget_t child = ei_widget_get_first_child(widget);
    while (child != NULL) {
        run_all_ei_place(child);
        child = ei_widget_get_next_sibling(child);
    }
}

void draw_all_buttons_raised (ei_widget_t widget) {
    ei_widget_t child = ei_widget_get_first_child(widget);
    while (child != NULL) {
        if (strcmp(child->wclass->name, "button") == 0) {
            button_t *button = (button_t *) child;
            button->relief = ei_relief_raised;
            button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
        }
        // Recursively draw children of the current child widget
        draw_all_buttons_raised(child);

        // Move to the next sibling
        child = ei_widget_get_next_sibling(child);
    }
}

bool callback_toplevel_move_front(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                // If the widget is already the last child, no need to move it
                if (widget->next_sibling == NULL) {
                        return true;
                }

                while (strcmp(widget->wclass->name, "toplevel") != 0) {
                        widget = widget->parent;
                }

                // Remove widget from its current position
                ei_widget_t* prev = &(root->children_head);
                while (*prev != NULL && *prev != widget) {
                        prev = &((*prev)->next_sibling);
                }
                if (*prev != root->children_tail) {
                        *prev = ei_widget_get_next_sibling(widget);
                }
                if (*prev != NULL) {
                        // Move widget to the end of the sibling list
                        ei_widget_t* last = &(root->children_head);
                        while (*last != NULL && (*last)->next_sibling != NULL) {
                                last = &((*last)->next_sibling);
                        }
                        if (*last != NULL) {
                                (*last)->next_sibling = widget;
                                widget->next_sibling = NULL;
                                root->children_tail = widget;
                        }
                }

                // Redraw the widget hierarchy
                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                ei_rect_t *clipper = &(root->children_head->screen_location);
                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);

                return true;
        } else {
                return false;
        }
}