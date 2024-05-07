#include "ei_widget_configure.h"
#include "ei_implementation.h"
#include "ei_frame.h"

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
        frame_t *widget_f = malloc(sizeof (frame_t));
        widget_f->widget = malloc(sizeof (ei_widget_t));
        widget_f->widget = widget;
        if (requested_size != NULL){
                widget_f->widget->requested_size.height = (*requested_size).height;
                widget_f->widget->requested_size.width = (*requested_size).width;

        }

        if ( widget_f->widget->pick_color != NULL) {
                widget_f->widget->pick_color->alpha = color->alpha;
                widget_f->widget->pick_color->red = color->red;
                widget_f->widget->pick_color->green = color->green;
                widget_f->widget->pick_color->blue = color->blue;
        }

        if (relief == NULL){
                widget_f->relief = ei_relief_none;
        } else {
                widget_f->relief =relief;
        }

        if (border_width == NULL){
                widget_f->border_width = 0;
        } else {
                widget_f->border_width =border_width;
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
