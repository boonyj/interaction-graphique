#include "ei_frame.h"
#include "ei_draw.h"
#include "ei_draw_tool.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = calloc(1,sizeof(struct frame_t));
        return widget;
}

void frame_releasefunc (ei_widget_t	widget){
}

void draw_frame (frame_t* child,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        if (child->widget.wclass->drawfunc != NULL) {
                ei_color_t color = {child->widget.color->red, child->widget.color->green, child->widget.color->blue};

                ei_point_t *points = NULL;
                size_t points_size;

                switch (child->relief) {
                        case ei_relief_none :
                                ei_fill(surface, child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_fill(pick_surface, child->widget.pick_color, clipper);
                                }
                                break;
                        case ei_relief_raised :
                                assertion_color(child->widget.color, color, 1);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);

                                points_size = 5;

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

                                calculate_clipper_avec_border(clipper, child->border_width);
                                break;
                        case ei_relief_sunken :
                                assertion_color(child->widget.color, color, 2);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper,child->widget.screen_location.size);

                                points_size = 5;

                                ei_draw_polygon(surface, points, points_size,
                                                *child->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *child->widget.pick_color, clipper);
                                }

                                assertion_color(child->widget.color, color, 1);


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

                                calculate_clipper_avec_border(clipper, child->border_width);
                                break;
                }
                if (child->text != NULL){
                        draw_text(child->text, child->text_font, child->text_color, child->widget.screen_location.top_left,
                                  child->widget.screen_location.size, surface, clipper);
                }
        }
}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        draw_frame((frame_t *) widget, surface, pick_surface, clipper);
}

void frame_setdefaultsfunc(ei_widget_t		widget){
        frame_t* frame = (frame_t*) widget;
        frame->widget = *widget;
        frame->relief = ei_relief_none;
        frame->border_width = 0;
}

ei_widgetclass_t* create_frame_class() {
        ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(frame->name, "frame");
        frame->allocfunc =  &frame_allocfunc;
        frame->releasefunc = &frame_releasefunc;
        frame->drawfunc = &frame_drawfunc;
        frame->setdefaultsfunc = &frame_setdefaultsfunc;

        return frame;
}