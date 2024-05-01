#ifndef TRAINTICKETINGSYSTEM_FORM_H
#define TRAINTICKETINGSYSTEM_FORM_H

#include <tui/utils/tui_utils.h>
#include <stdlib.h>

typedef struct Form form_t;
typedef struct FormField form_field_t;
typedef enum FormAction form_action_t;

struct FormField {
    int y, x;  // entire field start y, x
    const char* label;

    int offset_x;  // buffer start x
    char* buffer;
    int pos;
};

struct Form {
    int width, height;

    size_t number_of_fields;
    size_t buffer_length;  // all field's buffer have same length
    form_field_t* fields;

    int field_label_width;  // all form label have same width

    int selection_row;

    WINDOW* window;
};

enum FormAction {
    NO_ACTION,
    EXIT_FORM_ACTION,
    SWITCH_MENU_ACTION,
    SUBMIT_ACTION,
    RELOAD_ACTION,
    REGISTER_ACTION,
    FORGOT_PASSWORD_ACTION
};

void init_form(form_t* form, WINDOW* form_window, int n_buffer, int buffer_length);

void move_cursor_to_input_field(form_t* form);
void move_cursor_to_start_of_input_field(form_t* form);
void move_cursor_to_end_of_input_field(form_t* form);
void print_current_field_buffer(form_t* form);
void print_current_field_label(form_t* form);
void clear_field_buffers(form_t* form, int n_field);

void backspace_action_field(form_t* form);
void delete_action_field(form_t* form);
void add_action_field(form_t* form, int ch);

form_action_t form_driver(form_t* form, int ch);

void free_form(form_t* form);
void reset_form(form_t* form);
void cleanup_form(form_t* form);


#endif //TRAINTICKETINGSYSTEM_FORM_H
