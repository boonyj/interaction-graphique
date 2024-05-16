#include "ei_frame.h"
#include "ei_draw.h"
#include "ei_draw_tool.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = calloc(1,sizeof(struct frame_t));

        return widget;
}

void frame_releasefunc (ei_widget_t	widget){
}

void draw_frame (frame_t* frame,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        assertion_pick_color((ei_widget_t) frame, surface);

        if (frame->widget.wclass->drawfunc != NULL) {
                ei_color_t color = {frame->widget.color->red, frame->widget.color->green, frame->widget.color->blue};

                ei_point_t *points = NULL;
                size_t points_size;

                switch (frame->relief) {
                        case ei_relief_none :
                                ei_fill(surface, frame->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_fill(pick_surface, frame->widget.pick_color, clipper);
                                }
                                break;
                        case ei_relief_raised :
                                assertion_color(frame->widget.color, color, 1);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper, frame->widget.screen_location.size);

                                points_size = 5;

                                ei_draw_polygon(surface, points, points_size,
                                                *frame->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *frame->widget.pick_color, clipper);
                                }

                                assertion_color(frame->widget.color, color, 2);


                                points[4].x = clipper->top_left.x + frame->widget.screen_location.size.width;
                                points[4].y = clipper->top_left.y + frame->widget.screen_location.size.height;

                                ei_draw_polygon(surface, points, points_size,
                                                *frame->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *frame->widget.pick_color, clipper);
                                }

                                calculate_clipper_sans_border(clipper, frame->border_width);

                                assertion_color(frame->widget.color, color, 0);

                                ei_fill(surface, frame->widget.color, clipper);

                                calculate_clipper_avec_border(clipper, frame->border_width);
                                break;
                        case ei_relief_sunken :
                                assertion_color(frame->widget.color, color, 2);

                                points = malloc(5 * sizeof(ei_point_t));
                                calculate_haut_sans_corner_radius(points, clipper, frame->widget.screen_location.size);

                                points_size = 5;

                                ei_draw_polygon(surface, points, points_size,
                                                *frame->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *frame->widget.pick_color, clipper);
                                }

                                assertion_color(frame->widget.color, color, 1);


                                points[4].x = clipper->top_left.x + frame->widget.screen_location.size.width;
                                points[4].y = clipper->top_left.y + frame->widget.screen_location.size.height;

                                ei_draw_polygon(surface, points, points_size,
                                                *frame->widget.color, clipper);
                                if (pick_surface != NULL) {
                                        ei_draw_polygon(pick_surface, points, points_size,
                                                        *frame->widget.pick_color, clipper);
                                }

                                calculate_clipper_sans_border(clipper, frame->border_width);

                                assertion_color(frame->widget.color, color, 0);

                                ei_fill(surface, frame->widget.color, clipper);

                                calculate_clipper_avec_border(clipper, frame->border_width);
                                break;
                }
                if ((frame->text) != NULL){
                        printf("%p ", &(frame->text));
                        printf("%s", (frame->text));
                        if (*(frame->text) != NULL){
                                //ei_draw_text(surface,&(frame->widget.screen_location.top_left), (ei_string_t){"2"}, frame->text_font, frame->text_color, clipper);
                               draw_text((frame->text), frame->text_font, frame->text_color, frame->widget.screen_location.top_left,
                                         frame->widget.screen_location.size, surface, clipper);}
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
        frame->text_color = ei_font_default_color;
        frame->text_anchor = ei_anc_none;
        frame->text_font = ei_default_font;
        frame->widget.requested_size.width = 300;
        frame->widget.requested_size.height = 200;
        frame->widget.color->red = ei_default_background_color.red;
        frame->widget.color->green = ei_default_background_color.green;
        frame->widget.color->blue = ei_default_background_color.blue;
        frame->widget.color->alpha = ei_default_background_color.alpha;
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