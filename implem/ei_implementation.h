/**
 * @file	ei_implementation.h
 *
 * @brief 	Private definitions.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"
#include "ei_widget_attributes.h"

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t {
	ei_widgetclass_t*	wclass;		///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
	uint32_t		pick_id;	///< Id of this widget in the picking offscreen.
	ei_color_t*		pick_color;	///< pick_id encoded as a color.
        ei_color_t*		color;	        ///< color.
        void*			user_data;	///< Pointer provided by the programmer for private use. May be NULL.
	ei_widget_destructor_t	destructor;	///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

	/* Widget Hierachy Management */
	ei_widget_t		parent;		///< Pointer to the parent of this widget.
	ei_widget_t		children_head;	///< Pointer to the first child of this widget.	Children are chained with the "next_sibling" field.
	ei_widget_t		children_tail;	///< Pointer to the last child of this widget.
	ei_widget_t		next_sibling;	///< Pointer to the next child of this widget's parent widget.

	/* Geometry Management */
	ei_geom_param_t		geom_params;	///< Pointer to the geometry management parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
	ei_size_t		requested_size;	///< See \ref ei_widget_get_requested_size.
	ei_rect_t		screen_location;///< See \ref ei_widget_get_screen_location.
	ei_rect_t*		content_rect;	///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
} ei_impl_widget_t;


/**
 * \brief	Fields common to all geometry managers. Every geometry manager specializes this by adding its own fields.
 */
typedef struct ei_impl_geom_param_t {
	ei_geometrymanager_t*		manager;	///< The geometry managers that manages this widget.
} ei_impl_geom_param_t;

/**
 * @brief	Parameters for the placer geometry manager.
 */
typedef struct placeur_param {
        ei_impl_geom_param_t geom_mng;
        ei_anchor_t anchor;
        int x;
        int y;
        int width;
        int height;
        float rel_x;
        float rel_y;
        float rel_width;
        float rel_height;
} placeur_param;

/**
 * @brief	Structure for binding events to widgets.
 */
typedef struct ei_event_bind_widget_t{
        ei_event_t* event;
        ei_impl_widget_t* widget;
}ei_event_bind_widget_t;


/**
 * @brief	Linked list structure for managing events.
 */
typedef struct ei_linked_event_t{
        ei_eventtype_t eventtype;
        ei_widget_t widget;
        ei_callback_t callback;
        ei_tag_t tag;
        void* user_param;
        struct ei_linked_event_t* next;
}ei_linked_event_t;

/**
 * @brief	Draws the children of a widget.
 * 		The children are draw withing the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget which children are drawn.
 * @param	surface		A locked surface where to draw the widget's children.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void		ei_impl_widget_draw_children	(ei_widget_t		widget,
						 ei_surface_t		surface,
						 ei_surface_t		pick_surface,
						 ei_rect_t*		clipper);

/**
 * @brief	Frees the widget and its siblings.
 *
 * @param	widget		The widget to free.
 * @param	is_root		Indicates if the widget is the root.
 */
void free_widget_and_siblings(ei_widget_t* widget, bool is_root);

/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */
uint32_t	ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);

#endif