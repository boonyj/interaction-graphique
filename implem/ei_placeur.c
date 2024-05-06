#include "ei_placeur.h"

void placeur_runfunc(ei_widget_t widget){

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