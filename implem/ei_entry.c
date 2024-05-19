#include "ei_entry.h"
#include "ei_entry_class.h"
#include "ei_placer.h"
#include "ei_toplevel.h"

void			ei_entry_configure		(ei_widget_t		widget,
                                                               int*			requested_char_size,
                                                               const ei_color_t*	color,
                                                               int*			border_width,
                                                               ei_font_t*		text_font,
                                                               ei_color_t*		text_color){
        entry_t * entry = (entry_t*) widget;
        entry->widget = *widget;

        if (requested_char_size != NULL){
                entry->requested_char_size = *requested_char_size;
                // Create an array with space for characters plus the null terminator
                char string[entry->requested_char_size+1];
                // Use memset to fill the array with 'a' characters
                memset(string, 'a', entry->requested_char_size);

                // Manually add the null terminator at the end
                string[entry->requested_char_size] = '\0';
                int width = 0;
                int height = 0;
                hw_text_compute_size(string,ei_default_font,&width,&height);
                entry->widget.parent->content_rect->size.height = 5*height;

                entry->widget.parent->screen_location.size.height = 5*height;

                if(((toplevel_t*)entry->widget.parent)->resizable != ei_axis_none){
                        ei_place_xy(entry->widget.parent->children_head->next_sibling,0,0);

                }

        }

        if (color != NULL) {
                entry->widget.color->alpha = color->alpha;
                entry->widget.color->red = color->red;
                entry->widget.color->green = color->green;
                entry->widget.color->blue = color->blue;
        }


        if (border_width != NULL){
                entry->border_width = *border_width;
        }

        if (text_font != NULL){
                entry->text_font = *text_font;
        }

        if (text_color != NULL){
                entry->text_color = *text_color;
        }
}

void			ei_entry_set_text		(ei_widget_t		widget,
                                                              ei_const_string_t 	text) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                if(strlen(text) <= entry->requested_char_size) {
                        entry->text = (char*)text;
                }else {
                        strncpy(entry->text,(char*)text,entry->requested_char_size);
                }
        }
}

ei_const_string_t 	ei_entry_get_text		(ei_widget_t		widget) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                return entry->text;
        }
        return NULL;
}


void			ei_entry_give_focus		(ei_widget_t		widget) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                entry->in_focus = true;
        }
}

