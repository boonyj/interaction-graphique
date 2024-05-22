#include <stdio.h>
#include <stdlib.h>
#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"

int main(int argc, char** argv)
{
        ei_widget_t	button1;
        ei_widget_t	button2;
        ei_widget_t	window1;
        ei_widget_t	window2;

        /* Create the application and change the color of the background. */
        ei_app_create			((ei_size_t){800, 600}, false);
        ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

        /* Create, configure and place a toplevel window that is x-axis resizable. */
        window1 = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
        ei_toplevel_configure		(window1, &(ei_size_t){320,240},
                                              &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
                                              &(int){2},
                                              &(ei_string_t){"X-Axis Resize"}, NULL, &(ei_axis_set_t){ei_axis_x}, NULL);
        ei_place_xy			(window1, 30, 10);

        /* Create, configure and place a button as a descendant of the toplevel window. */
        button1 = ei_widget_create	("button", window1, NULL, NULL);
        ei_button_configure		(button1, NULL,
                                            &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                                            &(int){2}, NULL,
                                            &(ei_relief_t){ei_relief_raised},
                                            &(ei_string_t){"X"}, NULL,
                                            &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                            NULL, NULL);
        ei_place			(button1, &(ei_anchor_t){ei_anc_southeast},
                                         &(int){-20}, &(int){-20}, NULL, NULL,
                                         &(float){1.0f}, &(float){1.0f},
                                         &(float){0.5f}, NULL);

        /* Create, configure and place a toplevel window that is y-axis resizable. */
        window2 = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
        ei_toplevel_configure		(window2, &(ei_size_t){320,240},
                                              &(ei_color_t){0xA0,0xA0,0xA0, 0xff},
                                              &(int){2},
                                              &(ei_string_t){"Y-Axis Resize"}, NULL, &(ei_axis_set_t){ei_axis_y}, NULL);
        ei_place_xy			(window2, 40, 40);

        /* Create, configure and place a button as a descendant of the toplevel window. */
        button2 = ei_widget_create	("button", window2, NULL, NULL);
        ei_button_configure		(button2, NULL,
                                            &(ei_color_t){0x88, 0x88, 0x88, 0xff},
                                            &(int){2}, NULL,
                                            &(ei_relief_t){ei_relief_raised},
                                            &(ei_string_t){"Y"}, NULL,
                                            &(ei_color_t){0x00, 0x00, 0x00, 0xff}, NULL, NULL, NULL, NULL,
                                            NULL, NULL);
        ei_place			(button2, &(ei_anchor_t){ei_anc_southeast},
                                         &(int){-20}, &(int){-20}, NULL, NULL,
                                         &(float){1.0f}, &(float){1.0f},
                                         &(float){0.5f}, NULL);


        ei_app_run();

        ei_app_free();

        return (EXIT_SUCCESS);
}
