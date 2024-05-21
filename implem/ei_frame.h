#ifndef PROJETC_IG_EI_FRAME_H
#define PROJETC_IG_EI_FRAME_H

#include "ei_implementation.h"
#include "ei_draw.h"
#include "ei_draw_tool.h"
#include "ei_global.h"

/**
 * @brief Represents a frame widget, which can display text or an image.
 */
typedef struct frame_t {
        ei_impl_widget_t widget; ///< The base widget fields.
        int border_width; ///< Width of the border around the frame.
        ei_relief_t relief; ///< Relief type of the frame (raised, sunken, etc.).
        ei_color_t text_color; ///< Color of the text displayed in the frame.
        ei_font_t text_font; ///< Font of the text displayed in the frame.
        char* text; ///< Text string displayed in the frame.
        ei_anchor_t text_anchor; ///< Anchor position for the text.
        ei_surface_t img; ///< Image displayed in the frame.
        ei_rect_t img_rect; ///< Rectangle specifying the part of the image to display.
        ei_anchor_t img_anchor; ///< Anchor position for the image.
} frame_t;

/**
 * @brief Allocates and initializes a new frame widget.
 *
 * This function allocates memory for a new frame widget and initializes it to zero.
 *
 * @return A pointer to the newly allocated frame widget.
 */
ei_widget_t frame_allocfunc ();

/**
 * @brief Releases the resources associated with a frame widget.
 *
 * This function frees the memory used by the text and image associated with the frame widget.
 *
 * @param widget The frame widget to release.
 */
void frame_releasefunc (ei_widget_t	widget);

/**
 * @brief Draws the frame widget, including its relief, text, and image.
 *
 * This function handles drawing the frame widget based on its relief type (none, raised, or sunken),
 * and draws the text and image if they are set.
 *
 * @param frame The frame widget to draw.
 * @param surface The surface on which to draw the widget.
 * @param pick_surface The picking offscreen surface.
 * @param clipper The clipping rectangle to restrict drawing.
 */
void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

/**
 * @brief Sets the default values for a frame widget.
 *
 * This function initializes a frame widget with default values, including default colors, text settings,
 * and sizes.
 *
 * @param widget The frame widget to initialize.
 */
void frame_setdefaultsfunc(ei_widget_t		widget);

/**
 * @brief Notification function for the geometry manager.
 *
 * This function is called when the geometry of the frame widget changes.
 *
 * @param widget The frame widget whose geometry has changed.
 */
void frame_geomnotifyfunc(ei_widget_t		widget);

/**
 * @brief Creates and initializes the frame widget class.
 *
 * This function allocates and initializes a new frame widget class, setting its
 * function pointers for allocation, release, drawing, setting defaults, and geometry notifications.
 *
 * @return A pointer to the newly created frame widget class.
 */
ei_widgetclass_t* create_frame_class();

#endif //PROJETC_IG_EI_FRAME_H
