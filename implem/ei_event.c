#include "ei_event.h"

typedef struct ei_linked_event_t{
        ei_eventtype_t eventtype;
        ei_widget_t widget;
        ei_callback_t callback;
        ei_tag_t tag;
        void* user_param;
}ei_linked_event_t;

static ei_linked_event_t** linked_event_list = NULL;
static size_t linked_event_list_size = 0;

bool static check_if_new_linked_event(ei_eventtype_t eventtype, ei_widget_t widget, ei_tag_t tag, ei_callback_t callback){
        for(int i =0 ; i < linked_event_list_size; i++){
                if(     (linked_event_list[i]->eventtype == eventtype) &&
                        (linked_event_list[i]->widget == widget) &&
                        (linked_event_list[i]->tag == tag) &&
                        (linked_event_list[i]->callback == callback)){
                        return false;
                }
        }
        return true;
}

void		ei_bind			(ei_eventtype_t		eventtype,
                                                    ei_widget_t		widget,
                                                    ei_tag_t		tag,
                                                    ei_callback_t		callback,
                                                    void*			user_param){

        bool is_new_linked_event = check_if_new_linked_event(eventtype,widget,tag,callback);

        if (linked_event_list == NULL) {
                // Première linked_event à être enregistrée
                linked_event_list = malloc(sizeof(ei_linked_event_t*));
                if (linked_event_list == NULL) {
                        // Échec de l'allocation mémoire
                        return;
                }
                ei_linked_event_t* event_to_bind = malloc(sizeof(ei_linked_event_t));
                event_to_bind->eventtype = eventtype;
                event_to_bind->widget = widget;
                event_to_bind->callback = callback;
                event_to_bind->tag = tag;
                event_to_bind->user_param = user_param;
                linked_event_list[0] = event_to_bind;
                linked_event_list_size = 1;
        } else if(is_new_linked_event){
                        linked_event_list = realloc(linked_event_list,(linked_event_list_size+1)*sizeof(ei_linked_event_t*));
                        linked_event_list_size += 1;
                        ei_linked_event_t* event_to_bind = malloc(sizeof(ei_linked_event_t));
                        event_to_bind->eventtype = eventtype;
                        event_to_bind->widget = widget;
                        event_to_bind->callback = callback;
                        event_to_bind->tag = tag;
                        event_to_bind->user_param = user_param;
                        linked_event_list[linked_event_list_size - 1] = event_to_bind;
        }
}

void		ei_unbind		(ei_eventtype_t		eventtype,
                                              ei_widget_t		widget,
                                              ei_tag_t		tag,
                                              ei_callback_t		callback,
                                              void*			user_param){
        if (linked_event_list == NULL || linked_event_list_size == 0) {
                return;
        }

        // Find the matching event to unbind
        for (int i = 0; i < linked_event_list_size; i++) {
                if (linked_event_list[i]->eventtype == eventtype &&
                    linked_event_list[i]->widget == widget &&
                    linked_event_list[i]->tag == tag &&
                    linked_event_list[i]->callback == callback) {

                        if(user_param != NULL && linked_event_list[i]->user_param != NULL && linked_event_list[i]->user_param != user_param){
                                break;
                        }

                        free(linked_event_list[i]);

                        // Shift remaining elements in the array to remove the gap
                        for (int j = i; j < linked_event_list_size - 1; j++) {
                                linked_event_list[j] = linked_event_list[j + 1];
                        }

                        // Resize the array to remove the last (now duplicate) element
                        linked_event_list = realloc(linked_event_list, (linked_event_list_size - 1) * sizeof(ei_linked_event_t*));

                        // Update the size of the linked_event_list
                        linked_event_list_size--;

                        // Exit the function as we've successfully unbound the event
                        return;
                }
        }
}