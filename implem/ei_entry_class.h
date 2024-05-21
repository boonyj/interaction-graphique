#ifndef PROJETC_IG_EI_ENTRY_CLASS_H
#define PROJETC_IG_EI_ENTRY_CLASS_H

#include "ei_implementation.h"
#include "ei_draw_tool.h"
#include "ei_global.h"

/**
 * @brief Structure representing an entry widget.
 *
 * Contains properties for text input, including character size, border width,
 * text color, font, entered text, and focus status.
 */
typedef struct entry_t {
        ei_impl_widget_t widget; ///< Base widget properties.
        int requested_char_size; ///< Requested character size.
        int border_width;        ///< Border width.
        ei_color_t text_color;   ///< Text color.
        ei_font_t text_font;     ///< Text font.
        char* text;              ///< Entered text.
        bool in_focus;           ///< Focus status.
} entry_t;

/**
 * @brief Allocates memory for a new entry widget.
 *
 * @return ei_widget_t A pointer to the allocated entry widget.
 */
ei_widget_t entry_allocfunc ();

/**
 * @brief Releases the memory allocated for an entry widget.
 *
 * @param widget The entry widget to be released.
 */
void entry_releasefunc (ei_widget_t	widget);

/**
 * @brief Draws the entry widget on the given surface.
 *
 * @param widget The widget to draw.
 * @param surface The surface to draw on.
 * @param pick_surface The pick surface.
 * @param clipper The clipping rectangle.
 */
void entry_drawfunc (ei_widget_t       widget,
                     ei_surface_t		    surface,
                     ei_surface_t		    pick_surface,
                     ei_rect_t*		      clipper);

/**
 * @brief Sets default values for the entry widget.
 *
 * @param widget The entry widget.
 */
void entry_setdefaultsfunc(ei_widget_t		widget);

/**
 * @brief Notification function called when the geometry of the entry widget changes.
 *
 * @param widget The entry widget.
 */
void entry_geomnotifyfunc(ei_widget_t		widget);

/**
 * @brief Creates and initializes the entry widget class.
 *
 * This function allocates memory for the entry widget class, sets its name, and assigns
 * function pointers to the functions responsible for allocating, releasing, drawing,
 * setting defaults, and handling geometry notification for the entry widget.
 *
 * @return A pointer to the newly created entry widget class.
 */
ei_widgetclass_t* create_entry_class();

#endif //PROJETC_IG_EI_ENTRY_CLASS_H
