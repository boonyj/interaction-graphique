#include "ei_button.h"
#include "ei_draw_tool.h"


ei_widget_t button_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct button_t));
        return widget;
}

void button_releasefunc (ei_widget_t	widget){

}

void draw_button (button_t * child,
                  ei_surface_t		surface,
                  ei_surface_t		pick_surface,
                  ei_rect_t*		clipper){
        // Call the draw function for each child widget
        if (child->border_width != 0) {
                if (child->widget.wclass->drawfunc != NULL) {
                        ei_color_t color = {child->widget.color->red, child->widget.color->green, child->widget.color->blue};

                        switch (child->relief) {
                                case ei_relief_none :
                                        assertion_color(child->widget.color, color, 2);

                                        if (child->corner_radius == 0) {
                                                child->widget.wclass->drawfunc(&(child->widget), surface, pick_surface,
                                                                               clipper);

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                ei_fill(surface, child->widget.color, clipper);

                                                if (pick_surface != NULL) {
                                                        ei_fill(pick_surface, child->widget.pick_color, clipper);
                                                }

                                        } else {
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 0);

                                                size_t points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                                case ei_relief_raised :
                                        assertion_color(child->widget.color, color, 1);

                                        if (child->corner_radius == 0){
                                                ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);
                                                size_t points_size = 5;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                assertion_color(child->widget.color, color, 2);


                                                points[4].x = clipper->top_left.x + child->widget.screen_location.size.width;
                                                points[4].y = clipper->top_left.y + child->widget.screen_location.size.height;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                ei_fill(surface, child->widget.color, clipper);

                                                if (pick_surface != NULL) {
                                                        ei_fill(pick_surface, child->widget.pick_color, clipper);
                                                }
                                        } else {
                                                // Partie haute
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                size_t points_size = 3*nb_segments+2;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Partie basse
                                                assertion_color(child->widget.color, color, 2);

                                                points = rounded_frame(*clipper, child->corner_radius, 2);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Draw inside button
                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                                case ei_relief_sunken :
                                        assertion_color(child->widget.color, color, 2);

                                        if (child->corner_radius == 0){
                                                ei_point_t *points = malloc(5 * sizeof(ei_point_t));
                                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);
                                                size_t points_size = 5;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                assertion_color(child->widget.color, color, 1);

                                                points += 4;
                                                points->x = clipper->top_left.x +
                                                            child->widget.screen_location.size.width;
                                                points->y = clipper->top_left.y +
                                                            child->widget.screen_location.size.height;
                                                points -= 4;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                ei_fill(surface, child->widget.color, clipper);

                                                if (pick_surface != NULL) {
                                                        ei_fill(pick_surface, child->widget.pick_color, clipper);
                                                }
                                        } else {
                                                // Partie haute
                                                ei_point_t *points = rounded_frame(*clipper, child->corner_radius, 1);
                                                size_t points_size = 3*nb_segments+2;

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Partie basse
                                                assertion_color(child->widget.color, color, 1);

                                                points = rounded_frame(*clipper, child->corner_radius, 2);

                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                                if (pick_surface != NULL) {
                                                        ei_draw_polygon(pick_surface, points, points_size,
                                                                        *child->widget.pick_color, clipper);
                                                }

                                                // Draw inside button
                                                calculate_clipper_sans_border(clipper, child->border_width);

                                                assertion_color(child->widget.color, color, 0);

                                                points = rounded_frame(*clipper, child->corner_radius, 0);

                                                points_size = 4*nb_segments;
                                                ei_draw_polygon(surface, points, points_size,
                                                                *child->widget.color, clipper);
                                        }
                                        calculate_clipper_avec_border(clipper, child->border_width);
                                        break;
                        }
                        if (child->text != NULL){
                                draw_text(child->text, child->text_font, child->text_color, child->widget.screen_location.top_left,
                                          child->widget.screen_location.size, surface, clipper);
                        }
                }
        }
}

void button_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        draw_button((button_t*) widget, surface, pick_surface, clipper);
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

}

ei_widgetclass_t* create_button_class() {
        ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
        strcpy(button->name, "button");
        button->allocfunc =  &button_allocfunc;
        button->releasefunc = &button_releasefunc;
        button->drawfunc = &button_drawfunc;
        button->setdefaultsfunc = &button_setdefaultsfunc;

        return button;
}
