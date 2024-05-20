#include "ei_event.h"
#include "ei_global.h"

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

        // Traverse the linked list to find the matching event
        ei_linked_event_t* current = linked_event_list;
        ei_linked_event_t* previous = NULL;

        while (current != NULL) {
                if (current->eventtype == eventtype &&
                    current->widget == widget &&
                    current->tag == tag &&
                    current->callback == callback) {

                        /*if (user_param != NULL && current->user_param != NULL && current->user_param != user_param) {
                                previous = current;
                                current = current->next;
                                continue;
                        }*/

                        // Match found, remove the node from the linked list
                        ei_linked_event_t* node_to_remove = current;
                        if (previous == NULL) {
                                // Removing the head of the list
                                linked_event_list = node_to_remove->next;
                        } else {
                                // Removing a node from the middle or end of the list
                                previous->next = node_to_remove->next;
                        }

                        free(node_to_remove);

                        // Exit the function as we've successfully unbound the event
                        //return;
                }

                previous = current;
                current = current->next;
        }
}
