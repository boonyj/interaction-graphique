#ifndef PROJETC_IG_EI_BUTTON_H
#define PROJETC_IG_EI_BUTTON_H

#include "ei_implementation.h"
#include "ei_draw_tool.h"
#include "ei_widget_configure.h"

typedef struct button_t {
        ei_impl_widget_t widget;
        int border_width;
        ei_relief_t relief;
        int corner_radius;
        ei_color_t text_color;
        ei_font_t text_font;
        ei_string_t text;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t img_rect;
        ei_anchor_t img_anchor;
        ei_callback_t	callback;
        ei_user_param_t	user_param;
} button_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of the button class: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, the function *must* initialize the memory to 0.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t button_allocfunc ();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void button_releasefunc (ei_widget_t	widget);

/**
 * \brief	A function that draws a widget of a class.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a widget of this class.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void button_setdefaultsfunc(ei_widget_t		widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *		The new location can be retrieved by \ref ei_widget_get_screen_location.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 */
void button_geomnotifyfunc(ei_widget_t		widget);

/**
 * \brief 	A function that is called to create a button class
 */
ei_widgetclass_t* create_button_class();

#endif //PROJETC_IG_EI_BUTTON_H
