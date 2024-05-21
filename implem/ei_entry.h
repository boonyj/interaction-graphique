/**
*  @file	ei_entry.h
 *  @brief	Contains all functions linked to events and text manipulations for an entry widget.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#ifndef PROJETC_IG_EI_ENTRY_H
#define PROJETC_IG_EI_ENTRY_H

#include "ei_entry_class.h"
#include "ei_placer.h"
#include "ei_toplevel.h"

/**
 * \brief	Returns a string without the cursor '|'.
 *		<ul>
 *			<li> Accepts a string containing the character '|' </li>
 *			<li> creates a new char pointer which is a copy of the initial string without '|' </li>
 *			<li> returns the new string </li>
 *		</ul>
 *
 * @param	str	Pointer to the input string.
 * @return	A new string without the '|' character, or NULL if the input string is NULL.
 */
char* get_text_without_cursor(char* str);

/**
 * \brief	Returns a new string with a character concatenated at the end of it.
 *		<ul>
 *			<li> Accepts a string and a character </li>
 *			<li> Creates a new char pointer, puts the character at the end of the string </li>
 *			<li> returns the new string </li>
 *		</ul>
 *
 * @param	str	Pointer to the input string.
 * @param ch	Character to be concatenated to the string.
 * @return	A new string with the character concatenated, or NULL if the input string is NULL.
 */
char* get_text_with_char_concatenated(char* str, char ch);

/**
 * \brief	Inserts a character before the '|' character in a string.
 *		<ul>
 *			<li> Accepts a string, a character, and the maximum size of the string </li>
 *			<li> Creates a new char pointer, inserts the character before '|' </li>
 *			<li> returns the new string </li>
 *		</ul>
 *
 * @param	str	Pointer to the input string.
 * @param ch	Character to be inserted before the '|'.
 * @param max	Maximum size of the string.
 * @return	A new string with the character inserted, or NULL if the input string is NULL.
 */
char* insert_char_before_pipe(char* str, char ch, int max);

/**
 * \brief	Removes a character either before or after the '|' character in a string.
 *		<ul>
 *			<li> Accepts a string and a boolean flag </li>
 *			<li> If the flag is true, removes the character before '|' </li>
 *			<li> If the flag is false, removes the character after '|' </li>
 *			<li> returns the new string </li>
 *		</ul>
 *
 * @param	str	Pointer to the input string.
 * @param delete_before	Boolean flag to determine whether to delete the character before (true) or after (false) the '|'.
 * @return	A new string with the character removed, or NULL if the input string is NULL.
 */
char* remove_character_before_pipe(char* str, bool delete_before);

/**
 * \brief	Moves the '|' character to the left or right in the string.
 *		<ul>
 *			<li> Accepts a string and a boolean direction flag </li>
 *			<li> If the flag is true, moves '|' to the right </li>
 *			<li> If the flag is false, moves '|' to the left </li>
 *			<li> returns the new string </li>
 *		</ul>
 *
 * @param	str	Pointer to the input string.
 * @param dir	Boolean flag to determine the direction to move the '|' (true for right, false for left).
 * @return	A new string with '|' moved, or NULL if the input string is NULL.
 */
char* move_pipe_in_text(char* str, bool dir);

/**
 * \brief	Handles keyboard input events for the entry widget in focus.
 *		<ul>
 *			<li> Accepts a widget, an event, and a user parameter </li>
 *			<li> Handles character input, backspace, delete, arrow keys, and tab key events </li>
 *			<li> Updates the text in the entry widget and redraws it </li>
 *		</ul>
 *
 * @param widget	Pointer to the widget.
 * @param event	Pointer to the event.
 * @param user_param	Pointer to the user parameter.
 * @return	true if the event was handled, false otherwise.
 */
bool callback_type_in_focus (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * \brief	Handles mouse button down events to remove focus from the entry widget.
 *		<ul>
 *			<li> Accepts a widget, an event, and a user parameter </li>
 *			<li> Removes focus from the entry widget </li>
 *			<li> Updates the text in the entry widget and redraws it </li>
 *		</ul>
 *
 * @param widget	Pointer to the widget.
 * @param event	Pointer to the event.
 * @param user_param	Pointer to the user parameter.
 * @return	true if the event was handled, false otherwise.
 */
bool callback_buttondown_remove_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

/**
 * \brief	Handles mouse button down events to give focus to the entry widget.
 *		<ul>
 *			<li> Accepts a widget, an event, and a user parameter </li>
 *			<li> Gives focus to the entry widget </li>
 *			<li> Updates the text in the entry widget and redraws it </li>
 *		</ul>
 *
 * @param widget	Pointer to the widget.
 * @param event	Pointer to the event.
 * @param user_param	Pointer to the user parameter.
 * @return	true if the event was handled, false otherwise.
 */
bool callback_buttondown_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //PROJETC_IG_EI_ENTRY_CLASS_H