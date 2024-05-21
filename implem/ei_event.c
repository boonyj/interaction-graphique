#include "ei_event.h"
#include "ei_global.h"

/**
 * @brief Checks if a new linked event is already present in the linked event list.
 *
 * This function iterates through the linked event list and checks if a linked event with the same
 * event type, widget, tag, callback, and user parameter already exists. If such an event is found,
 * it returns false, indicating that the event is not new. Otherwise, it returns true.
 *
 * @param eventtype The type of event to check.
 * @param widget The widget associated with the event.
 * @param tag The tag associated with the event.
 * @param callback The callback function associated with the event.
 * @param user_param The user parameter associated with the event.
 *
 * @return true if the event is new and not present in the linked event list, false otherwise.
 */
bool static check_if_new_linked_event(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback, void* user_param) {
        ei_linked_event_t* current = linked_event_list;
        while (current != NULL) {
                if ((current->eventtype == eventtype) &&
                    (current->widget == widget) &&
                    (current->tag == tag) &&
                    (current->callback == callback) &&
                    (current->user_param == user_param)) {
                        return false;
                }
                current = current->next;
        }
        return true;
}

void ei_bind(ei_eventtype_t eventtype,
             ei_widget_t widget,
             ei_tag_t tag,
             ei_callback_t callback,
             void* user_param) {

        bool is_new_linked_event = check_if_new_linked_event(eventtype, widget, tag, callback, user_param);

        if (!is_new_linked_event) {
                return;
        }

        ei_linked_event_t* event_to_bind = malloc(sizeof(ei_linked_event_t));
        if (event_to_bind == NULL) {
                // Memory allocation failure
                return;
        }

        event_to_bind->eventtype = eventtype;
        event_to_bind->widget = widget;
        event_to_bind->tag = tag;
        event_to_bind->callback = callback;
        event_to_bind->user_param = user_param;
        event_to_bind->next = NULL;

        if (linked_event_list == NULL) {
                // First linked event to be registered
                linked_event_list = event_to_bind;
        } else {
                // Add new event to the front of the linked list
                event_to_bind->next = linked_event_list;
                linked_event_list = event_to_bind;
        }
}


void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t widget,
               ei_tag_t tag,
               ei_callback_t callback,
               void* user_param) {
        if (linked_event_list == NULL) {
                return;
        }

        ei_linked_event_t* current = linked_event_list;
        ei_linked_event_t* previous = NULL;

        while (current != NULL) {
                if (current->eventtype == eventtype &&
                    current->widget == widget &&
                    current->tag == tag &&
                    current->callback == callback) {

                        // Match found, remove the node from the linked list
                        ei_linked_event_t* node_to_remove = current;
                        if (previous == NULL) {
                                // Removing the head of the list
                                linked_event_list = node_to_remove->next;
                        } else {
                                // Removing a node from the middle or end of the list
                                previous->next = node_to_remove->next;
                        }

                        current = node_to_remove->next; // Move to the next node before freeing
                } else {
                        previous = current;
                        current = current->next;
                }
        }
}