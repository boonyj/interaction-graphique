#include "ei_placer.h"
#include "ei_implementation.h"

void		ei_place	(ei_widget_t		widget,
                                     ei_anchor_t*		anchor,
                                     int*			x,
                                     int*			y,
                                     int*			width,
                                     int*			height,
                                     float*			rel_x,
                                     float*			rel_y,
                                     float*			rel_width,
                                     float*			rel_height){
        if (widget == NULL) {
                return;
        }

        // Déterminer les valeurs à utiliser en fonction des paramètres fournis
        // Si un paramètre est NULL, utiliser la valeur par défaut
        ei_anchor_t anchor_value = (anchor != NULL) ? *anchor : ei_anc_northwest;
        int x_value = (x != NULL) ? *x : 0;
        int y_value = (y != NULL) ? *y : 0;
        int width_value = (width != NULL) ? *width : (widget->requested_size.width != 0) ? widget->requested_size.width: 0;
        int height_value = (height != NULL) ? *height : (widget->requested_size.height != 0) ? widget->requested_size.height: 0;
        float rel_x_value = (rel_x != NULL) ? *rel_x : 0.0;
        float rel_y_value = (rel_y != NULL) ? *rel_y : 0.0;
        float rel_width_value = (rel_width != NULL) ? *rel_width : 0.0;
        float rel_height_value = (rel_height != NULL) ? *rel_height : 0.0;

        if(rel_x != NULL || rel_y != NULL){
                ei_widget_t parent = widget->parent;
                int parent_width = parent->screen_location.size.width;
                int parent_height = parent->screen_location.size.height;

                int parent_coordinate_x = (int)((float)parent_width * rel_x_value);
                int parent_coordinate_y = (int)((float)parent_height * rel_y_value);

                width_value = (rel_width_value != 0.0) ? (int)((float)parent_width * rel_width_value) : width_value;
                height_value = (rel_width_value != 0.0) ? (int)((float)parent_height * rel_height_value) : height_value;

                switch (anchor_value) {
                        case ei_anc_center:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width/2 + x_value +parent_coordinate_x - width_value/2;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height/2 + y_value +parent_coordinate_y - height_value/2;
                                break;
                        case ei_anc_north:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width/2 + x_value +parent_coordinate_x - width_value/2;
                                widget->screen_location.top_left.y =  y_value +parent_coordinate_y ;
                                break;
                        case ei_anc_northeast:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width + x_value +parent_coordinate_x - width_value;
                                widget->screen_location.top_left.y =  y_value +parent_coordinate_y;
                                break;
                        case ei_anc_east:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width + x_value +parent_coordinate_x - width_value;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height/2 + y_value +parent_coordinate_y - height_value/2;
                                break;
                        case ei_anc_southeast:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width + x_value +parent_coordinate_x - width_value ;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height + y_value +parent_coordinate_y - height_value;
                                break;
                        case ei_anc_south:
                                widget->screen_location.top_left.x =  widget->screen_location.size.width/2 + x_value +parent_coordinate_x - width_value/2;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height + y_value +parent_coordinate_y - height_value;
                                break;
                        case ei_anc_southwest:
                                widget->screen_location.top_left.x =  x_value +parent_coordinate_x;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height + y_value +parent_coordinate_y - height_value;
                                break;
                        case ei_anc_west:
                                widget->screen_location.top_left.x =  x_value +parent_coordinate_x;
                                widget->screen_location.top_left.y =  widget->screen_location.size.height/2 + y_value +parent_coordinate_y - height_value/2;
                                break;
                        case ei_anc_northwest:
                                widget->screen_location.top_left.x =  x_value +parent_coordinate_x;
                                widget->screen_location.top_left.y =  y_value +parent_coordinate_y;
                                break;
                }
                widget->screen_location.size.width = width_value;
                widget->screen_location.size.height = height_value;

        } else{
                widget->screen_location.top_left.x = x_value;
                widget->screen_location.top_left.y = y_value;
                widget->screen_location.size.width = width_value;
                widget->screen_location.size.height = height_value;
        }



}



