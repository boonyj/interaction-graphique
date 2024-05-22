#include "ei_entry.h"
#include <ei_widget_attributes.h>
#include "ei_global.h"

void ei_entry_set_text (ei_widget_t             widget,
                        ei_const_string_t       text) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                if(strlen(text) <= entry->requested_char_size+1) {
                        entry->text = (char*)text;
                }else {
                        strncpy(entry->text,(char*)text,entry->requested_char_size);
                }
        }
}

ei_const_string_t ei_entry_get_text (ei_widget_t widget) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                return entry->text;
        }
        return NULL;
}

void ei_entry_give_focus (ei_widget_t widget) {
        if((strcmp(widget->wclass->name,"entry")) == 0){
                entry_t* entry = (entry_t*)widget;
                entry->in_focus = true;
        }
}

char* get_text_without_cursor(char* str) {
        char* res = malloc((strlen(str)) * sizeof(char));
        if (res == NULL) {
                return NULL;
        }
        int j = 0;
        for (int i = 0; str[i] != '\0'; ++i) {
                if (str[i] != '|') {
                        res[j++] = str[i];
                }
        }
        res[j] = '\0';

        return res;
}

char* get_text_with_char_concatenated(char* str, char ch) {
        if (str == NULL) {
                return NULL;
        }

        size_t str_len = strlen(str);
        size_t new_str_len = str_len + 1;

        char* new_str = malloc((new_str_len + 1) * sizeof(char)); // +1 for the null terminator
        if (new_str == NULL) {
                return NULL;
        }

        strcpy(new_str, str);
        new_str[str_len] = ch;
        new_str[new_str_len] = '\0';

        return new_str;
}

char* insert_char_before_pipe(char* str, char ch, int max) {
        if (str == NULL) {
                return NULL;
        }

        char* pipe_pos = strchr(str, '|');
        if (pipe_pos == NULL) {
                return strdup(str);
        }

        size_t str_len = strlen(str);
        size_t prefix_len = pipe_pos - str;

        if (str_len > max) {
                return str;
        }

        char* new_str = malloc((str_len + 2) * sizeof(char)); // +2 for the new character and null terminator
        if (new_str == NULL) {
                return NULL;
        }

        strncpy(new_str, str, prefix_len);
        new_str[prefix_len] = ch;
        strcpy(new_str + prefix_len + 1, pipe_pos); // Copy the rest of the string, including the '|'
        new_str[strlen(new_str)] = '\0';
        return new_str;
}

char* remove_character_before_pipe(char* str, bool delete_before) {
        if (str == NULL) {
                return NULL;
        }

        char* pipe_pos = strchr(str, '|');
        if (pipe_pos == NULL) {
                return strdup(str);
        }
        size_t index = pipe_pos - str;

        size_t delete_index;
        if (delete_before) {
                if(*str == '|') {
                        return str;
                }
                delete_index = (index > 0) ? index - 1 : index;
        } else {
                delete_index = index+1;
        }

        if(strlen(str) == 1) {
                return str;
        }

        size_t str_len = strlen(str);
        size_t new_str_len = str_len + 1; // Length of the new string (excluding the removed character)

        char* new_str = malloc((new_str_len) * sizeof(char)); // +1 for the null terminator
        if (new_str == NULL) {
                return NULL;
        }

        if(delete_before) {
                strncpy(new_str, str, delete_index);
                strcpy(new_str + delete_index, str + delete_index+1);
        }else{
                strncpy(new_str, str, delete_index);
                if(delete_index != strlen(str)) {
                        strcpy(new_str + delete_index, str + delete_index + 1);
                }else {
                        return str;
                }
        }

        new_str[strlen(new_str)] = '\0';
        return new_str;
}

char* move_pipe_in_text(char* str, bool dir) {
        if (str == NULL) {
                return NULL;
        }

        char* pipe_pos = strchr(str, '|');
        if (pipe_pos == NULL) {
                return strdup(str);
        }

        size_t index = pipe_pos - str;
        size_t str_len = strlen(str);

        size_t new_index;
        if (dir) {
                new_index = (index < str_len - 1) ? index + 1 : index;
        } else {
                new_index = (index > 0) ? index - 1 : index;
        }

        if (new_index == index) {
                return strdup(str);
        }

        char* new_str = malloc((str_len + 1) * sizeof(char));
        if (new_str == NULL) {
                return NULL;
        }

        strcpy(new_str, str);
        new_str[index] = new_str[new_index];
        new_str[new_index] = '|';
        new_str[strlen(new_str)] = '\0';

        return new_str;
}

