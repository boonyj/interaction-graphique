/**
 *  @file	ei_application.h
 *  @brief	Manages the main steps of a graphical application: initialization, main window,
 *		main loop, quitting, resource freeing.
 *
 *  \author
 *  Created by François Bérard on 30.12.11.
 *  Copyright 2011 Ensimag. All rights reserved.
 *
 */

#ifndef EI_APPLICATION_H
#define EI_APPLICATION_H


#include "ei_types.h"
#include "ei_widget.h"
#include "ei_application.h"


void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        ei_surface_t			main_window		= NULL;
        ei_widget_t                     widget_class            = NULL;
//        if (fullscreen){
//                main_window_size.height = GetSystemMetrics(SM_CYSCREEN);
//                main_window_size.width = GetSystemMetrics(SM_CXSCREEN);
//        }

        hw_init();
        main_window = hw_create_window(main_window_size, false);
//        ei_widgetclass_register(widget_class);
//        ei_widget_create(widget_class);

}

void ei_app_free(void){

}

void ei_app_run(void){

}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

ei_widget_t ei_app_root_widget(void){

}

ei_surface_t ei_app_root_surface(void){

}




#endif

