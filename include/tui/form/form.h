#ifndef TRAINTICKETINGSYSTEM_FORM_H
#define TRAINTICKETINGSYSTEM_FORM_H

#include <tui/utils/tui_utils.h>
#include <stdlib.h>

typedef struct Form {
    WINDOW* window;
    int LOGIN_FORM_LINES, LOGIN_FORM_COLS;
    int selection_row, selection_col;
    size_t n_buffer;
    size_t buffer_length;
    int field_start_x;
    char** field_buffers;
} FORM;

typedef enum FormAction {
    NO_ACTION = 0,
    EXIT_FORM_ACTION = 1,
    SWITCH_MENU_ACTION = 2,
    SUBMIT_ACTION = 3,
    RELOAD_ACTION = 4,
    REGISTER_ACTION = 5
} form_action_t;

FORM* init_form(WINDOW* form_window, int n_buffer, int label_field_length);
void move_cursor_to_input_field(FORM* form, int field_n, int pos);
int move_cursor_to_end_of_input_field(FORM* form, int field_n);
void print_input_field_buffer(FORM* form, int field_n);
form_action_t form_driver(FORM* form, int ch);
void free_form(FORM* form);
void clear_field_buffers(FORM* login_form, int field_index);
void reset_form(FORM* form);
void cleanup_form(FORM* form);


#endif //TRAINTICKETINGSYSTEM_FORM_H
