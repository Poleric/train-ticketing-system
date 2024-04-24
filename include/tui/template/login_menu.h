#ifndef TRAINTICKETINGSYSTEM_LOGIN_MENU_H
#define TRAINTICKETINGSYSTEM_LOGIN_MENU_H

#include <tui/tui_common.h>
#include <tui/tui_utils.h>
#include <string.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define MAX(a,b) __max(a,b)
#else
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#endif

#define LOGIN_USERNAME_LABEL "Username :"
#define LOGIN_PASSWORD_LABEL "Password :"
#define LOGIN_LABEL_FIELD_LENGTH MAX(strlen(LOGIN_USERNAME_LABEL), strlen(LOGIN_PASSWORD_LABEL))
#define LOGIN_FIELD_GAP 1

typedef enum LoginFormAction {
    NO_ACTION = 0,
    EXIT_FORM_ACTION = 1,
    SWITCH_MENU_ACTION = 2,
    SUBMIT_ACTION = 3,
    RELOAD_ACTION = 4,
    REGISTER_ACTION = 5
} login_form_action_t;

typedef struct Form {
    WINDOW* window;
    int LOGIN_FORM_LINES, LOGIN_FORM_COLS;
    int selection_row, selection_col;
    size_t n_buffer;
    size_t buffer_length;
    char** field_buffers;
} FORM;

FORM* init_form(WINDOW* form_window, int n_buffer);
void move_cursor_to_input_field(FORM* form, int field_n, int pos);
int move_cursor_to_end_of_input_field(FORM* form, int field_n);
void print_input_field_buffer(FORM* form, int field_n);
login_form_action_t form_driver(FORM* form, int ch);
void free_login_form(FORM* form);
void reset_login_form(FORM* form);
void cleanup_login_form(FORM* login_form);

// helpers

FORM* create_login_form(WINDOW* form_window);
void print_login_fields(FORM* login_form, bool underline);
void clear_login_field(FORM* login_form, int field_index);
void print_form(FORM* login_form);
char* get_username(FORM* login_form);
char* get_password(FORM* login_form);

#endif //TRAINTICKETINGSYSTEM_LOGIN_MENU_H
