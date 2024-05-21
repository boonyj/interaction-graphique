#include "ei_button.h"

ei_widget_t button_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct button_t));
        return widget;
}

void button_releasefunc (ei_widget_t	widget){
        button_t * button = (button_t*) widget;

        free(button->text);
        if (button->img != NULL) {
                hw_surface_free(button->img);
        }
}

void draw_button (button_t * button,
                  ei_surface_t		surface,
                  ei_surface_t		pick_surface,
                  ei_rect_t*		clipper){
        assertion_pick_color((ei_widget_t) button, surface);

        // Call the draw function for each button widget
        if (button->widget.wclass->drawfunc != NULL) {
                ei_color_t color = {button->widget.color->red, button->widget.color->green, button->widget.color->blue};

                switch (button->relief) {
                        case ei_relief_none :
                                assertion_color(button->widget.color, color, 2);

                                if (button->corner_radius == 0) {
                                        ei_fill(surface, button->widget.color, clipper);

                                        if (pick_surface != NULL) {
                                                ei_fill(pick_surface, button->widget.pick_color, clipper);
                                        }

                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        ei_fill(surface, button->widget.color, clipper);
                                } else {
                                        ei_point_t *points = rounded_frame(*clipper, button->corner_radius, 0);

                                        size_t points_size = 4*nb_segments;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        points = rounded_frame(*clipper, button->corner_radius, 0);

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);

                                        free(points);
                                }
                                calculate_clipper_avec_border(clipper, button->border_width);
                                break;
                        case ei_relief_raised :
                                assertion_color(button->widget.color, color, 1);

                                if (button->corner_radius == 0){
                                        ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                        calculate_haut_sans_corner_radius(points, clipper, button->widget.screen_location.size);
                                        size_t points_size = 5;

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        assertion_color(button->widget.color, color, 2);


                                        points[4].x = clipper->top_left.x + button->widget.screen_location.size.width;
                                        points[4].y = clipper->top_left.y + button->widget.screen_location.size.height;

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        ei_fill(surface, button->widget.color, clipper);

                                        free(points);
                                } else {
                                        // Partie haute
                                        ei_point_t *points = rounded_frame(*clipper, button->corner_radius, 1);
                                        size_t points_size = 3*nb_segments+2;

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }
                                        free(points);

                                        // Partie basse
                                        assertion_color(button->widget.color, color, 2);

                                        points = rounded_frame(*clipper, button->corner_radius, 2);

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }
                                        free(points);

                                        // Draw inside button
                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        points = rounded_frame(*clipper, button->corner_radius, 0);

                                        points_size = 4*nb_segments;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);

                                        free(points);
                                }
                                calculate_clipper_avec_border(clipper, button->border_width);
                                break;
                        case ei_relief_sunken :
                                assertion_color(button->widget.color, color, 2);

                                if (button->corner_radius == 0){
                                        ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                        calculate_haut_sans_corner_radius(points, clipper, button->widget.screen_location.size);
                                        size_t points_size = 5;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        assertion_color(button->widget.color, color, 1);

                                        points += 4;
                                        points->x = clipper->top_left.x +
                                                    button->widget.screen_location.size.width;
                                        points->y = clipper->top_left.y +
                                                    button->widget.screen_location.size.height;
                                        points -= 4;

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        ei_fill(surface, button->widget.color, clipper);

                                        free(points);
                                } else {
                                        // Partie haute
                                        ei_point_t *points = rounded_frame(*clipper, button->corner_radius, 1);
                                        size_t points_size = 3*nb_segments+2;

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }
                                        free(points);

                                        // Partie basse
                                        assertion_color(button->widget.color, color, 1);

                                        points = rounded_frame(*clipper, button->corner_radius, 2);

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }
                                        free(points);

                                        // Draw inside button
                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        points = rounded_frame(*clipper, button->corner_radius, 0);

                                        points_size = 4*nb_segments;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);

                                        free(points);
                                }
                                calculate_clipper_avec_border(clipper, button->border_width);
                                break;
                }
                if (button->text != NULL){
                        draw_text((button->text), button->text_font, button->text_color, &(button->widget.screen_location), surface,
                                  clipper, button->text_anchor);
                }
                if (button->img != NULL){
                        draw_image_from_surface(surface, button->img, &(button->widget.screen_location), &(button->img_rect),
                                                button->img_anchor);
                }
        }
}

void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        if (ei_widget_is_displayed(widget)) {
                draw_button((button_t*) widget, surface, pick_surface, clipper);
        }
}

void button_setdefaultsfunc(ei_widget_t		widget){
        button_t * button = (button_t*) widget;
        button->widget = *widget;
        button->relief = ei_relief_raised;
        button->widget.color->red = ei_default_background_color.red;
        button->widget.color->green = ei_default_background_color.green;
        button->widget.color->blue = ei_default_background_color.blue;
        button->widget.color->alpha = ei_default_background_color.alpha;
        button->border_width = k_default_button_border_width;
        button->corner_radius = k_default_button_corner_radius;
        button->text_color = ei_font_default_color;
        button->text_anchor = ei_anc_center;
        button->text_font = ei_default_font;
        button->text = NULL;
        button->widget.requested_size.width = 100;
        button->widget.requested_size.height = 48;
        button->widget.screen_location.size.width = 100;
        button->widget.screen_location.size.height = 48;
        button->img = NULL;
        button->widget.screen_location.top_left.x = 0;
        button->widget.screen_location.top_left.y = 0;
        //button->img_rect = NULL;
        button->img_anchor = ei_anc_center;
        button->user_param = NULL;
        button->callback =NULL;

}

void button_geomnotifyfunc(ei_widget_t		widget){

}

ei_widgetclass_t* create_button_class() {
        ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
        strcpy(button->name, "button");
        button->allocfunc =  &button_allocfunc;
        button->releasefunc = &button_releasefunc;
        button->drawfunc = &button_drawfunc;
        button->setdefaultsfunc = &button_setdefaultsfunc;
        button->geomnotifyfunc = &button_geomnotifyfunc;

        return button;
}
