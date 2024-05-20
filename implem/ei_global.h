#include "ei_event.h"
#include "hw_interface.h"

typedef struct ei_linked_event_t{
        ei_eventtype_t eventtype;
        ei_widget_t widget;
        ei_callback_t callback;
        ei_tag_t tag;
        void* user_param;
        struct ei_linked_event_t* next;
}ei_linked_event_t;

extern ei_surface_t main_surface;
extern ei_surface_t pick_surface;
extern ei_widget_t root;
extern ei_size_t* root_size;
extern ei_linked_rect_t* invalidated_rects_head;
extern ei_linked_event_t* linked_event_list;
extern double blinking;