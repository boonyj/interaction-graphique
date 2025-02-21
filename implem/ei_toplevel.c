#include "ei_toplevel.h"

ei_widget_t toplevel_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct toplevel_t ));
        return widget;
}

void toplevel_releasefunc (ei_widget_t	widget){
        toplevel_t * toplevel = (toplevel_t*) widget;
        free(toplevel->title);
}

/**
 * @brief Draws a toplevel widget on the given surfaces.
 *
 * @param toplevel A pointer to the toplevel widget to be drawn.
 * @param surface The surface on which to draw the widget.
 * @param pick_surface The picking surface.
 * @param clipper The clipping rectangle.
 */
void draw_toplevel (toplevel_t * toplevel,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t* clipper) {
        assertion_pick_color((ei_widget_t) toplevel, surface);

        ei_color_t color = {toplevel->widget.color->red, toplevel->widget.color->green, toplevel->widget.color->blue};

        assertion_color(toplevel->widget.color, color, 2);

        int width = 0;
        int height = 0;
        hw_text_compute_size(toplevel->title,toplevel->title_font, &width, &height);

        ei_rect_t* clipper_title = malloc(sizeof (ei_rect_t));
        clipper->size.width += toplevel->border_width*2;
        clipper->size.height += toplevel->border_width*2+height;
        clipper->top_left.x -= toplevel->border_width;
        clipper->top_left.y -= toplevel->border_width +height;

        clipper_title->top_left.x = clipper->top_left.x;
        clipper_title->top_left.y = clipper->top_left.y;

        clipper_title->size.width = clipper->size.width;
        clipper_title->size.height = height;

        ei_point_t *center_corner = malloc(sizeof(ei_point_t));
        ei_point_t *points = NULL;
        int radius = 15;
        points = malloc((2*nb_segments + 2) * sizeof(ei_point_t));

        center_corner->x = clipper_title->top_left.x + clipper_title->size.width;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[0].x = center_corner->x;
        points[0].y = center_corner->y;

        center_corner->x = clipper_title->top_left.x +  clipper_title->size.width  - radius;
        center_corner->y = clipper_title->top_left.y + radius;
        generate_rounded_corner(*center_corner, radius, 0, 90, points,1);

        center_corner->x = clipper_title->top_left.x + radius;
        center_corner->y = clipper_title->top_left.y + radius;
        generate_rounded_corner(*center_corner, radius, 90, 180, points,nb_segments + 1);

        center_corner->x = clipper_title->top_left.x ;
        center_corner->y = clipper_title->top_left.y + clipper_title->size.height;
        points[nb_segments*2 + 1].x = center_corner->x;
        points[nb_segments*2 + 1].y = center_corner->y;

        size_t points_size = 2*nb_segments + 2;
        ei_draw_polygon(surface, points, points_size,
                        *toplevel->widget.color, clipper);
        if (pick_surface != NULL) {
                ei_draw_polygon(pick_surface, points, points_size,
                                *toplevel->widget.pick_color, clipper);
        }

        ei_rect_t* clipper_content = toplevel->widget.content_rect;
        clipper_content->top_left.x = clipper->top_left.x;
        clipper_content->top_left.y = clipper->top_left.y + height;

        clipper_content->size.width += 2 * toplevel->border_width;
        clipper_content->size.height += 2 * toplevel->border_width;

        ei_fill(surface, toplevel->widget.color, clipper_content);
        if (pick_surface != NULL) {
                ei_fill(pick_surface, toplevel->widget.pick_color, clipper_content);
        }

        calculate_clipper_without_border(clipper_content, toplevel->border_width);
        assertion_color(toplevel->widget.color, color, 0);

        ei_fill(surface, toplevel->widget.color, clipper_content);

        ei_point_t where = toplevel->widget.screen_location.top_left;
        where.x += 30;
        ei_draw_text(surface, &where, toplevel->title, toplevel->title_font, toplevel->title_color, clipper);

        clipper->size.width -= toplevel->border_width*2;
        clipper->size.height -= toplevel->border_width*2+height;
        clipper->top_left.x += toplevel->border_width;
        clipper->top_left.y += toplevel->border_width +height;

        free(points);
        free(center_corner);
        free(clipper_title);
}

void toplevel_drawfunc (ei_widget_t		widget,
                      ei_surface_t		surface,
                      ei_surface_t		pick_surface,
                      ei_rect_t*		clipper){
        if (ei_widget_is_displayed(widget)) {
                draw_toplevel((toplevel_t *) widget, surface, pick_surface, clipper);
        }
}

void toplevel_setdefaultsfunc(ei_widget_t widget){
        toplevel_t * toplevel = (toplevel_t*) widget;
        toplevel->widget = *widget;
        toplevel->title = NULL;
        toplevel->title_font = ei_default_font;
        toplevel->title_color = ei_font_default_color;
        toplevel->widget.color->red = ei_default_background_color.red;
        toplevel->widget.color->green = ei_default_background_color.green;
        toplevel->widget.color->blue = ei_default_background_color.blue;
        toplevel->widget.color->alpha = ei_default_background_color.alpha;
        toplevel->widget.requested_size.width = 320;
        toplevel->widget.requested_size.height = 240;
        toplevel->widget.content_rect->size.width = 320;
        toplevel->widget.content_rect->size.height = 240;
        toplevel->widget.screen_location.size.width = 320;
        toplevel->widget.screen_location.size.height = 240;
        toplevel->widget.screen_location.top_left.x = 0;
        toplevel->widget.screen_location.top_left.y = 0;
        toplevel->closable = true;
        toplevel->resizable = ei_axis_both;
        toplevel->min_size.width = 160;
        toplevel->min_size.height = 120;
        toplevel->border_width = 4;

}

void toplevel_geomnotifyfunc(ei_widget_t widget){
        toplevel_t * toplevel = (toplevel_t*) widget;
        int width = 0;
        int height = 0;
        hw_text_compute_size(toplevel->title,toplevel->title_font, &width, &height);

        ei_place			(toplevel->widget.children_head, &(ei_anchor_t){ei_anc_northwest},
                                         &(int){-(toplevel->border_width)+8}, &(int){-(height+toplevel->border_width)+5}, NULL, NULL,
                                         &(float){0.0f}, &(float){0.0f},
                                         NULL, NULL);

        ei_place			(toplevel->widget.children_head->next_sibling, &(ei_anchor_t){ei_anc_southeast},
                                         &(int){0}, &(int){0}, NULL, NULL,
                                         &(float){1.0f}, &(float){1.0f},
                                         NULL, NULL);
}

ei_widgetclass_t* create_toplevel_class() {
        ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));
        strcpy(toplevel->name, "toplevel");
        toplevel->allocfunc =  &toplevel_allocfunc;
        toplevel->releasefunc = &toplevel_releasefunc;
        toplevel->drawfunc = &toplevel_drawfunc;
        toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
        toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
        return toplevel;
}