#include "ei_geometrymanager.h"
#include "ei_implementation.h"

typedef struct placeur_param {
        ei_impl_geom_param_t geom_mng;
        ei_anchor_t anchor;
        int x;
        int y;
        int width;
        int height;
        float rel_x;
        float rel_y;
        float rel_width;
        float rel_height;
} placeur_param;

void placeur_runfunc(ei_widget_t widget);

void placeur_releasefunc(ei_widget_t widget);

ei_geometrymanager_t* create_placeur_mng();