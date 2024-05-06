#include "ei_placeur.h"
#include "ei_implementation.h"

void placeur_runfunc(ei_widget_t widget){
        //ei_rect_t* new_location = malloc(sizeof (ei_rect_t));
        //new_location->top_left.x = widget->screen_location.top_left.x;
        //new_location->top_left.y = widget->screen_location.top_left.y;
        //new_location->size.width = widget->screen_location.size.width;
        //new_location->size.height = widget->screen_location.size.height;

        ei_geometry_run_finalize(widget,NULL);
}

void placeur_releasefunc(ei_widget_t widget){

}

ei_geometrymanager_t* create_placeur (){
        ei_geometrymanager_t* placeur = malloc(sizeof(ei_geometrymanager_t));
        strcpy(placeur->name, "placeur");
        placeur->runfunc =  &placeur_runfunc;
        placeur->releasefunc = &placeur_releasefunc;

        return placeur;
}