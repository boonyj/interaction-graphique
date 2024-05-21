#include "ei_placeur.h"

void placeur_runfunc(ei_widget_t widget){
        placeur_param* placeur = (placeur_param*) widget->geom_params;
        ei_rect_t* new_location = malloc(sizeof(ei_rect_t));
        if (widget->parent != NULL) {
                        ei_widget_t parent = widget->parent;
                        int parent_width = parent->screen_location.size.width;
                        int parent_height = parent->screen_location.size.height;

                        int parent_coordinate_x =
                                (int) ((float) parent_width * placeur->rel_x) + parent->screen_location.top_left.x;
                        int parent_coordinate_y =
                                (int) ((float) parent_height * placeur->rel_y) + parent->screen_location.top_left.y;


                        placeur->width = (placeur->rel_width != 0.0) ? (int) ((float) parent_width * placeur->rel_width)
                                                                     : placeur->width;
                        placeur->height = (placeur->rel_height != 0.0) ? (int) ((float) parent_height *
                                                                                placeur->rel_height) : placeur->height;

                        switch (placeur->anchor) {
                                case ei_anc_center:
                                        new_location->top_left.x =
                                                placeur->x + parent_coordinate_x - placeur->width / 2;
                                        new_location->top_left.y =
                                                placeur->y + parent_coordinate_y - placeur->height / 2;
                                        break;
                                case ei_anc_north:
                                        new_location->top_left.x =
                                                placeur->x + parent_coordinate_x - placeur->width / 2;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y;
                                        break;
                                case ei_anc_northeast:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x - placeur->width;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y;
                                        break;
                                case ei_anc_east:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x - placeur->width;
                                        new_location->top_left.y =
                                                placeur->y + parent_coordinate_y - placeur->height / 2;
                                        break;
                                case ei_anc_southeast:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x - placeur->width;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y - placeur->height;
                                        break;
                                case ei_anc_south:
                                        new_location->top_left.x =
                                                placeur->x + parent_coordinate_x - placeur->width / 2;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y - placeur->height;
                                        break;
                                case ei_anc_southwest:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y - placeur->height;
                                        break;
                                case ei_anc_west:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x;
                                        new_location->top_left.y =
                                                placeur->y + parent_coordinate_y - placeur->height / 2;
                                        break;
                                case ei_anc_northwest:
                                        new_location->top_left.x = placeur->x + parent_coordinate_x;
                                        new_location->top_left.y = placeur->y + parent_coordinate_y;
                                        break;
                                case ei_anc_none:
                                        break;
                        }
                        new_location->size.width = placeur->width;
                        new_location->size.height = placeur->height;

                ei_geometry_run_finalize(widget, new_location);
        }
        free(new_location);
}

void placeur_releasefunc(ei_widget_t widget){

}

ei_geometrymanager_t* create_placeur_mng (){
        ei_geometrymanager_t* placeur = malloc(sizeof(ei_geometrymanager_t));
        strcpy(placeur->name, "placeur");
        placeur->runfunc =  &placeur_runfunc;
        placeur->releasefunc = &placeur_releasefunc;

        return placeur;
}