bool callback_type_in_focus (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_keydown) {
                entry_t* entry = (entry_t*) user_param;
                char input = event->param.key_code;
                char* res = entry->text;

                if(event->param.key_code >= SDLK_SPACE && event->param.key_code <= SDLK_z) {
                        if(ei_event_has_shift(event)) {
                                input = toupper(input);
                        }
                        res = insert_char_before_pipe(entry->text,input,entry->requested_char_size);
                }else if (event->param.key_code == SDLK_BACKSPACE){
                        res = remove_character_before_pipe(entry->text, true);
                }
                else if (event->param.key_code == SDLK_DELETE){
                        res = remove_character_before_pipe(entry->text, false);
                }
                else if(event->param.key_code == SDLK_RIGHT || event->param.key_code == SDLK_LEFT) {
                        if(event->param.key_code == SDLK_RIGHT) {
                                res = move_pipe_in_text(entry->text, true);
                        }else {
                                res = move_pipe_in_text(entry->text, false);
                        }
                }else if(event->param.key_code == SDLK_TAB) {
                        entry = (entry_t*) user_param;
                        entry->in_focus = false;
                        res = get_text_without_cursor(entry->text);
                        ei_unbind(ei_ev_mouse_buttondown,NULL,"all",callback_buttondown_remove_focus_entry,entry);
                        ei_unbind(ei_ev_keydown,NULL,"all",callback_type_in_focus,entry);

                        ei_widget_t parc = &entry->widget;

                        if(!ei_event_has_shift(event)) {
                                while(parc != NULL) {
                                        parc = ei_widget_get_next_sibling(parc);
                                        if(parc != NULL && strcmp(parc->wclass->name,"entry") == 0) {
                                                break;
                                        }
                                }

                                if(parc == NULL) {
                                        parc = entry->widget.parent->children_head;
                                        while(strcmp(parc->wclass->name,"entry") != 0) {
                                                parc = ei_widget_get_next_sibling(parc);
                                        }
                                }
                        }else {
                                ei_widget_t prev = NULL;
                                ei_widget_t current = entry->widget.parent->children_head;

                                while (current != &entry->widget) {
                                        if (strcmp(current->wclass->name, "entry") == 0) {
                                                prev = current;
                                        }
                                        current = ei_widget_get_next_sibling(current);
                                }

                                if (prev != NULL) {
                                        parc = prev;
                                } else {
                                        parc = entry->widget.parent->children_head;
                                        ei_widget_t last_entry = NULL;

                                        while (parc != NULL ) {
                                                if (strcmp(parc->wclass->name, "entry") == 0) {
                                                        last_entry = parc;
                                                }
                                                parc = ei_widget_get_next_sibling(parc);
                                        }

                                        if (last_entry != NULL) {
                                                parc = last_entry;
                                        }
                                }

                        }
                        entry_t* new_entry = (entry_t*) parc;
                        if (new_entry != NULL) {
                                new_entry->in_focus = true;
                        }
                        ei_entry_set_text(&new_entry->widget, get_text_with_char_concatenated(new_entry->text, '|'));
                        ei_bind(ei_ev_mouse_buttondown,NULL,"all",callback_buttondown_remove_focus_entry,new_entry);
                        ei_bind(ei_ev_keydown,NULL,"all",callback_type_in_focus,new_entry);
                }
                ei_entry_set_text(&(entry->widget),res);

                entry->widget.wclass->drawfunc(&(entry->widget),main_surface,pick_surface,&entry->widget.screen_location);
                return true;
        }
        return false;
}

bool callback_buttondown_remove_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                entry_t* entry = (entry_t*) user_param;
                entry->in_focus = false;
                ei_entry_set_text(&(entry->widget),get_text_without_cursor(entry->text));

                ei_unbind(ei_ev_mouse_buttondown,NULL,"all",callback_buttondown_remove_focus_entry,entry);
                ei_unbind(ei_ev_keydown,NULL,"all",callback_type_in_focus,entry);
                entry->widget.wclass->drawfunc(&(entry->widget),main_surface,pick_surface,&entry->widget.screen_location);
                return true;
        }
        return false;
}

bool callback_buttondown_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param) {
        if (event->type == ei_ev_mouse_buttondown) {
                entry_t* entry = (entry_t*) widget;
                entry->in_focus = true;
                ei_entry_set_text(&entry->widget, get_text_with_char_concatenated(entry->text, '|'));

                ei_bind(ei_ev_mouse_buttondown,NULL,"all",callback_buttondown_remove_focus_entry,entry);
                ei_bind(ei_ev_keydown,NULL,"all",callback_type_in_focus,entry);
                return true;
        }
        return false;
}

void ei_entry_configure (ei_widget_t		widget,
                        int*			requested_char_size,
                        const ei_color_t*	color,
                        int*			border_width,
                        ei_font_t*		text_font,
                        ei_color_t*		text_color){
        entry_t * entry = (entry_t*) widget;
        entry->widget = *widget;

        if (requested_char_size != NULL){
                entry->requested_char_size = *requested_char_size;
                char string[entry->requested_char_size+2];
                memset(string, 'a', entry->requested_char_size);

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

        ei_bind(ei_ev_mouse_buttondown, &entry->widget,NULL, callback_buttondown_focus_entry, NULL);
}