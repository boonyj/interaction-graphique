#include "ei_placer.h"
#include "ei_implementation.h"
#include "ei_widget_configure.h"
#include "ei_toplevel.h"
#include "ei_placeur.h"
#include "ei_event.h"
#include "ei_button.h"
#include "ei_global.h"

void		ei_place	(ei_widget_t		widget,
                                     ei_anchor_t*		anchor,
                                     int*			x,
                                     int*			y,
                                     int*			width,
                                     int*			height,
                                     float*			rel_x,
                                     float*			rel_y,
                                     float*			rel_width,
                                     float*			rel_height){
        if (widget == NULL) {
                return;
        }

        placeur_param* placeur;
        if (widget->geom_params == NULL) {
                widget->geom_params = malloc(sizeof(struct placeur_param));
                widget->geom_params->manager = malloc(sizeof(ei_geometrymanager_t));
                ei_geometrymanager_t* type_geom_mng = ei_geometrymanager_from_name("placeur");
                strcpy(widget->geom_params->manager->name, "placeur");
                widget->geom_params->manager->runfunc = type_geom_mng->runfunc;
                widget->geom_params->manager->releasefunc = type_geom_mng->releasefunc;

                placeur = (placeur_param*) widget->geom_params;

                placeur->anchor = (anchor != NULL) ? *anchor : ei_anc_northwest;
                placeur->x = (x != NULL) ? *x : 0;
                placeur->y = (y != NULL) ? *y : 0;
                placeur->width = (width != NULL) ? *width : (widget->requested_size.width != 0) ? widget->requested_size.width: 0;
                placeur->height = (height != NULL) ? *height : (widget->requested_size.height != 0) ? widget->requested_size.height: 0;
                placeur->rel_x = (rel_x != NULL) ? *rel_x : 0.0;
                placeur->rel_y = (rel_y != NULL) ? *rel_y : 0.0;
                placeur->rel_width = (rel_width != NULL) ? *rel_width : 0.0;
                placeur->rel_height = (rel_height != NULL) ? *rel_height : 0.0;
        } else {
                placeur = (placeur_param*) widget->geom_params;

                placeur->anchor = (anchor != NULL) ? *anchor : placeur->anchor;
                placeur->x = (x != NULL) ? *x : placeur->x;
                placeur->y = (y != NULL) ? *y : placeur->y;
                placeur->width = (width != NULL) ? *width : (widget->requested_size.width != 0) ? widget->requested_size.width: placeur->width;
                placeur->height = (height != NULL) ? *height : (widget->requested_size.height != 0) ? widget->requested_size.height: placeur->height;
                placeur->rel_x = (rel_x != NULL) ? *rel_x : placeur->rel_x;
                placeur->rel_y = (rel_y != NULL) ? *rel_y : placeur->rel_y;
                placeur->rel_width = (rel_width != NULL) ? *rel_width : placeur->rel_width;
                placeur->rel_height = (rel_height != NULL) ? *rel_height : placeur->rel_height;
        }

        placeur->geom_mng.manager->runfunc(widget);
        //printf("x : %d, y : %d\n",placeur->x, placeur->y);
}



