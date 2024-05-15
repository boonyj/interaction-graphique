#include "ei_placer.h"
#include "ei_implementation.h"
#include "ei_widget_configure.h"
#include "ei_toplevel.h"
#include "ei_placeur.h"
#include "ei_event.h"
#include "ei_button.h"
#include "ei_global.h"

//Callback function for close button in toplevel
bool callback_toplevel_close_confirmed(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
        if (event->type == ei_ev_mouse_buttonup) {
                button_t *button = (button_t *) user_param;
                button->relief = ei_relief_raised;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                ei_unbind(ei_ev_mouse_buttonup, user_param, NULL, callback_toplevel_close_confirmed, user_param);
                exit(0);
                return true;
        } else
                return false;
}

bool callback_toplevel_close(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
        if (event->type == ei_ev_mouse_buttondown) {
                button_t *button = (button_t *) widget;
                button->relief = ei_relief_sunken;
                ei_bind(ei_ev_mouse_buttonup, widget, NULL, callback_toplevel_close_confirmed, widget);
                return true;
        } else {
                return false;
        }
}


void		ei_place	(ei_widget_t		widget,
                                     ei_anchor_t*		anchor,
                                     int*			x,
                                     int*			y,
                                     int*			width,
                                     int*			height,
                                     float*			rel_x,
                                     float*			rel_y,
                                     float*			rel_width,
                                     float*			rel_height){
        if (widget == NULL) {
                return;
        }
        if (widget->geom_params == NULL) {
                widget->geom_params = malloc(sizeof(struct placeur_param));
                widget->geom_params->manager = malloc(sizeof(ei_geometrymanager_t));
                ei_geometrymanager_t* type_geom_mng = ei_geometrymanager_from_name("placeur");
                strcpy(widget->geom_params->manager->name, "placeur");
                widget->geom_params->manager->runfunc = type_geom_mng->runfunc;
                widget->geom_params->manager->releasefunc = type_geom_mng->releasefunc;
        }

        placeur_param* placeur = (placeur_param*) widget->geom_params;

        placeur->anchor = (anchor != NULL) ? *anchor : ei_anc_northwest;
        placeur->x = (x != NULL) ? *x : 0;
        placeur->y = (y != NULL) ? *y : 0;
        placeur->width = (width != NULL) ? *width : (widget->requested_size.width != 0) ? widget->requested_size.width: 0;
        placeur->height = (height != NULL) ? *height : (widget->requested_size.height != 0) ? widget->requested_size.height: 0;
        placeur->rel_x = (rel_x != NULL) ? *rel_x : 0.0;
        placeur->rel_y = (rel_y != NULL) ? *rel_y : 0.0;
        placeur->rel_width = (rel_width != NULL) ? *rel_width : 0.0;
        placeur->rel_height = (rel_height != NULL) ? *rel_height : 0.0;

        placeur->geom_mng.manager->runfunc(widget);

        if (strcmp(widget->wclass->name, "toplevel") == 0) {
                toplevel_t* toplevel = (toplevel_t*) widget;
                int width = 0;
                int height = 0;
                hw_text_compute_size(toplevel->title,toplevel->title_font, &width, &height);
                toplevel->widget.screen_location.top_left.x += toplevel->border_width;
                toplevel->widget.screen_location.top_left.y += toplevel->border_width + height;

                ei_widget_t button_round = ei_widget_create	("button", widget, NULL, NULL);

                ei_button_configure		(button_round, &((ei_size_t){18, 18}),
                                                    &(ei_color_t){0xB2, 0x22, 0x22, 0xff},
                                                    &(int){3},
                                                    &(int){9},
                                                    &(ei_relief_t){ei_relief_raised},
                                                    NULL, NULL,
                                                    &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                                    &(ei_callback_t){callback_toplevel_close}, NULL);
                ei_place			(button_round, &(ei_anchor_t){ei_anc_northwest},
                                                 &(int){-(toplevel->border_width)+8}, &(int){-(height+toplevel->border_width)+5}, NULL, NULL,
                                                 &(float){0.0f}, &(float){0.0f},
                                                 NULL, NULL);

                ei_widget_t button_square = ei_widget_create	("button", widget, NULL, NULL);
                ei_button_configure		(button_square, &((ei_size_t){12, 12}),
                                                    &(ei_color_t){0x66, 0x66, 0x66, 0xff},
                                                    &(int){2},
                                                    &(int){0},
                                                    &(ei_relief_t){ei_relief_raised},
                                                    NULL, NULL,
                                                    &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                                    NULL, NULL);
                ei_place			(button_square, &(ei_anchor_t){ei_anc_southeast},
                                                 &(int){0}, &(int){0}, NULL, NULL,
                                                 &(float){1.0f}, &(float){1.0f},
                                                 NULL, NULL);
        }
}



