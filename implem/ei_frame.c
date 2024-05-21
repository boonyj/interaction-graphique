#include "ei_frame.h"

ei_widget_t frame_allocfunc (){
        ei_widget_t widget = calloc(1,sizeof(struct frame_t));

        return widget;
}

void frame_releasefunc (ei_widget_t	widget){
        frame_t * frame = (frame_t*) widget;

        /*ei_impl_widget_t widget;
        int border_width;
        ei_relief_t relief;
        ei_color_t text_color;
        ei_font_t text_font;
        char* text;
        ei_anchor_t text_anchor;
        ei_surface_t img;
        ei_rect_t img_rect;
        ei_anchor_t img_anchor;*/

        free(frame->text);
        if (frame->img != NULL) {
                hw_surface_free(frame->img);
        }
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
                       draw_text((frame->text), frame->text_font, frame->text_color, &(frame->widget.screen_location),
                                 surface, clipper, frame->text_anchor);
                }
                free(points);
        }
        if (frame->img != NULL){
                draw_image_from_surface(surface, frame->img, &(frame->widget.screen_location), &(frame->img_rect), frame->img_anchor);
        }
}

void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        if (widget != root) {
                if (ei_widget_is_displayed(widget)) {
                        draw_frame((frame_t *) widget, surface, pick_surface, clipper);
                }
        } else {
                draw_frame((frame_t *) widget, surface, pick_surface, clipper);
        }
}

void frame_setdefaultsfunc(ei_widget_t		widget){
        frame_t* frame = (frame_t*) widget;
        frame->widget = *widget;
        frame->relief = ei_relief_none;
        frame->border_width = 0;
        frame->text = NULL;
        frame->text_color = ei_font_default_color;
        frame->text_anchor = ei_anc_center;
        frame->text_font = ei_default_font;
        frame->img = NULL;
        frame->img_anchor = ei_anc_center;
        frame->widget.requested_size.width = 125;
        frame->widget.requested_size.height = 20;
        frame->widget.screen_location.size.width = 125;
        frame->widget.screen_location.size.height = 20;
        frame->widget.color->red = ei_default_background_color.red;
        frame->widget.color->green = ei_default_background_color.green;
        frame->widget.color->blue = ei_default_background_color.blue;
        frame->widget.color->alpha = ei_default_background_color.alpha;
}

void frame_geomnotifyfunc(ei_widget_t		widget){

}


ei_widgetclass_t* create_frame_class() {
        ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(frame->name, "frame");
        frame->allocfunc =  &frame_allocfunc;
        frame->releasefunc = &frame_releasefunc;
        frame->drawfunc = &frame_drawfunc;
        frame->setdefaultsfunc = &frame_setdefaultsfunc;
        frame->geomnotifyfunc = &frame_geomnotifyfunc;

        return frame;
}