#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_global.h"
#include "ei_toplevel.h"

static ei_geometrymanager_t** geo_mgr_list = NULL;
static size_t geo_mgr_list_size = 0;


void	ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location){
        // Vérifier si le widget est valide
        if (widget == NULL || new_screen_location == NULL) {
                // Gérer l'erreur ou retourner
                return;
        }

        int text_width = 0;
        int text_height = 0;

        if(strcmp(widget->wclass->name,"toplevel") == 0){
                toplevel_t* toplevel = (toplevel_t*) widget;
                hw_text_compute_size(toplevel->title,toplevel->title_font,&text_width,&text_height);
        }

        if (new_screen_location->top_left.x < 0) {
                new_screen_location->top_left.x = 0;
        }
        if (new_screen_location->top_left.y - text_height < 0) {
                new_screen_location->top_left.y = 0 + text_height;
        }
        if (new_screen_location->top_left.x + new_screen_location->size.width > root_size->width) {
                new_screen_location->top_left.x = root_size->width - new_screen_location->size.width;
        }
        if (new_screen_location->top_left.y + new_screen_location->size.height > root_size->height) {
                new_screen_location->top_left.y = root_size->height - new_screen_location->size.height;
        }

        widget->screen_location = *new_screen_location;
        widget->wclass->geomnotifyfunc(widget);
}

void	ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
        if (geo_mgr_list == NULL) {
                // Première classe à être enregistrée
                geo_mgr_list = malloc(sizeof(ei_geometrymanager_t*));
                if (geo_mgr_list == NULL) {
                        // Échec de l'allocation mémoire
                        return;
                }
                geo_mgr_list[0] = geometrymanager;
                geo_mgr_list_size = 1;
        } else {
                // Vérifie si la classe existe déjà
                if (ei_geometrymanager_from_name(geometrymanager->name) == NULL) {
                        // Augmente la taille du tableau et ajoute la nouvelle classe
                        ei_geometrymanager_t** new_geo_mgr_list = realloc(geo_mgr_list, (geo_mgr_list_size + 1) * sizeof(ei_geometrymanager_t*));
                        if (new_geo_mgr_list != NULL) {
                                geo_mgr_list = new_geo_mgr_list;
                                geo_mgr_list[geo_mgr_list_size] = geometrymanager;
                                geo_mgr_list_size++;
                        }
                }
        }
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
        if (geo_mgr_list != NULL) {
                for (size_t i = 0; i < geo_mgr_list_size; ++i) {
                        if (strcmp(geo_mgr_list[i]->name, name) == 0) {
                                return geo_mgr_list[i];
                        }
                }
        }
        return NULL;
}

void			ei_geometrymanager_unmap	(ei_widget_t widget){

}

ei_geometrymanager_t*	ei_widget_get_geom_manager	(ei_widget_t widget){

}


void			ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager){

}

ei_geom_param_t		ei_widget_get_geom_params	(ei_widget_t widget){

}


void			ei_widget_set_geom_params	(ei_widget_t widget, ei_geom_param_t geom_param){

}


