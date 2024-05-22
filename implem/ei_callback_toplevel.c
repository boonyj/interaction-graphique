#include "ei_callback_toplevel.h"

// Callbacks for toplevel resizing

// Function to move a widget to become the first sibling among its siblings
void widget_move_to_front(ei_widget_t parent, ei_widget_t widget) {
        if (parent == NULL || widget == NULL || parent->children_head == NULL) return;

        if (parent->children_head == widget) return;

        ei_widget_t prev = NULL;
        ei_widget_t current = parent->children_head;
        while (current != NULL && current != widget) {
                prev = current;
                current = current->next_sibling;
        }

        if (current == widget) {
                if (prev != NULL) {
                        prev->next_sibling = widget->next_sibling;
                }

                if (parent->children_tail == widget) {
                        parent->children_tail = prev;
                }

                widget->next_sibling = parent->children_head;
                parent->children_head = widget;
        }
}

// Function to draw a resized toplevel (Half screen to the left, Half screen to the right, Fullscreen)
void draw_resized_toplevel(ei_widget_t widget, int width, int x, ei_callback_t callback) {
        toplevel_t* toplevel = (toplevel_t*) widget;
        if (strcmp(root->children_head->wclass->name, "frame") == 0) {
                int text_width = 0;
                int text_height = 0;
                hw_text_compute_size(toplevel->title,toplevel->title_font,&text_width,&text_height);

                int height = root_size->height-text_height;
                ei_place(&(toplevel->widget), NULL, &(int){x}, &(int){0}, &(int){width}, &(int){height}, NULL, NULL, NULL, NULL);
                run_all_ei_place(widget);
                ei_widget_destroy(root->children_head);
        }
        root->screen_location.top_left.x = 0;
        root->screen_location.top_left.y = 0;

        ei_unbind(ei_ev_mouse_buttonup, &(toplevel->widget), NULL, callback, NULL);
        ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_toplevel, NULL);
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_toplevel, NULL);
}

bool callback_left_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                draw_resized_toplevel(widget, root_size->width/2, -root->screen_location.top_left.x, callback_left_resize_toplevel);
                return true;
        }
        return false;
}

bool callback_right_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                draw_resized_toplevel(widget, root_size->width/2, -root->screen_location.top_left.x+(root_size->width/2), callback_right_resize_toplevel);
                return true;
        }
        return false;
}

bool callback_fullscreen_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttonup) {
                draw_resized_toplevel(widget, root_size->width, -root->screen_location.top_left.x, callback_fullscreen_toplevel);
                return true;
        }
        return false;
}

bool callback_move_resizing_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_move) {
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                ei_mouse_event_t initial_mouse = initial_event_bind->event->param.mouse;

                toplevel_t* tl = (toplevel_t*) initial_event_bind->widget;
                ei_widget_t toplevel = &tl->widget;

                bool is_resizable = (tl->resizable == ei_axis_x || tl->resizable == ei_axis_y || tl->resizable == ei_axis_both);

                if(is_resizable) {
                        ei_point_t mouse_position = event->param.mouse.where;

                        int dx = mouse_position.x - initial_mouse.where.x;
                        int dy = mouse_position.y - initial_mouse.where.y;

                        toplevel->screen_location.size.width += (tl->resizable != ei_axis_y) ? dx : 0;
                        toplevel->screen_location.size.height += (tl->resizable != ei_axis_x) ? dy : 0;
                        toplevel->content_rect->size.width += (tl->resizable != ei_axis_y) ? dx : 0;
                        toplevel->content_rect->size.height += (tl->resizable != ei_axis_x) ? dy : 0;

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
                return true;
        } else if (event->type == ei_ev_mouse_buttonup) {
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                button_t* button = (button_t*) initial_event_bind->widget->children_head->next_sibling;
                button->relief = ei_relief_raised;
                button->widget.wclass->drawfunc(&button->widget,main_surface,pick_surface,&button->widget.screen_location);

                ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_resizing_toplevel, user_param);
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_resizing_toplevel, user_param);
                return true;
        }
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

// Callbacks for toplevel movement

// Function to draw a temporary frame on the background to indicate that the toplevel will be resized if the left-click of the mouse is released
void temp_frame(toplevel_t* toplevel, int width, int x, int y, ei_callback_t callback) {
        if (strcmp(root->children_head->wclass->name, "frame") != 0) {
                ei_widget_t temp_frame = ei_widget_create("frame", root, NULL, NULL);
                ei_frame_configure(temp_frame, &(ei_size_t) {width, root_size->height},
                                   &(ei_color_t) {0x88, 0x88, 0x88, 0x60},
                                   0,
                                   &(ei_relief_t) {ei_relief_none}, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL);
                ei_place_xy(temp_frame, x, y);
                widget_move_to_front(temp_frame->parent, temp_frame);

                ei_bind(ei_ev_mouse_buttonup, &(toplevel->widget), NULL, callback, NULL);
        }
}

