/**
 *  @file    ei_global.h
 *  @brief   Declares global variables.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#include "hw_interface.h"
#include "ei_implementation.h"

/**
 * @brief The main surface for drawing the widgets.
 */
extern ei_surface_t main_surface;

/**
 * @brief The surface used for picking, which maps mouse clicks to widgets.
 */
extern ei_surface_t pick_surface;

/**
 * @brief The root widget of the widget tree.
 */
extern ei_widget_t root;

/**
 * @brief The size of the root widget.
 */
extern ei_size_t* root_size;

/**
 * @brief A linked list of rectangles that need to be redrawn.
 */
extern ei_linked_rect_t* invalidated_rects_head;

/**
 * @brief A linked list of events that are bound to widgets.
 */
extern ei_linked_event_t* linked_event_list;