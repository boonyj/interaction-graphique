#include <stdlib.h>
#include <string.h>
#include "ei_widgetclass.h"

static ei_widgetclass_t** widgetclass_list = NULL;
static size_t widgetclass_list_size = 0;

size_t ei_widget_struct_size() {
        return sizeof(ei_widget_t);
}

void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
        if (widgetclass_list == NULL) {
                // Première classe à être enregistrée
                widgetclass_list = malloc(sizeof(ei_widgetclass_t*));
                if (widgetclass_list == NULL) {
                        // Échec de l'allocation mémoire
                        return;
                }
                widgetclass_list[0] = widgetclass;
                widgetclass_list_size = 1;
        } else {
                // Vérifie si la classe existe déjà
                if (ei_widgetclass_from_name(widgetclass->name) == NULL) {
                        // Augmente la taille du tableau et ajoute la nouvelle classe
                        ei_widgetclass_t** new_widgetclass_list = realloc(widgetclass_list, (widgetclass_list_size + 1) * sizeof(ei_widgetclass_t*));
                        if (new_widgetclass_list != NULL) {
                                widgetclass_list = new_widgetclass_list;
                                widgetclass_list[widgetclass_list_size] = widgetclass;
                                widgetclass_list_size++;
                        }
                }
        }
}

ei_widgetclass_t* ei_widgetclass_from_name(ei_const_string_t name) {
        if (widgetclass_list != NULL) {
                for (size_t i = 0; i < widgetclass_list_size; ++i) {
                        if (strcmp(widgetclass_list[i]->name, name) == 0) {
                                return widgetclass_list[i];
                        }
                }
        }
        return NULL;
}
