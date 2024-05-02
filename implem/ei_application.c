#include "ei_application.h"
#include "ei_implementation.h"
#include "ei_parser.h"

typedef struct {
        ei_widget_t widget;
} frame_t;

///< The function that allocated instances of this class of widget.
/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a widget of a class: both the common attributes, that use
 *		\ref ei_widget_struct_size bytes, and the specific attributes of the class.
 *		After allocation, the function *must* initialize the memory to 0.
 *
 * @return			A block of memory big enough to represent a widget of this class,
 * 				with all bytes set to 0.
 */
ei_widget_t frame_allocfunc (){
        // Allocate the memory
        // Initialize the memory to 0 (already done by calloc)
        ei_widget_t* widget = (ei_widget_t*) calloc(1, (unsigned long) sizeof(frame_t));

        return widget;
}

///< The function that releases all the resources used by an instance of this class of widget.
/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The memory used for the \ref ei_widget_t structure itself must *not* be freed by
 *		the function. It frees the *fields* of the structure, if necessary.
 *		Can be set to NULL in \ref ei_widgetclass_t if no memory is used by a class of widget.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void frame_releasefunc (ei_widget_t	widget){

}

///< The function that draws on screen an instance of this class of widget.
/**
 * \brief	A function that draws a widget of a class.
 * 		The function must also draw the children of the widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		A locked surface where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	pick_surface	The picking offscreen.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void frame_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        // Cast widget to the appropriate type if necessary
        frame_t* frame = (frame_t*)widget;

        ei_widget_t child = frame->widget->children_head;
        while (child != NULL) {
                // Call the draw function for each child widget
                if (child->wclass->drawfunc != NULL) {
                        child->wclass->drawfunc(child, surface, pick_surface, clipper);
                }
                // Move to the next child
                child = child->next_sibling;
        }
}

///< The function that sets the default values to all the parameters of an instance of this class of widget.
/**
 * \brief	A function that sets the default values for a widget of this class.
 *
 * @param	widget		A pointer to the widget instance to initialize.
 */
void frame_setdefaultsfunc(ei_widget_t		widget){
}

ei_surface_t main_surface = NULL;

void ei_app_create(ei_size_t main_window_size, bool fullscreen){
        hw_init();
        main_surface  = hw_create_window(main_window_size, false);

        ei_widgetclass_t frame;
        frame.allocfunc =  &frame_allocfunc;
        frame.releasefunc = &frame_releasefunc;
        frame.drawfunc = &frame_drawfunc;
        frame.setdefaultsfunc = &frame_setdefaultsfunc;
        ei_widgetclass_register(&frame);
        ei_widget_t frame_w = ei_widget_create("frame", &frame, NULL, NULL);

}

void ei_app_free(void){
        hw_quit();
}

void ei_app_run(void){
        // Get the root widget of the application
        ei_widget_t root_widget = ei_app_root_widget();

        // Get the clipper rectangle
        ei_rect_t clipper = hw_surface_get_rect(main_surface);

        hw_surface_lock(main_surface);

        // Call the draw function for the root widget to draw the entire widget hierarchy
        if (root_widget->wclass->drawfunc != NULL) {
                root_widget->wclass->drawfunc(root_widget, main_surface, NULL, &clipper);
        }

        hw_surface_unlock(main_surface);

        // Update the screen
        hw_surface_update_rects(main_surface, NULL);

        // Wait for user input
        getchar();

}

void ei_app_invalidate_rect(const ei_rect_t* rect){

}

void ei_app_quit_request(void){

}

/**
 * \brief	Returns the "root widget" of the application: a "frame" widget that span the entire
 *		root window.
 *
 * @return 			The root widget.
 */
ei_widget_t ei_app_root_widget(void){

}

/**
 * \brief	Returns the surface of the root window. Can be used to create surfaces with similar
 * 		r, g, b channels.
 *
 * @return 			The surface of the root window.
 */
ei_surface_t ei_app_root_surface(void){

}
