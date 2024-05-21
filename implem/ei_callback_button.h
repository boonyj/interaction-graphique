#ifndef PROJETC_IG_EI_CALLBACK_BUTTON_H
#define PROJETC_IG_EI_CALLBACK_BUTTON_H

#include "ei_button.h"

/**
 * @brief	A function that is called in response to a user event. This function is called when a
 *              click on a button is released regardless of where the cursor is. The button's relief
 *              will be switched back to its original relief.
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
bool callback_buttonup_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) ;

/**
 * @brief	A function that is called in response to a user event. This function is called when a
 *              button is clicked. The button's relief will be switched to the opposite of its relief
 *              to give an effect of a clicked button.
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
bool callback_buttondown_reverse_relief (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //PROJETC_IG_EI_CALLBACK_BUTTON_H
