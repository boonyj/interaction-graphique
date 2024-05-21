#include "ei_placer.h"
#include "ei_implementation.h"
#include "ei_placeur.h"

/**
 * \brief	Places a widget within its parent according to the specified geometry parameters.
 * 		<ul>
 * 			<li> If the widget is NULL, the function returns immediately. </li>
 * 			<li> If the widget's geometry manager's parameters are not set, it initializes them. </li>
 * 			<li> Allocates memory for the geometry manager and sets its name and function pointers. </li>
 * 			<li> Sets the placer parameters (anchor, x, y, width, height, rel_x, rel_y, rel_width, rel_height) based on the provided arguments or default values. </li>
 * 			<li> If the geometry parameters are already set, it updates them with the provided values or retains the existing ones. </li>
 * 			<li> Calls the run function of the geometry manager to position and size the widget. </li>
 * 		</ul>
 *
 * @param	widget		The widget to be placed.
 * @param	anchor		Optional, the anchor position for the widget. If NULL, defaults to northwest.
 * @param	x		Optional, the x coordinate for the widget. If NULL, defaults to 0.
 * @param	y		Optional, the y coordinate for the widget. If NULL, defaults to 0.
 * @param	width		Optional, the width of the widget. If NULL, uses the widget's current width or 0.
 * @param	height		Optional, the height of the widget. If NULL, uses the widget's current height or 0.
 * @param	rel_x		Optional, the relative x coordinate for the widget. If NULL, defaults to 0.0.
 * @param	rel_y		Optional, the relative y coordinate for the widget. If NULL, defaults to 0.0.
 * @param	rel_width	Optional, the relative width of the widget. If NULL, defaults to 0.0.
 * @param	rel_height	Optional, the relative height of the widget. If NULL, defaults to 0.0.
 */
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
                placeur->width = (width != NULL) ? *width : (widget->screen_location.size.width != 0) ? widget->screen_location.size.width: 0;
                placeur->height = (height != NULL) ? *height : (widget->screen_location.size.height != 0) ? widget->screen_location.size.height: 0;
                placeur->rel_x = (rel_x != NULL) ? *rel_x : 0.0;
                placeur->rel_y = (rel_y != NULL) ? *rel_y : 0.0;
                placeur->rel_width = (rel_width != NULL) ? *rel_width : 0.0;
                placeur->rel_height = (rel_height != NULL) ? *rel_height : 0.0;
        } else {
                placeur = (placeur_param*) widget->geom_params;

                placeur->anchor = (anchor != NULL) ? *anchor : placeur->anchor;
                placeur->x = (x != NULL) ? *x : placeur->x;
                placeur->y = (y != NULL) ? *y : placeur->y;
                placeur->width = (width != NULL) ? *width : (widget->screen_location.size.width != 0) ? widget->screen_location.size.width: placeur->width;
                placeur->height = (height != NULL) ? *height : (widget->screen_location.size.height != 0) ? widget->screen_location.size.height: placeur->height;
                placeur->rel_x = (rel_x != NULL) ? *rel_x : placeur->rel_x;
                placeur->rel_y = (rel_y != NULL) ? *rel_y : placeur->rel_y;
                placeur->rel_width = (rel_width != NULL) ? *rel_width : placeur->rel_width;
                placeur->rel_height = (rel_height != NULL) ? *rel_height : placeur->rel_height;
        }

        placeur->geom_mng.manager->runfunc(widget);
}