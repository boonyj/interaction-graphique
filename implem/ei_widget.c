/**
 * @file	ei_widget.h
 *
 * @brief 	API for widgets management: creation, destruction
 *
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 */

#ifndef EI_WIDGET_H
#define EI_WIDGET_H

#include "ei_widgetclass.h"
#include "ei_draw.h"


struct ei_event_t;

typedef void		(*ei_widget_destructor_t)	(ei_widget_t widget);

typedef bool		(*ei_callback_t)		(ei_widget_t		widget,
                                                             struct ei_event_t*	event,
                                                             ei_user_param_t	user_param);


ei_widget_t		ei_widget_create		(ei_const_string_t	class_name,
                                                            ei_widget_t		parent,
                                                            ei_user_param_t	user_data,
                                                            ei_widget_destructor_t destructor){

}


void			ei_widget_destroy		(ei_widget_t		widget){}


bool	 		ei_widget_is_displayed		(ei_widget_t		widget){}


ei_widget_t		ei_widget_pick			(ei_point_t*		where){}






#endif
