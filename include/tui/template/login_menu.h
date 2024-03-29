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
    SUBMIT_ACTION = 3
} login_form_action_t;

typedef struct LoginForm {
    WINDOW* window;

    size_t n_buffer;
    size_t buffer_length;
    char** field_buffers;
} LOGIN_FORM;

LOGIN_FORM* init_login_form(WINDOW* form_window);
void print_login_fields(LOGIN_FORM* login_form, bool underline);
void move_cursor_to_input_field(LOGIN_FORM* login_form, int field_n, int pos);
int move_cursor_to_end_of_input_field(LOGIN_FORM* login_form, int field_n);
void print_input_field_buffer(LOGIN_FORM* login_form, int field_n);
void delete_char(char* buffer, int pos);
void add_char(char* buffer, char ch, int pos);
login_form_action_t form_driver(LOGIN_FORM* login_form, int ch);
void free_login_form(LOGIN_FORM* login_form);
void cleanup_login_form(LOGIN_FORM* login_form);


LOGIN_FORM* create_login_form(WINDOW* form_window);

#endif //TRAINTICKETINGSYSTEM_LOGIN_MENU_H
