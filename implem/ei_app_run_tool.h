/**
 *  @file    ei_app_run_tool.h
 *  @brief   Provides utility functions for managing and handling events in the application,
 *           including button press events, drawing widgets, and handling keydown events.
 *
 *  \author
 *  Created by BOON, MOHD NOOR and NAIR on 21.05.24.
 *
 */

#ifndef PROJETC_IG_EI_APP_RUN_TOOL_H
#define PROJETC_IG_EI_APP_RUN_TOOL_H

#include "ei_implementation.h"
#include "ei_global.h"
#include "ei_callback_button.h"
#include <setjmp.h>
#include <signal.h>

/**
 * @brief   Handles button press events specifically for toplevel exit buttons.
 *
 * @param   widget              The widget that received the event.
 * @param   head                The head of the linked list of events.
 * @param   event               The event containing all its parameters (type, etc.).
 * @param   exit_button_handled A boolean flag to indicate if the exit button event was handled.
 */
void exit_toplevel_button_press(ei_widget_t widget, ei_linked_event_t *head, ei_event_t *event, bool *exit_button_handled);

/**
 * @brief   Draws all widgets in the application.
 *
 * @param   clipper             The clipping rectangle to restrict drawing.
 */
void draw_all_widgets(ei_rect_t *clipper);

/**
 * @brief   Handles generic events for widgets.
 *
 * @param   clipper             The clipping rectangle to restrict drawing.
 * @param   event               The event containing all its parameters (type, etc.).
 * @param   widget              The widget that received the event.
 * @param   head                The head of the linked list of events.
 */
void get_event(ei_rect_t *clipper, ei_event_t *event, struct ei_impl_widget_t *widget, ei_linked_event_t *head);

/**
 * @brief   Handles keydown events specifically for widgets.
 *
 * @param   widget              The widget that received the event.
 * @param   head                The head of the linked list of events.
 * @param   clipper             The clipping rectangle to restrict drawing.
 * @param   event               The event containing all its parameters (type, etc.).
 */
void get_keydown_event(ei_impl_widget_t *widget, ei_linked_event_t *head, ei_rect_t **clipper, ei_event_t *event);

/**
 * @brief   Handles application-specific events.
 *
 * @param   widget              The widget that received the event.
 * @param   head                The head of the linked list of events.
 * @param   clipper             The clipping rectangle to restrict drawing.
 * @param   event               The event containing all its parameters (type, etc.).
 */
void run_ev_app_event(ei_impl_widget_t *widget, ei_linked_event_t *head, ei_rect_t **clipper, ei_event_t *event);

/**
 * @brief   Frees all bound events in the event list.
 */
void free_event_list();

/**
 * @brief   Clears the list of invalidated rectangles.
 */
void clear_invalidated_rects();

/**
 * @brief   Signal handler function for SIGSEGV (segmentation fault).
 *
 * @param   signum  The signal number.
 */
void sigsegv_handler(int signum);

/**
 * @brief   Checks if a given pointer is a valid memory address.
 *
 * @param   ptr     The pointer to be checked.
 *
 * @return          1 if the address is valid, 0 otherwise.
 */
int is_valid_address(void *ptr);

#endif //PROJETC_IG_EI_APP_RUN_TOOL_H
