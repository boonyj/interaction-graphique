#include "ei_widget_configure.h"
#include "ei_implementation.h"
#include "ei_frame.h"
#include "ei_widget_attributes.h"
#include "ei_button.h"
#include "ei_toplevel.h"
#include "ei_event.h"
#include "ei_callback_toplevel.h"

void			ei_frame_configure		(ei_widget_t		        widget,
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
                frame->widget.screen_location.size = *requested_size;
                frame->widget.content_rect->size = *requested_size;
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

        if (text != NULL) {
                frame->text = malloc((strlen(*text) + 1) * sizeof(char));
                strcpy(frame->text, *text);
                frame->text[strlen(*text)] = '\0';

                if (text_color != NULL) {
                        frame->text_color.alpha = text_color->alpha;
                        frame->text_color.red = text_color->red;
                        frame->text_color.green = text_color->green;
                        frame->text_color.blue = text_color->blue;
                }
                if (text_font != NULL) {
                        frame->text_font = *text_font;
                }
                if (text_anchor != NULL) {
                        frame->text_anchor = *text_anchor;
                }
        }

        if(img != NULL){
                ei_size_t img_size= hw_surface_get_size(*img);
                frame->img = hw_surface_create(main_surface,img_size,true );
                ei_rect_t t= hw_surface_get_rect(*img);
                ei_copy_surface(frame->img, &t,*img,&t, true);
                frame->img_rect = img_rect != NULL? **img_rect : t;
                if (img_anchor != NULL) {
                        frame->img_anchor = *img_anchor;
                }
        }

        if(requested_size == NULL && text!= NULL ){
                int text_width = 0;
                int text_height = 0;
                hw_text_compute_size((ei_const_string_t) text, ei_default_font, &text_width, &text_height);
                ei_size_t size = (ei_size_t){(frame->widget.requested_size.width> text_width*2) ? frame->widget.requested_size.width : text_width*2
                        , (frame->widget.requested_size.height> text_height )? frame->widget.requested_size.height : text_height};
                ei_widget_set_requested_size(&(frame->widget), size);
                frame->widget.screen_location.size = size;
                frame->widget.content_rect->size = size;
        }

}

void			ei_button_configure		(ei_widget_t		        widget,
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
                button->widget.screen_location.size = *requested_size;
                button->widget.content_rect->size = *requested_size;
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
                button->text = malloc((strlen(*text)+1) * sizeof (char));
                strcpy(button->text, *text);
                button->text[strlen(*text)] = '\0';
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

        button->widget.user_data = (user_param != NULL) ? *user_param : button->widget.user_data;

        if (callback != NULL) {
                button->callback = *callback;
                if (button->text != NULL && strcmp(button->text, "Ok") == 0) {
                        ei_bind(ei_ev_mouse_buttonup, widget, NULL, *callback, user_param);
                } else {
                        ei_bind(ei_ev_mouse_buttondown, widget, NULL, *callback, user_param);
                }
        }

        if(img != NULL){
                ei_size_t img_size= hw_surface_get_size(*img);
                button->img = hw_surface_create(main_surface,img_size,true );
                ei_rect_t t= hw_surface_get_rect(*img);
                ei_copy_surface(button->img, &t,*img,&t, true);
                button->img_rect = (img_rect != NULL) ? **img_rect : t;
                if (img_anchor != NULL) {
                        button->img_anchor = *img_anchor;
                }
        }

}

/**
 * \brief	Callback function to confirm the closing of a toplevel widget when the mouse button is released.
 * 		<ul>
 * 			<li> Checks if the event type is ei_ev_mouse_buttonup. </li>
 * 			<li> If true, destroys the parent widget of the given widget. </li>
 * 			<li> Redraws the root widget and its children. </li>
 * 			<li> Unbinds the mouse button up event from this callback. </li>
 * 			<li> Returns true to indicate the event was handled. </li>
 * 		</ul>
 *
 * @param	widget		The widget that received the event.
 * @param	event		The event that occurred.
 * @param	user_param	Additional user parameters.
 * @return			Returns true if the event was handled, false otherwise.
 */
