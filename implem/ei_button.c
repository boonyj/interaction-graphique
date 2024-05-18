#include "ei_button.h"
#include "ei_draw_tool.h"
#include "ei_implementation.h"

ei_widget_t button_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct button_t));
        return widget;
}

void button_releasefunc (ei_widget_t	widget){
        free(widget->user_data);
        free(widget->color);
        free(widget->pick_color);
        free(widget->geom_params);
        free(widget->wclass);
        free(widget->content_rect);
        free_widget_and_siblings(&widget, true);
        widget->user_data = NULL;
        widget->color = NULL;
        widget->pick_color = NULL;
        widget->geom_params = NULL;
        widget->wclass = NULL;
        widget->content_rect = NULL;
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

                                        // Partie basse
                                        assertion_color(button->widget.color, color, 2);

                                        points = rounded_frame(*clipper, button->corner_radius, 2);

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        // Draw inside button
                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        points = rounded_frame(*clipper, button->corner_radius, 0);

                                        points_size = 4*nb_segments;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
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

                                        // Partie basse
                                        assertion_color(button->widget.color, color, 1);

                                        points = rounded_frame(*clipper, button->corner_radius, 2);

                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                        if (pick_surface != NULL) {
                                                ei_draw_polygon(pick_surface, points, points_size,
                                                                *button->widget.pick_color, clipper);
                                        }

                                        // Draw inside button
                                        calculate_clipper_sans_border(clipper, button->border_width);

                                        assertion_color(button->widget.color, color, 0);

                                        points = rounded_frame(*clipper, button->corner_radius, 0);

                                        points_size = 4*nb_segments;
                                        ei_draw_polygon(surface, points, points_size,
                                                        *button->widget.color, clipper);
                                }
                                calculate_clipper_avec_border(clipper, button->border_width);
                                break;
                }
                if (button->text != NULL){
                        draw_text((button->text), button->text_font, button->text_color, button->widget.screen_location.top_left,
                                  button->widget.screen_location.size, surface, clipper);
                }
                if (button->img != NULL){
                        draw_image_from_surface(surface, button->img, &(button->widget.screen_location.top_left), &(button->img_rect));
                }
        }
}


void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        if (widget->geom_params != NULL) {
                draw_button((button_t*) widget, surface, pick_surface, clipper);
        }
}

void button_setdefaultsfunc(ei_widget_t		widget){
        button_t * button = (button_t*) widget;
        button->widget = *widget;
        button->relief = ei_relief_none;
        button->border_width = 0;
        button->corner_radius = 0;
        button->text_color = ei_font_default_color;
        button->text_anchor = ei_anc_none;
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
