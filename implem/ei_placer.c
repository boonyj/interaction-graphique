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
        int width_value = (width != NULL) ? *width : (widget->requested_size.width != NULL) ? widget->requested_size.width: 0;
        int height_value = (height != NULL) ? *height : (widget->requested_size.height != NULL) ? widget->requested_size.height: 0;
        float rel_x_value = (rel_x != NULL) ? *rel_x : 0.0;
        float rel_y_value = (rel_y != NULL) ? *rel_y : 0.0;
        float rel_width_value = (rel_width != NULL) ? *rel_width : 0.0;
        float rel_height_value = (rel_height != NULL) ? *rel_height : 0.0;

        widget->screen_location.top_left.x = x_value;
        widget->screen_location.top_left.y = y_value;
        widget->screen_location.size.width = width_value;
        widget->screen_location.size.height = height_value;

}



