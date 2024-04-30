/**
 *  @file	ei_placer.h
 *  @brief	Manages the positioning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_PLACER_H
#define EI_PLACER_H

#include "ei_types.h"
#include "ei_widget.h"

void		ei_place	(ei_widget_t		widget,
                                     ei_anchor_t*		anchor,
                                     int*			x,
                                     int*			y,
                                     int*			width,
                                     int*			height,
                                     float*			rel_x,
                                     float*			rel_y,
                                     float*			rel_width,
                                     float*			rel_height){}





static inline void ei_place_xy		(ei_widget_t widget, int x, int y)			{ ei_place(widget, NULL, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL); }
static inline void ei_place_anchored_xy	(ei_widget_t widget, ei_anchor_t anchor, int x, int y)	{ ei_place(widget, &anchor, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL); }

#endif