bool callback_move_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_move) {
                ei_event_bind_widget_t* initial_event_bind = (ei_event_bind_widget_t*) user_param;
                ei_mouse_event_t initial_mouse = initial_event_bind->event->param.mouse;
                ei_mouse_event_t mouse = event->param.mouse;

                ei_widget_t toplevel = (ei_widget_t) initial_event_bind->widget;

                if (widget == toplevel) {
                        ei_point_t mouse_position = event->param.mouse.where;

                        int dx = mouse_position.x - initial_mouse.where.x;
                        int dy = mouse_position.y - initial_mouse.where.y;
                        ei_place_xy(widget, dx, dy);
                        run_all_ei_place(widget);

                        toplevel_t* toplevel_w = (toplevel_t*) widget;
                        // Check if the toplevel is resizable on both axis to enable half screen and fullscreen
                        if (toplevel_w->resizable == ei_axis_both) {
                                int text_width = 0;
                                int text_height = 0;
                                hw_text_compute_size(toplevel_w->title, toplevel_w->title_font, &text_width,
                                                     &text_height);

                                if (toplevel_w->widget.screen_location.size.height == root_size->height - text_height) {
                                        toplevel_w->widget.screen_location.size.width = 320;
                                        toplevel_w->widget.screen_location.size.height = 240;
                                }

                                if (toplevel_w->widget.screen_location.top_left.x == toplevel_w->border_width) {
                                        temp_frame(toplevel_w, root_size->width / 2, -root->screen_location.top_left.x,
                                                   -root->screen_location.top_left.y,callback_left_resize_toplevel);
                                } else if(toplevel_w->widget.screen_location.top_left.x + toplevel_w->widget.screen_location.size.width +
                                        toplevel_w->border_width == root_size->width) {
                                        temp_frame(toplevel_w, root_size->width / 2, -root->screen_location.top_left.x+(root_size->width/2),
                                                   -root->screen_location.top_left.y,callback_right_resize_toplevel);
                                } else if(toplevel_w->widget.screen_location.top_left.y - text_height == 0) {
                                        temp_frame(toplevel_w, root_size->width, -root->screen_location.top_left.x,
                                                   -root->screen_location.top_left.y,callback_fullscreen_toplevel);
                                } else if (strcmp(root->children_head->wclass->name, "frame") == 0) {
                                        ei_widget_destroy(root->children_head);
                                        ei_unbind(ei_ev_mouse_buttonup, &(toplevel_w->widget), NULL, callback_left_resize_toplevel, NULL);
                                }
                        }

                       if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                       }
                       ei_rect_t *clipper = &(root->children_head->screen_location);
                       ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                       return true;
                }
        } else if(event->type == ei_ev_mouse_buttonup) {
                root->screen_location.top_left.x = 0;
                root->screen_location.top_left.y = 0;

                ei_unbind(ei_ev_mouse_move, NULL, "all", callback_move_toplevel, user_param);
                ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_move_toplevel, NULL);
                return true;
        }
        return false;
}

bool callback_buttondown_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                toplevel_t* toplevel = (toplevel_t *) widget;
                int width = 0;
                int height = 0;
                hw_text_compute_size(toplevel->title,toplevel->title_font, &width, &height);
                if (event->param.mouse.where.x > widget->screen_location.top_left.x
                        && event->param.mouse.where.x < widget->screen_location.top_left.x +widget->screen_location.size.width
                        && event->param.mouse.where.y > widget->screen_location.top_left.y - height
                        && event->param.mouse.where.y < widget->screen_location.top_left.y){
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

// Callback for toplevel placement

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
                draw_all_buttons_raised(child);

                child = ei_widget_get_next_sibling(child);
        }
}

bool callback_toplevel_move_front(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                while (widget->parent != root) {
                        if(widget->parent != NULL){
                                widget = ei_widget_get_parent(widget);
                        } else {
                                return true;
                        }
                }

                ei_widget_t* prev = &(root->children_head);
                while (*prev != NULL && *prev != widget) {
                        prev = &((*prev)->next_sibling);
                }
                if (*prev != root->children_tail) {
                        *prev = ei_widget_get_next_sibling(widget);
                }
                if (*prev != NULL) {
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

                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                ei_rect_t *clipper = &(root->children_head->screen_location);
                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                return true;
        } else {
                return false;
        }
}