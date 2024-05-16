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
        if (widget->geom_params == NULL) {
                widget->geom_params = malloc(sizeof(struct placeur_param));
                widget->geom_params->manager = malloc(sizeof(ei_geometrymanager_t));
                ei_geometrymanager_t* type_geom_mng = ei_geometrymanager_from_name("placeur");
                strcpy(widget->geom_params->manager->name, "placeur");
                widget->geom_params->manager->runfunc = type_geom_mng->runfunc;
                widget->geom_params->manager->releasefunc = type_geom_mng->releasefunc;
        }

        placeur_param* placeur = (placeur_param*) widget->geom_params;

        placeur->anchor = (anchor != NULL) ? *anchor : ei_anc_northwest;
        printf("Valeur x : %d\n",*x);
        placeur->x = (x != NULL) ? *x : 0;
        placeur->y = (y != NULL) ? *y : 0;
        placeur->width = (width != NULL) ? *width : (widget->requested_size.width != 0) ? widget->requested_size.width: 0;
        placeur->height = (height != NULL) ? *height : (widget->requested_size.height != 0) ? widget->requested_size.height: 0;
        placeur->rel_x = (rel_x != NULL) ? *rel_x : 0.0;
        placeur->rel_y = (rel_y != NULL) ? *rel_y : 0.0;
        placeur->rel_width = (rel_width != NULL) ? *rel_width : 0.0;
        placeur->rel_height = (rel_height != NULL) ? *rel_height : 0.0;

        placeur->geom_mng.manager->runfunc(widget);
        printf("x : %d, y : %d\n",placeur->x, placeur->y);


}



