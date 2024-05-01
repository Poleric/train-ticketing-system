#ifndef TRAINTICKETINGSYSTEM_LOGIN_FORM_H
#define TRAINTICKETINGSYSTEM_LOGIN_FORM_H

#include <tui/form/form.h>
#include <panel.h>
#include <string.h>
#include <utils.h>

#define LOGIN_USERNAME_LABEL "Username :"
#define LOGIN_PASSWORD_LABEL "Password :"
#define LOGIN_LABEL_FIELD_LENGTH MAX(strlen(LOGIN_USERNAME_LABEL), strlen(LOGIN_PASSWORD_LABEL))
#define LOGIN_FIELD_GAP 1

FORM* create_login_form(WINDOW* form_window);
void print_login_fields(FORM* login_form, bool underline);
void print_login_form(FORM* login_form);
char* get_username(FORM* login_form);
char* get_password(FORM* login_form);

#endif //TRAINTICKETINGSYSTEM_LOGIN_FORM_H
