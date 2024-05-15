#include "ei_toplevel.h"
#include "ei_widget_configure.h"
#include "ei_draw_tool.h"
#include "ei_draw.h"

ei_widget_t toplevel_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct toplevel_t ));

        return widget;
}

void toplevel_releasefunc (ei_widget_t	widget){

}

void draw_toplevel (toplevel_t * toplevel,
                    ei_surface_t		surface,
                    ei_surface_t		pick_surface,
                    ei_rect_t*		clipper) {
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

        calculate_clipper_sans_border(clipper_content, toplevel->border_width);
        assertion_color(toplevel->widget.color, color, 0);

        ei_fill(surface, toplevel->widget.color, clipper_content);

        ei_point_t where = toplevel->widget.screen_location.top_left;
        where.x += 30;
        ei_draw_text(surface, &where, toplevel->title, toplevel->title_font, toplevel->title_color, clipper);
        //calculate_clipper_avec_border(clipper, toplevel->border_width);
        toplevel->widget.screen_location.top_left.y += height + 2*toplevel->border_width;

}

void toplevel_drawfunc (ei_widget_t		widget,
                      ei_surface_t		surface,
                      ei_surface_t		pick_surface,
                      ei_rect_t*		clipper){
        draw_toplevel((toplevel_t *) widget, surface, pick_surface, clipper);
}

void toplevel_setdefaultsfunc(ei_widget_t		widget){
        toplevel_t * toplevel = (toplevel_t*) widget;
        toplevel->widget = *widget;
        toplevel->title = NULL;
        toplevel->title_font = ei_default_font;
        toplevel->title_color = ei_font_default_color;

}

ei_widgetclass_t* create_toplevel_class() {
        ei_widgetclass_t* toplevel = malloc(sizeof(ei_widgetclass_t));
        strcpy(toplevel->name, "toplevel");
        toplevel->allocfunc =  &toplevel_allocfunc;
        toplevel->releasefunc = &toplevel_releasefunc;
        toplevel->drawfunc = &toplevel_drawfunc;
        toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;

        return toplevel;
}

