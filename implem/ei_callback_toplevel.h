#ifndef PROJETC_IG_EI_CALLBACK_TOPLEVEL_H
#define PROJETC_IG_EI_CALLBACK_TOPLEVEL_H

#include "ei_implementation.h"
#include "ei_event.h"
#include "ei_widget_configure.h"
#include "ei_widget_attributes.h"
#include "ei_placer.h"
#include "ei_placeur.h"
#include "ei_button.h"
#include "ei_global.h"
#include "ei_toplevel.h"
#include "ei_frame.h"

void run_all_ei_place(ei_widget_t widget);

void draw_all_buttons_raised (ei_widget_t widget);

bool callback_move_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_top_level (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_move_resizing_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_resize_toplevel_start (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_toplevel_move_front(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_left_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_right_resize_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_fullscreen_toplevel(ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

#endif //PROJETC_IG_EI_CALLBACK_TOPLEVEL_H