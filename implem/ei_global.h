#include "ei_event.h"
#include "hw_interface.h"
#include "ei_implementation.h"

extern ei_surface_t main_surface;
extern ei_surface_t pick_surface;
extern ei_widget_t root;
extern ei_size_t* root_size;
extern ei_linked_rect_t* invalidated_rects_head;
extern ei_linked_event_t* linked_event_list;