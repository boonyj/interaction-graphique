#include "ei_global.h"

ei_surface_t main_surface = NULL;
ei_surface_t pick_surface = NULL;
ei_widget_t root = NULL;
ei_size_t* root_size = NULL;
ei_linked_rect_t* invalidated_rects_head = NULL;
ei_linked_event_t* linked_event_list = NULL;