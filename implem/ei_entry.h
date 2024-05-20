#include "ei_entry_class.h"
#include "ei_placer.h"
#include "ei_toplevel.h"

char* get_text_without_cursor(char* str);

char* get_text_with_char_concatenated(char* str, char ch);

char* insert_char_before_pipe(char* str, char ch, int max);

char* remove_character_before_pipe(char* str, bool delete_before);

char* move_pipe_in_text(char* str, bool dir);

bool callback_type_in_focus (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_remove_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);

bool callback_buttondown_focus_entry (ei_widget_t widget, ei_event_t* event, ei_user_param_t user_param);