bool callback_toplevel_close_confirmed(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
        if (event->type == ei_ev_mouse_buttonup) {
                ei_widget_destroy(widget->parent);
                root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                ei_rect_t *clipper = &(root->children_head->screen_location);
                ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
                ei_unbind(ei_ev_mouse_buttonup, widget, NULL, callback_toplevel_close_confirmed, NULL);
                return true;
        }
        return false;
}

/**
* \brief	Callback function to initiate the closing of a toplevel widget when the mouse button is pressed down.
* 		<ul>
* 			<li> Checks if the event type is ei_ev_mouse_buttondown. </li>
* 			<li> If true, sets the button's relief to sunken to indicate it is pressed. </li>
* 			<li> Redraws the button widget. </li>
* 			<li> Binds the mouse button up event to the callback_toplevel_close_confirmed function. </li>
* 			<li> Returns true to indicate the event was handled. </li>
* 		</ul>
*
* @param	widget		The widget that received the event.
* @param	event		The event that occurred.
* @param	user_param	Additional user parameters.
* @return			Returns true if the event was handled, false otherwise.
*/
bool callback_toplevel_close(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param){
        if (event->type == ei_ev_mouse_buttondown) {
                button_t *button = (button_t *) widget;
                button->relief = ei_relief_sunken;
                button->widget.wclass->drawfunc(&(button->widget), main_surface, NULL, &(button->widget.screen_location));
                ei_bind(ei_ev_mouse_buttonup, widget, NULL, callback_toplevel_close_confirmed, NULL);
                return true;
        }
        return false;
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
                toplevel->widget.content_rect->size = *requested_size;
                ei_widget_set_requested_size(&(toplevel->widget), *requested_size);
                toplevel->widget.screen_location.size = *requested_size;
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
                toplevel->title = malloc((strlen(*title)+1) * sizeof (char));
                strcpy(toplevel->title, *title);
                toplevel->title[strlen(*title)] = '\0';
                toplevel->title_color.red =  0xff;
                toplevel->title_color.green =  0xff;
                toplevel->title_color.blue =  0xff;
                toplevel->title_color.alpha =  0xff;

        }

        ei_size_t size_button_round = {18 ,18};
        if (closable != NULL) {
                toplevel->closable = *closable;
                size_button_round = *closable ? size_button_round : (ei_size_t){0,0};
        }else {
                size_button_round = toplevel->closable? size_button_round : (ei_size_t){0,0};;
        }

        ei_size_t size_button_square = {12, 12};
        if (resizable != NULL) {
                toplevel->resizable = *resizable;
                size_button_square = *resizable !=0 ? size_button_square : (ei_size_t){0, 0};
        }else {
                size_button_square = toplevel->resizable != 0 ? size_button_square : (ei_size_t){0,0};;
        }

        if (min_size != NULL) {
                toplevel->min_size = **min_size;
        }

        if(toplevel->widget.children_head == NULL){
                ei_widget_t button_round = ei_widget_create	("button", &(toplevel->widget), NULL, NULL);
                ei_button_configure		(button_round, &(size_button_round),
                                                    &(ei_color_t){0xB2, 0x22, 0x22, 0xff},
                                                    &(int){3},
                                                    &(int){9},
                                                    &(ei_relief_t){ei_relief_raised},
                                                    NULL, NULL,
                                                    &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                                    &(ei_callback_t){callback_toplevel_close}, NULL);

                ei_widget_t button_square = ei_widget_create	("button", &(toplevel->widget), NULL, NULL);
                ei_button_configure		(button_square, &(size_button_square),
                                                    &(ei_color_t){0x66, 0x66, 0x66, 0xff},
                                                    &(int){2},
                                                    &(int){0},
                                                    &(ei_relief_t){ei_relief_raised},
                                                    NULL, NULL,
                                                    &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                                    &(ei_callback_t){callback_buttondown_resize_toplevel_start}, NULL);
        }

}