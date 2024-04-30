/**
 * @file	ei_widget_configure.h
 *
 * @brief 	API for widgets configuration.
 *
 *  Created by François Bérard on 10.04.2023
 *  Copyright 2023 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGET_CONFIGURE_H
#define EI_WIDGET_CONFIGURE_H

#include "ei_widget.h"

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

}



static const int	k_default_button_border_width	= 4;	///< The default border width of button widgets.
static const int	k_default_button_corner_radius	= 10;	///< The default corner radius of button widgets.

static inline void ei_frame_set_bg_color	(ei_widget_t frame,  ei_color_t bg_color)	{ ei_frame_configure(frame, NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_frame_set_text		(ei_widget_t frame,  ei_string_t text)		{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_frame_set_image		(ei_widget_t frame,  ei_surface_t image)	{ ei_frame_configure(frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL); }

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

static inline void ei_button_set_bg_color	(ei_widget_t button, ei_color_t bg_color)	{ ei_button_configure(button, NULL, &bg_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_button_set_text		(ei_widget_t button, ei_string_t text)		{ ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, &text, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_button_set_image		(ei_widget_t button, ei_surface_t image)	{ ei_button_configure(button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &image, NULL, NULL, NULL, NULL); }

void			ei_toplevel_configure		(ei_widget_t		widget,
                                                                  ei_size_t*		requested_size,
                                                                  const ei_color_t*	color,
                                                                  int*			border_width,
                                                                  ei_string_t*		title,
                                                                  bool*			closable,
                                                                  ei_axis_set_t*		resizable,
                                                                  ei_size_ptr_t*		min_size){

}




#endif //EI_WIDGET_CONFIGURE_H
