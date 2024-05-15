#include "ei_widget_configure.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_widget_attributes.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_draw_tool.h"
#include "ei_event.h"


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
        frame_t* frame = (frame_t*) widget;
        frame->widget = *widget;

        if (requested_size != NULL){
                ei_widget_set_requested_size(&(frame->widget), *requested_size);
        }

        if (color != NULL) {
                frame->widget.color->alpha = color->alpha;
                frame->widget.color->red = color->red;
                frame->widget.color->green = color->green;
                frame->widget.color->blue = color->blue;
        }

        if (relief != NULL){
                frame->relief = *relief;
        }

        if (border_width != NULL){
                frame->border_width = *border_width;
        }

        if (text != NULL){
                frame->text = *text;
                if (text_color !=  NULL){
                        frame->text_color.alpha = text_color->alpha;
                        frame->text_color.red = text_color->red;
                        frame->text_color.green = text_color->green;
                        frame->text_color.blue = text_color->blue;
                }
                if (text_font !=  NULL){
                        frame->text_font = *text_font;
                }
                if (text_anchor !=  NULL){
                        frame->text_anchor = *text_anchor;
                }
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
        button_t* button = (button_t*) widget;
        button->widget = *widget;

        if (requested_size != NULL){
                ei_widget_set_requested_size(&(button->widget), *requested_size);
        }

        if (color != NULL) {
                button->widget.color->alpha = color->alpha;
                button->widget.color->red = color->red;
                button->widget.color->green = color->green;
                button->widget.color->blue = color->blue;
        }

        if (relief != NULL){
                button->relief = *relief;
        }

        if (border_width != NULL){
                button->border_width = *border_width;
        }

        if (corner_radius != NULL){
                button->corner_radius = *corner_radius;
        }

        if (text != NULL){
                button->text = *text;
                if (text_color !=  NULL){
                        button->text_color.alpha = text_color->alpha;
                        button->text_color.red = text_color->red;
                        button->text_color.green = text_color->green;
                        button->text_color.blue = text_color->blue;
                }
                if (text_font !=  NULL){
                        button->text_font = *text_font;
                }
                if (text_anchor !=  NULL){
                        button->text_anchor = *text_anchor;
                }
        }

        // To be verified with prof
        if (callback != NULL) {
                ei_bind(ei_ev_mouse_buttondown, widget, NULL, *callback, NULL);
        }

        if(img != NULL){
                draw_image_from_surface(ei_app_root_surface(),*img,&(button->widget.screen_location.top_left),button->widget.parent->content_rect);
        }
}

void			ei_toplevel_configure		(ei_widget_t		widget,
                                                                  ei_size_t*		requested_size,
                                                                  const ei_color_t*	color,
                                                                  int*			border_width,
                                                                  ei_string_t*		title,
                                                                  bool*			closable,
                                                                  ei_axis_set_t*		resizable,
                                                                  ei_size_ptr_t*		min_size){
        toplevel_t * toplevel = (toplevel_t*) widget;
        toplevel->widget = *widget;

        if (requested_size != NULL){
                toplevel->widget.content_rect = malloc(sizeof (ei_rect_t));
                toplevel->widget.content_rect->size = *requested_size;
                ei_widget_set_requested_size(&(toplevel->widget), *requested_size);
        }

        if (color != NULL) {
                toplevel->widget.color->alpha = color->alpha;
                toplevel->widget.color->red = color->red;
                toplevel->widget.color->green = color->green;
                toplevel->widget.color->blue = color->blue;
        }

        if (border_width != NULL){
                toplevel->border_width = *border_width;
        }

        if (title != NULL){
                toplevel->title = *title;
                toplevel->title_color.red =  0xff;
                toplevel->title_color.green =  0xff;
                toplevel->title_color.blue =  0xff;
                toplevel->title_color.alpha =  0xff;

        }
}
