#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_entry.h"
#include "ei_widget_configure.h"
#include "ei_widget_attributes.h"
#include "ei_placer.h"
#include "ei_geometrymanager.h"
#include "ei_utils.h"

ei_point_t		g_current_toplevel_position	= {20, 40};
const ei_point_t 	g_toplevel_position_increment	= {120, 140};


void create_login_window();

ei_size_t widget_size(ei_widget_t widget)
{
        ei_place_xy(widget, 0, 0);
        ei_rect_t wr = *ei_widget_get_screen_location(widget);
        ei_geometrymanager_unmap(widget);

        return wr.size;
}

bool button_cb(ei_widget_t widget, struct ei_event_t* event, ei_user_param_t user_param)
{
        ei_widget_t toplevel = (ei_widget_t)user_param;
        ei_widget_destroy(toplevel);
        return true;
}

void create_login_window()
{
        ei_widget_t	toplevel, label, entries[3];
        ei_size_t	label_size, edit_size;
        int		i, y, label_y_offset, margin = 6;

        // toplevel

        toplevel = ei_widget_create	("toplevel", ei_app_root_widget(), NULL, NULL);
        ei_toplevel_configure		(toplevel, NULL, NULL, NULL, &(ei_string_t){"Login information"}, NULL, &(ei_axis_set_t){ei_axis_x}, NULL);
        ei_place_xy			(toplevel, g_current_toplevel_position.x, g_current_toplevel_position.y);
        g_current_toplevel_position	= ei_point_add(g_current_toplevel_position, g_toplevel_position_increment);

        // size of the longest label

        label				= ei_widget_create("frame", toplevel, NULL, NULL);
        ei_frame_set_text		(label, "Password");
        label_size			= widget_size(label);

        // 3 lines, each with a label and an entry.

        const char* labels[] = {"Login", "Email", "Password" };

        for (i = 0, y = margin; i < 3; i++, y += 2*margin + edit_size.height) {
                if (i > 0)
                        label		= ei_widget_create("frame", toplevel, NULL, NULL);
                ei_frame_configure	(label, NULL, NULL, NULL, NULL, &(ei_string_t){ labels[i] },
                                           NULL, NULL, &(ei_anchor_t){ei_anc_east}, NULL, NULL, NULL);

                entries[i]		= ei_widget_create	("entry", toplevel, NULL, NULL);
                ei_entry_configure	(entries[i], &(int){30}, &(ei_color_t){0xff, 0xff, 0xff, 0xff}, NULL, NULL, NULL);
                if (i == 0) {
                        ei_entry_set_text(entries[i], "Some login");
                        edit_size	= widget_size(entries[i]);
                        label_y_offset	= (edit_size.height - label_size.height) / 2;
                }

                ei_place		(label, &(ei_anchor_t){ei_anc_northeast}, &(int){label_size.width + margin},
                                         &(int){y + label_y_offset}, NULL, NULL, NULL, NULL, NULL, NULL);
                ei_place		(entries[i], &(ei_anchor_t){ei_anc_northwest}, &(int){label_size.width + 2*margin}, &y,
                                         &(int){-label_size.width - 3*margin}, NULL, NULL, NULL, &(float){1.0f}, NULL);
        }
        ei_entry_give_focus(entries[0]);

        // resize toplevel

        const ei_rect_t* erect = ei_widget_get_screen_location(entries[2]);
        const ei_rect_t* trect = ei_widget_get_screen_location(toplevel);

        ei_widget_set_requested_size(toplevel, ei_size(erect->top_left.x - trect->top_left.x + erect->size.width + margin,
                                                       erect->top_left.y - trect->top_left.y + erect->size.height + 3*margin));
}


int main(int argc, char** argv)
{
        /* Create the application and change the color of the background. */
        ei_app_create			((ei_size_t){500, 400}, false);
        ei_frame_set_bg_color		(ei_app_root_widget(), (ei_color_t){0x52, 0x7f, 0xb4, 0xff});

        create_login_window();


        /* Run the application's main loop. */
        ei_app_run();


        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_app_free();

        return (EXIT_SUCCESS);
}
