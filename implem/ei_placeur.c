#include "ei_placeur.h"
#include "ei_implementation.h"

void placeur_runfunc(ei_widget_t widget){
        placeur_param* placeur = (placeur_param*) widget->geom_params;
        ei_rect_t* new_location = malloc(sizeof(ei_rect_t));
        if (widget->parent != NULL) {
                //if (placeur->rel_x != 0.0 && placeur->rel_y != 0.0) {
                        ei_widget_t parent = widget->parent;
                        int parent_width = parent->screen_location.size.width;
                        int parent_height = parent->screen_location.size.height;

                        /*printf("Parent (%s): w = %d, h = %d\n", parent->wclass->name,
                               parent->screen_location.size.width, parent->screen_location.size.height);
                        printf("Parent topleft (%s): x = %d, y = %d\n", parent->wclass->name,
                               parent->screen_location.top_left.x, parent->screen_location.top_left.y);
                        printf("Placeur REL : x = %f, y = %f\n", placeur->rel_x, placeur->rel_y);*/

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
                                        /*printf("NEW PLACER VALUE : x = %d, y = %d\n", placeur->x, placeur->y);
                                        printf("NEW PARENT COORDINATE : x = %d, y = %d\n", parent_coordinate_x,
                                               parent_coordinate_y);
                                        printf("NEW LOCATION : x = %d, y = %d\n", new_location->top_left.x,
                                               new_location->top_left.y);*/
                                        break;
                                case ei_anc_none:
                                        break;
                        }
                        new_location->size.width = placeur->width;
                        new_location->size.height = placeur->height;


                /*} else {
                        new_location->top_left.x = widget->screen_location.top_left.x + placeur->x;
                        new_location->top_left.y = widget->screen_location.top_left.y + placeur->y;
                        new_location->size.width = placeur->width;
                        new_location->size.height = placeur->height;
                }*/
                ei_geometry_run_finalize(widget, new_location);
        }
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