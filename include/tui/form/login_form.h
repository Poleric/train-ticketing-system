#ifndef TRAINTICKETINGSYSTEM_LOGIN_FORM_H
#define TRAINTICKETINGSYSTEM_LOGIN_FORM_H

#include <tui/form/form.h>

typedef struct LoginForm login_form_t;

struct LoginForm {
    form_t form;

    const char* header;

    const char **footers;
    int* footer_widths;
    int number_of_footers;
};

void init_login_form(login_form_t* login_form, WINDOW* form_window, const char* header);

void scale_login_form_to_window(login_form_t* login_form);

void print_login_fields(login_form_t* login_form);
void print_login_form_header(login_form_t* login_form, short color_pair);
void print_login_form_footer(login_form_t* login_form, short color_pair);

void display_login_form(login_form_t* login_form);

void free_login_form(login_form_t* login_form);

char* get_username(login_form_t* login_form);
char* get_password(login_form_t* login_form);

#endif //TRAINTICKETINGSYSTEM_LOGIN_FORM_H
