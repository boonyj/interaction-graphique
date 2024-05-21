#ifndef PROJETC_IG_EI_DRAW_TOOL_H
#define PROJETC_IG_EI_DRAW_TOOL_H

#include <math.h>
#include "ei_implementation.h"
#include "ei_global.h"

#define PI 3.14159265
extern int nb_segments;

/**
 * @brief   Darkens a color component by multiplying it by a factor of 0.6.
 *
 * @param   colorComponent  The color component value to be darkened.
 *
 * @return  The darkened color component value.
 *
 */
unsigned char dark(unsigned char colorComponent);

/**
 * @brief   Darkens a color component by multiplying it by a factor of 1.4.
 *
 * @param   colorComponent  The color component value to be lightened.
 *
 * @return  The lightened color component value.
 *
 */
unsigned char light(unsigned char colorComponent);

/**
 * @brief   Ensures that the pick color of a widget is properly initialized.
 *
 * @param   widget      The widget whose pick color is to be asserted.
 * @param   surface     The surface associated with the widget.
 *
 */
void assertion_pick_color(ei_widget_t widget, ei_surface_t surface);

/**
 * @brief   Ensures that a color is properly adjusted based on the specified mode.
 *
 * @param   child_color     Pointer to the color structure to be updated.
 * @param   color           The original color.
 * @param   mode            The adjustment mode:
 *                              - 0: No adjustment
 *                              - 1: Lighten the color
 *                              - 2: Darken the color
 *
 */
void assertion_color(ei_color_t* child_color, ei_color_t color, int mode);

/**
 * @brief   Draws text on a surface at a specified location with the given font, color, and anchor.
 *
 * @param   text            The text string to be drawn.
 * @param   text_font       The font to use for rendering the text.
 * @param   text_color      The color of the text.
 * @param   screen_location The location and size of the area where the text will be drawn.
 * @param   surface         The surface on which to draw the text.
 * @param   clipper         The clipping rectangle to limit drawing operations.
 * @param   text_anchor     The anchor point for positioning the text within the screen location.
 *
 */
void draw_text(ei_string_t text, ei_font_t text_font, ei_color_t text_color,  ei_rect_t * where,
               ei_surface_t surface, ei_rect_t* clipper, ei_anchor_t text_anchor);

/**
 * @brief   Draws an image from a source surface onto a destination surface at a specified location,
 *          with the option to specify an anchor point for positioning.
 *
 * @param   surface     The destination surface on which to draw the image.
 * @param   image       The source surface containing the image to be drawn.
 * @param   where       The location and size of the area on the destination surface where the image
 *                      will be drawn.
 * @param   img_rect    The location and size of the image within the source surface.
 * @param   img_anchor  The anchor point for positioning the image within the destination area.
 *
 */
void draw_image_from_surface(ei_surface_t surface, ei_surface_t image,  ei_rect_t * where, ei_rect_ptr_t img_rect, ei_anchor_t img_anchor);

/**
 * @brief   Calculates the clipping rectangle excluding the border width.
 *
 * @param   clipper         The clipping rectangle to be calculated.
 * @param   border_width    The width of the border to be excluded from the clipping rectangle.
 *
 */
void calculate_clipper_without_border(ei_rect_t*	clipper, int border_width);

/**
 * @brief   Calculates the clipping rectangle including the border width.
 *
 * @param   clipper         The clipping rectangle to be calculated.
 * @param   border_width    The width of the border to be included in the clipping rectangle.
 *
 */
void calculate_clipper_with_border(ei_rect_t*	clipper, int border_width);

/**
 * @brief   Calculates the points for drawing the top side of a rectangle without considering corner radius.
 *
 * @param   points      Array of points to store the calculated coordinates.
 * @param   clipper     The clipping rectangle defining the boundary of the top side.
 * @param   size        The size of the rectangle.
 *
 */
void calculate_top_without_corner_radius(ei_point_t *points, ei_rect_t*clipper, ei_size_t size);

/**
 * @brief   Generates the points of an arc.
 *
 * @param   center          The center point of the arc.
 * @param   radius          The radius of the arc.
 * @param   start_angle     The starting angle of the arc in degrees.
 * @param   end_angle       The ending angle of the arc in degrees.
 * @param   segments        The number of segments to approximate the arc.
 * @param   arc_points      A pointer to an array of points to store the calculated coordinates of the arc.
 *
 */
void arc(ei_point_t center, int radius, double start_angle, double end_angle, int segments, ei_point_t** arc_points) ;

/**
 * @brief   Generates the points of a rounded corner.
 *
 * @param   center_corner   The center point of the rounded corner.
 * @param   radius          The radius of the rounded corner.
 * @param   start_angle     The starting angle of the arc defining the rounded corner, in degrees.
 * @param   end_angle       The ending angle of the arc defining the rounded corner, in degrees.
 * @param   points          An array of points where the generated points of the rounded corner will be stored.
 * @param   start_pos       The starting index in the `points` array where the generated points will be stored.
 *
 */
void generate_rounded_corner(ei_point_t center_corner, int radius, float start_angle, float end_angle,
                             ei_point_t *points, int start_pos);

/**
 * @brief   Generates an array of points defining a rounded frame with specified radius and parts.
 *
 * @param   rect        The rectangular area in which the rounded frame will be drawn.
 * @param   radius      The radius of the rounded corners of the frame.
 * @param   part        The part of the frame to generate (0: all corners rounded, 1: left side rounded, 2: right side rounded).
 *
 * @return  An array of points defining the rounded frame.
 *
 */
ei_point_t* rounded_frame (ei_rect_t rect, int radius, int part);

#endif //PROJETC_IG_EI_DRAW_TOOL_H
