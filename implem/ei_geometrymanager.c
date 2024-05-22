#include "ei_geometrymanager.h"
#include "ei_implementation.h"
#include "ei_global.h"
#include "ei_toplevel.h"
#include "ei_widget.h"
#include "ei_widget_attributes.h"

/**
 * @brief A list of geometry managers available in the system.
 *        Each geometry manager is responsible for managing the position and size of widgets.
 */
static ei_geometrymanager_t** geo_mgr_list = NULL;

/**
 * @brief The number of geometry managers in the geo_mgr_list.
 */
static size_t geo_mgr_list_size = 0;

size_t ei_geom_param_size(){
        return sizeof(ei_impl_geom_param_t);
}

void ei_geometry_run_finalize(ei_widget_t widget, ei_rect_t* new_screen_location){
        if (widget == NULL || new_screen_location == NULL) {
                return;
        }

        int text_width = 0;
        int text_height = 0;
        if(strcmp(widget->wclass->name,"toplevel") == 0){
                toplevel_t* toplevel = (toplevel_t*) widget;
                hw_text_compute_size(toplevel->title,toplevel->title_font,&text_width,&text_height);

                //Border management
                if (new_screen_location->top_left.y - text_height < 0) {
                        new_screen_location->top_left.y = 0 + text_height;
                }
                if (new_screen_location->top_left.x + new_screen_location->size.width + toplevel->border_width > root_size->width) {
                        new_screen_location->top_left.x = root_size->width - new_screen_location->size.width - toplevel->border_width;
                }
                if (new_screen_location->top_left.y + new_screen_location->size.height + toplevel->border_width > root_size->height) {
                        new_screen_location->top_left.y = root_size->height - new_screen_location->size.height - toplevel->border_width;
                }
                if(new_screen_location->top_left.x <= toplevel->border_width) {
                        new_screen_location->top_left.x = toplevel->border_width;
                }

                //Size limits
                switch (toplevel->resizable) {
                        case(ei_axis_both):
                                if (new_screen_location->size.height < toplevel->min_size.height) {
                                        new_screen_location->size.height = toplevel->min_size.height;
                                }
                                if (new_screen_location->size.width < toplevel->min_size.width) {
                                        new_screen_location->size.width = toplevel->min_size.width;
                                }
                                break;
                        case(ei_axis_x):
                                new_screen_location->size.height = toplevel->widget.screen_location.size.height;

                                if (new_screen_location->size.width < toplevel->min_size.width) {
                                        new_screen_location->size.width = toplevel->min_size.width;
                                }
                                break;
                        case(ei_axis_y):
                                if (new_screen_location->size.height < toplevel->min_size.height) {
                                        new_screen_location->size.height = toplevel->min_size.height;
                                }

                                new_screen_location->size.width = toplevel->widget.screen_location.size.width;
                                break;
                        default:
                                break;
                }
                widget->content_rect->size.width = new_screen_location->size.width;
                widget->content_rect->size.height = new_screen_location->size.height;
        }
        if(strcmp(widget->wclass->name,"entry") == 0){
                if (new_screen_location->top_left.x + new_screen_location->size.width > widget->parent->screen_location.size.width) {
                        new_screen_location->size.width = widget->parent->screen_location.size.width
                                - (new_screen_location->top_left.x - widget->parent->screen_location.top_left.x + 10);
                }

        }
        widget->screen_location = *new_screen_location;
        widget->wclass->geomnotifyfunc(widget);

        ei_widget_t child = ei_widget_get_first_child(widget);
        while (child != NULL) {
                ei_place(child,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
                child = ei_widget_get_next_sibling(child);
        }
}

void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager){
        if (geo_mgr_list == NULL) {
                geo_mgr_list = malloc(sizeof(ei_geometrymanager_t*));
                if (geo_mgr_list == NULL) {
                        return;
                }
                geo_mgr_list[0] = geometrymanager;
                geo_mgr_list_size = 1;
        } else {
                if (ei_geometrymanager_from_name(geometrymanager->name) == NULL) {
                        ei_geometrymanager_t** new_geo_mgr_list = realloc(geo_mgr_list, (geo_mgr_list_size + 1) * sizeof(ei_geometrymanager_t*));
                        if (new_geo_mgr_list != NULL) {
                                geo_mgr_list = new_geo_mgr_list;
                                geo_mgr_list[geo_mgr_list_size] = geometrymanager;
                                geo_mgr_list_size++;
                        }
                }
        }
}

ei_geometrymanager_t* ei_geometrymanager_from_name (ei_geometrymanager_name_t name){
        if (geo_mgr_list != NULL) {
                for (size_t i = 0; i < geo_mgr_list_size; ++i) {
                        if (strcmp(geo_mgr_list[i]->name, name) == 0) {
                                return geo_mgr_list[i];
                        }
                }
        }
        return NULL;
}

ei_geometrymanager_t* ei_widget_get_geom_manager (ei_widget_t widget){
        return widget->geom_params->manager;
}

void ei_widget_set_geom_manager	(ei_widget_t widget, ei_geometrymanager_t* manager){
        widget->geom_params->manager = manager;
}

ei_geom_param_t ei_widget_get_geom_params (ei_widget_t widget){
        return widget->geom_params;
}

void ei_widget_set_geom_params (ei_widget_t widget, ei_geom_param_t geom_param){
        widget->geom_params = geom_param;
}

/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalidated (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void			ei_geometrymanager_unmap	(ei_widget_t widget){
        if (widget == NULL || widget->geom_params->manager == NULL) {
                return;
        }

        ei_geometrymanager_t* geom_mng = ei_widget_get_geom_manager(widget);
        if (geom_mng->releasefunc != NULL) {
                geom_mng->releasefunc(widget);
        }

        free(widget->geom_params->manager);
        widget->geom_params->manager = NULL;
        free(widget->geom_params);
        widget->geom_params = NULL;

        widget->screen_location.top_left.x = 0;
        widget->screen_location.top_left.y = 0;
}