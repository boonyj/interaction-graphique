#include <string.h>
#include "ei_widgetclass.h"

static ei_widgetclass_t* widgetclass_list = NULL; // A list containing widget classes

size_t ei_widget_struct_size() {
        return sizeof(ei_widget_t);
}

void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
        if (widgetclass_list == NULL) {
                // First class to be registered
                widgetclass_list = widgetclass;
                widgetclass->next = NULL;
        } else {
                // Check if the class already exists
                if (ei_widgetclass_from_name(widgetclass->name) == NULL) {
                        widgetclass->next = widgetclass_list;
                        widgetclass_list = widgetclass;
                }
        }
}

ei_widgetclass_t* ei_widgetclass_from_name(ei_const_string_t name) {
        ei_widgetclass_t* current = widgetclass_list;
        while (current != NULL) {
                if (strcmp(current->name, name) == 0) {
                        return current;
                }
                current = current->next;
        }
        return NULL;
}