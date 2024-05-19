#include "ei_implementation.h"
#include "ei_event.h"

typedef struct ei_event_bind_widget_t{
    ei_event_t* event;
    ei_impl_widget_t* widget;
}ei_event_bind_widget_t;

void run_all_ei_place(ei_widget_t widget);

void draw_all_buttons_raised (ei_widget_t widget);

bool callback_move_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_move_resizing_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_resize_toplevel_start (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_toplevel_move_front(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);