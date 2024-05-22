/**
 *  @file    ei_toplevel.h
 *  @brief   Declares the functions and structures for the toplevel widget class.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#ifndef PROJETC_IG_TOPLEVEL_H
#define PROJETC_IG_TOPLEVEL_H

#include "ei_implementation.h"
#include "ei_draw_tool.h"
#include "ei_placer.h"

/**
 * @brief Represents a toplevel widget.
 *
 * The toplevel widget is a window-like element with a border, title, and
 * various configurable properties such as title color, font, and resizability.
 */
typedef struct toplevel_t {
        ei_impl_widget_t widget;     ///< Base widget properties.
        int border_width;            ///< Width of the border.
        ei_string_t title;           ///< Title text of the toplevel widget.
        ei_color_t title_color;      ///< Color of the title text.
        ei_font_t title_font;        ///< Font used for the title text.
        bool closable;               ///< Indicates if the toplevel widget can be closed.
        ei_axis_set_t resizable;     ///< Defines the resizability of the widget (none, x axis, y axis, or both).
        ei_size_t min_size;          ///< Minimum size of the toplevel widget.
} toplevel_t;

/**
 * @brief Allocates memory for a toplevel widget.
 *
 * @return A pointer to the allocated widget.
 */
ei_widget_t toplevel_allocfunc ();

/**
 * @brief Releases memory allocated for a toplevel widget.
 *
 * @param widget A pointer to the widget to be released.
 */
void toplevel_releasefunc (ei_widget_t	widget);

/**
 * @brief Draw function for the toplevel widget.
 *
 * @param widget A pointer to the widget to be drawn.
 * @param surface The surface on which to draw the widget.
 * @param pick_surface The picking surface.
 * @param clipper The clipping rectangle.
 */
void toplevel_drawfunc (ei_widget_t		widget,
                      ei_surface_t		surface,
                      ei_surface_t		pick_surface,
                      ei_rect_t*		clipper);

/**
 * @brief Sets the default values for a toplevel widget.
 *
 * @param widget A pointer to the widget to be initialized.
 */
void toplevel_setdefaultsfunc(ei_widget_t		widget);

/**
 * @brief Notification function for geometry changes in the toplevel widget.
 *
 * @param widget A pointer to the widget whose geometry has changed.
 */
void toplevel_geomnotifyfunc(ei_widget_t		widget);

/**
 * @brief Creates the toplevel widget class and returns a pointer to it.
 *
 * @return A pointer to the created toplevel widget class.
 */
ei_widgetclass_t* create_toplevel_class();

#endif //PROJETC_IG_TOPLEVEL_H
