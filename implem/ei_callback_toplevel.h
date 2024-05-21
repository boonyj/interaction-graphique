#ifndef PROJETC_IG_EI_CALLBACK_TOPLEVEL_H
#define PROJETC_IG_EI_CALLBACK_TOPLEVEL_H

#include "ei_implementation.h"
#include "ei_event.h"
#include "ei_widget_configure.h"
#include "ei_widget_attributes.h"
#include "ei_placer.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_global.h"
#include "ei_toplevel.h"
#include "ei_frame.h"

/**
 * @brief	A function that is called in response to a user event. This function is called when a
 *              toplevel is moved to the left side of the screen and the left-click of the mouse is
 *              released. It resizes the toplevel to make it fit from the left side of the screen to
 *              the middle.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_left_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when a
 *              toplevel is moved to the right side of the screen and the left-click of the mouse is
 *              released. It resizes the toplevel to make it fit from the middle of the screen to
 *              the right.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_right_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when a
 *              toplevel is moved to the top of the screen and the left-click of the mouse is
 *              released. It resizes the toplevel to make it fullscreen.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_fullscreen_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when the
 *              left-click of a mouse is clicked, held and the cursor is moved on the button at the bottom
 *              left of a toplevel. It resizes the toplevel according to the position of the cursor on screen.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_move_resizing_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when the
 *              left-click of a mouse is clicked on the bottom-right button of a toplevel.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_buttondown_resize_toplevel_start(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when the
 *              left-click of a mouse is clicked, held and the cursor is moved on a toplevel. It moves
 *              the toplevel according to the position of the cursor on screen.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_move_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that is called in response to a user event. This function is called when the
 *              left-click of a mouse is clicked on a toplevel. It calculates the position of the cursor
 *              to prepare to move the toplevel.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_buttondown_top_level(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * @brief	A function that runs the ei_place of a widget, all its children and its children's siblings.
 */
void run_all_ei_place(ei_widget_t widget);

/**
 * @brief	Function that iterates through every child and the child's siblings of a widget to find every
 *              button in the widget's family. It then draws every button with a raised relief.
 */
void draw_all_buttons_raised(ei_widget_t widget);

/**
 * @brief	A function that is called in response to a user event. This function is called when the
 *              left-click of a mouse is clicked on a toplevel. It calculates the position of the cursor
 *              to prepare to move the toplevel.
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
bool callback_toplevel_move_front(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //PROJETC_IG_EI_CALLBACK_TOPLEVEL_H