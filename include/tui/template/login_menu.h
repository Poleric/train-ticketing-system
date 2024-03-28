#ifndef TRAINTICKETINGSYSTEM_LOGIN_MENU_H
#define TRAINTICKETINGSYSTEM_LOGIN_MENU_H

#include <tui/tui_common.h>
#include <tui/tui_utils.h>

#define LOGIN_USERNAME_LABEL "Username :"
#define LOGIN_PASSWORD_LABEL "Password :"
#define LOGIN_LABEL_FIELD_LENGTH 10
#define LOGIN_FIELD_GAP 1

typedef enum LoginFormAction {
    NO_ACTION = 0,
    EXIT_FORM_ACTION = 1,
    SWITCH_MENU_ACTION = 2,
    LOGIN_ACTION = 3
} login_form_action_t;

static FIELD** init_login_fields (int input_field_length);
FORM* init_login_form(WINDOW* form_window, const char* header_message, bool box, int margin_from_sides);
static void print_login_form_header(WINDOW* window, const char* header_message, bool box);
static void print_login_field_labels(WINDOW* form_fields_window);
void print_message_below_form(FORM* login_form, const char* message, int offset_y, int offset_x, bool center);
login_form_action_t login_form_driver(FORM* login_form, int ch, char** username_buffer, char** password_buffer);

void cleanup_login_form(FORM* login_form);
static void free_login_fields(FIELD** login_fields);

#endif //TRAINTICKETINGSYSTEM_LOGIN_MENU_H
