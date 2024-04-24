#ifndef TRAINTICKETINGSYSTEM_LOGIN_FORM_H
#define TRAINTICKETINGSYSTEM_LOGIN_FORM_H

#include <tui/template/form.h>

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

FORM* create_login_form(WINDOW* form_window);
void print_login_fields(FORM* login_form, bool underline);
void clear_login_field(FORM* login_form, int field_index);
void print_form(FORM* login_form);
char* get_username(FORM* login_form);
char* get_password(FORM* login_form);

#endif //TRAINTICKETINGSYSTEM_LOGIN_FORM_H
