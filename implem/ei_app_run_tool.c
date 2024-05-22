#include "ei_app_run_tool.h"

void exit_toplevel_button_press(ei_widget_t widget, ei_linked_event_t *head, ei_event_t *event, bool *exit_button_handled) {
        if (widget->parent != NULL && strcmp(widget->parent->wclass->name, "toplevel") == 0) {
                if (widget->pick_id == widget->parent->pick_id + 1) {
                        if ((*event).type == ei_ev_mouse_buttonup) {
                                while (head != NULL) {
                                        if (head->eventtype == ei_ev_mouse_buttonup) {
                                                if (head->widget == widget) {
                                                        head->callback(widget, event, widget->user_data);
                                                        (*exit_button_handled) = true;
                                                        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget);
                                                        //ei_unbind(ei_ev_mouse_buttonup, NULL, "all", callback_buttonup_reverse_relief, widget->next_sibling);
                                                        break;
                                                }
                                        }
                                        head = head->next;
                                }
                        } else if ((*event).type == ei_ev_mouse_buttondown) {
                                while (head != NULL) {
                                        if (head->eventtype == ei_ev_mouse_buttondown) {
                                                if (head->widget == widget) {
                                                        head->callback(widget, event, widget->user_data);
                                                        break;
                                                }
                                        }
                                        head = head->next;
                                }
                        }
                }
        }
}

void draw_all_widgets(ei_rect_t *clipper) {
        if (root->wclass != NULL) {
                if (root->wclass->drawfunc != NULL) {
                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                }
        }
        clipper = &(root->children_head->screen_location);
        ei_impl_widget_draw_children(root, main_surface, pick_surface, clipper);
}

void get_event(ei_rect_t *clipper, ei_event_t *event, struct ei_impl_widget_t *widget, ei_linked_event_t *head) {
        head = linked_event_list;
        //Search for event in list
        while (head != NULL) {
                if (head->eventtype == (*event).type) {
                        if(head->widget != NULL){
                                if(widget == head->widget){
                                        head->callback(widget, event, widget->user_data);
                                        draw_all_widgets(clipper);
                                }
                        } else {
                                if(is_valid_address(widget) &&(strcmp(head->tag, "all") == 0||
                                        strcmp(head->tag, widget->wclass->name) == 0) ){
                                        head->callback(widget, event, head->user_param);
                                }
                        }
                }
                head = head->next;
        }
}

void get_keydown_event(struct ei_impl_widget_t *widget, ei_linked_event_t *head, ei_rect_t **clipper, ei_event_t *event) {
        while (head != NULL) {
                if (head->eventtype == (*event).type) {
                        head->callback(widget, event, head->user_param);
                        if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        (*clipper) = &(root->children_head->screen_location);
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, (*clipper));
                }
                head = head->next;
        }
}

void run_ev_app_event(struct ei_impl_widget_t *widget,ei_linked_event_t *head, ei_rect_t **clipper, ei_event_t *event) {
        while (head != NULL) {
                if (head->eventtype == ei_ev_app) {
                        head->callback(NULL, event, head->user_param);
                        if (root->wclass != NULL) {
                                if (root->wclass->drawfunc != NULL) {
                                        root->wclass->drawfunc(root, main_surface, pick_surface, NULL);
                                }
                        }
                        (*clipper) = &(root->children_head->screen_location);
                        ei_impl_widget_draw_children(root, main_surface, pick_surface, (*clipper));
                }
                head = head->next;
        }
}

// Function free all binded events in the events list
void free_event_list() {
        ei_linked_event_t* current = linked_event_list;
        ei_linked_event_t* next_node;

        while (current != NULL) {
                next_node = current->next;
                free(current);
                current = next_node;
        }
        linked_event_list = NULL;
}

// Function to clear the invalidated rectangles list (for cleanup purposes)
void clear_invalidated_rects() {
        ei_linked_rect_t* current = invalidated_rects_head;
        while (current != NULL) {
                ei_linked_rect_t* next = current->next;
                free(current);
                current = next;
        }
        invalidated_rects_head = NULL;
}

sigjmp_buf jump_buffer;

void sigsegv_handler(int signum) {
        siglongjmp(jump_buffer, 1);
}

int is_valid_address(void *ptr) {
        struct sigaction sa;
        sa.sa_handler = sigsegv_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGSEGV, &sa, NULL);

        if (sigsetjmp(jump_buffer, 1) == 0) {
                // Try to read from the pointer
                volatile int value = *((volatile int *)ptr);
                (void)value;
                return 1; // Address is valid
        } else {
                return 0; // Address is not valid
        }
}