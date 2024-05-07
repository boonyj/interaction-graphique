#include "ei_widget_configure.h"
#include "ei_implementation.h"

void			ei_frame_configure		(ei_widget_t		widget,
                                                               ei_size_t*		requested_size,
                                                               const ei_color_t*	color,
                                                               int*			border_width,
                                                               ei_relief_t*		relief,
                                                               ei_string_t*		text,
                                                               ei_font_t*		text_font,
                                                               ei_color_t*		text_color,
                                                               ei_anchor_t*		text_anchor,
                                                               ei_surface_t*		img,
                                                               ei_rect_ptr_t*		img_rect,
                                                               ei_anchor_t*		img_anchor){
        if (requested_size != NULL){
                widget->requested_size.height = (*requested_size).height;
                widget->requested_size.width = (*requested_size).width;

        }

        if (widget->pick_color != NULL) {
                widget->pick_color->alpha = color->alpha;
                widget->pick_color->red = color->red;
                widget->pick_color->green = color->green;
                widget->pick_color->blue = color->blue;
        }

        if (relief == NULL){
                widget->geom_params->relief = ei_relief_none;
        } else {
                widget->geom_params->relief = relief;
        }

        if (border_width == NULL){
                widget->geom_params->border_width = 0;
        } else {
                widget->geom_params->border_width =border_width;
        }

}

void			ei_button_configure		(ei_widget_t		widget,
                                                                ei_size_t*		requested_size,
                                                                const ei_color_t*	color,
                                                                int*			border_width,
                                                                int*			corner_radius,
                                                                ei_relief_t*		relief,
                                                                ei_string_t*		text,
                                                                ei_font_t*		text_font,
                                                                ei_color_t*		text_color,
                                                                ei_anchor_t*		text_anchor,
                                                                ei_surface_t*		img,
                                                                ei_rect_ptr_t*		img_rect,
                                                                ei_anchor_t*		img_anchor,
                                                                ei_callback_t*		callback,
                                                                ei_user_param_t*	user_param){

}

void			ei_toplevel_configure		(ei_widget_t		widget,
                                                                  ei_size_t*		requested_size,
                                                                  const ei_color_t*	color,
                                                                  int*			border_width,
                                                                  ei_string_t*		title,
                                                                  bool*			closable,
                                                                  ei_axis_set_t*		resizable,
                                                                  ei_size_ptr_t*		min_size){

}
