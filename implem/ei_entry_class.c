#include "ei_entry_class.h"

ei_widget_t entry_allocfunc (){
        ei_widget_t widget = malloc(sizeof(struct entry_t ));
        return widget;
}

void entry_releasefunc (ei_widget_t widget){
        //No implementation for entry
}

/**
 * @brief Removes the first character from a given string.
 *
 * @param str The input string.
 * @return char* A pointer to the modified string.
 */
char* remove_first_char(char* str) {
        if (str == NULL) {
                return NULL;
        }
        size_t str_len = strlen(str);

        // -1 for the removed character, +1 for the null terminator
        char* new_str = malloc((str_len) * sizeof(char));
        if (new_str == NULL) {
                return NULL;
        }

        strcpy(new_str, str + 1);
        return new_str;
}

/**
 * @brief Removes the last character from a given string.
 *
 * @param str The input string.
 * @return char* A pointer to the modified string.
 */
char* remove_last_char(char* str) {
        if (str == NULL) {
                return NULL;
        }
        size_t str_len = strlen(str);

        // -1 for the removed character, +1 for the null terminator
        char* new_str = malloc((str_len) * sizeof(char));
        if (new_str == NULL) {
                return NULL;
        }

        strncpy(new_str, str, str_len - 1);
        new_str[str_len - 1] = '\0';

        return new_str;
}

/**
 * @brief Truncates the text of an entry widget to fit within a specified width.
 *
 * @param text The input text.
 * @param font The font used for text rendering.
 * @param max_width The maximum width to fit.
 * @return char* The truncated text.
 */
char* truncate_text_to_fit_width(char* text, void* font, int max_width) {
        int width = 0, height = 0;
        hw_text_compute_size(text, font, &width, &height);

        char* cursor_pos_ptr = strchr(text, '|') ;

        if (width > max_width) {
                int len = strlen(text);
                while (len > 0 && width > max_width) {
                        text = remove_first_char(text);
                        hw_text_compute_size(text, font, &width, &height);
                }
        }

        char* cursor_new = strchr(text, '|');
        if(cursor_new == NULL){
                hw_text_compute_size(cursor_pos_ptr, font, &width, &height);
                if (width > max_width) {
                        int len = strlen(cursor_pos_ptr);
                        while (len > 0 && width > max_width) {
                                cursor_pos_ptr = remove_last_char(cursor_pos_ptr);
                                hw_text_compute_size(cursor_pos_ptr, font, &width, &height);
                        }
                }
                if(cursor_pos_ptr !=NULL)text = cursor_pos_ptr;
        }

        return text;
}

/**
 * @brief Draws the entry widget on the given surface.
 *
 * @param entry The entry widget to draw.
 * @param surface The surface to draw on.
 * @param pick_surface The pick surface.
 * @param clipper The clipping rectangle.
 */
void draw_entry (entry_t * entry,
                 ei_surface_t		surface,
                 ei_surface_t		pick_surface,
                 ei_rect_t*		clipper){
        assertion_pick_color((ei_widget_t) entry, surface);

        if (entry->widget.wclass->drawfunc != NULL) {
                if(entry->in_focus && entry->widget.parent->parent->children_tail == entry->widget.parent) {
                        ei_color_t entry_color = (ei_color_t){255,255,255, 180};
                        ei_fill(surface, &entry_color, clipper);
                }else {
                        ei_fill(surface, entry->widget.color, clipper);
                }
                if (pick_surface != NULL) {
                        ei_fill(pick_surface, entry->widget.pick_color, clipper);
                }
                if ((entry->text) != NULL){
                        char * text = entry->text;
                        int width = 0;
                        int height = 0;
                        hw_text_compute_size(entry->text, entry->text_font, &width, &height);
                        if(width > entry->widget.screen_location.size.width){
                                text = truncate_text_to_fit_width(entry->text, entry->text_font, entry->widget.screen_location.size.width);
                        }
                        draw_text(text, entry->text_font, entry->text_color, &(entry->widget.screen_location),
                                   surface, clipper, ei_anc_northwest);
                }

        }
}

void entry_drawfunc (ei_widget_t		widget,
                     ei_surface_t		surface,
                     ei_surface_t		pick_surface,
                     ei_rect_t*		clipper){
        if (widget != root) {
                if (ei_widget_is_displayed(widget)) {
                        draw_entry((entry_t *) widget, surface, pick_surface, clipper);
                }
        } else {
                draw_entry((entry_t *) widget, surface, pick_surface, clipper);
        }
}

void entry_setdefaultsfunc(ei_widget_t		widget){
        entry_t * entry = (entry_t *) widget;
        entry->widget = *widget;
        entry->text = "\0";
        entry->text_font = ei_default_font;
        entry->text_color = ei_font_default_color;
        entry->border_width = 2;
        entry->requested_char_size = 10;
        entry->widget.color->red = ei_default_background_color.red;
        entry->widget.color->green = ei_default_background_color.green;
        entry->widget.color->blue = ei_default_background_color.blue;
        entry->widget.color->alpha = ei_default_background_color.alpha;
        int width = 0;
        int height = 0;
        hw_text_compute_size("aaaaaaaaaa",ei_default_font,&width,&height);
        entry->widget.requested_size.width = width;
        entry->widget.requested_size.height = height;
        entry->widget.screen_location.size.width = width;
        entry->widget.screen_location.size.height = height;
        entry->widget.screen_location.top_left.x = 0;
        entry->widget.screen_location.top_left.y = 0;
        entry->in_focus = false;

}

void entry_geomnotifyfunc(ei_widget_t		widget){
        //No implementation for entry.
}

ei_widgetclass_t* create_entry_class(){
        ei_widgetclass_t* entry = malloc(sizeof(ei_widgetclass_t));
        strcpy(entry->name, "entry");
        entry->allocfunc =  &entry_allocfunc;
        entry->releasefunc = &entry_releasefunc;
        entry->drawfunc = &entry_drawfunc;
        entry->setdefaultsfunc = &entry_setdefaultsfunc;
        entry->geomnotifyfunc = &entry_geomnotifyfunc;

        return entry;
}