#ifndef TRAINTICKETINGSYSTEM_REGISTER_FORM_H
#define TRAINTICKETINGSYSTEM_REGISTER_FORM_H

#include <tui/form/form.h>

typedef struct RegisterForm register_form_t;

struct RegisterForm {
    form_t form;

    const char* header;
    int header_y;

    const char* form_header;
    int form_header_y;

    int form_message_y;

    int footer_y;
};

void init_register_form(register_form_t* register_form, WINDOW* form_window, const char* header, const char* form_header);

void print_register_fields(register_form_t* register_form);
void print_register_form_header(register_form_t* register_form, short color_pair);
void print_register_form_form_header(register_form_t* register_form, short color_pair);
void print_register_form_footer(register_form_t* register_form, short color_pair);
void print_register_form_message(register_form_t* register_form, const char* message, short color_pair);

void display_register_form(register_form_t* register_form, short color_pair);

void free_register_form(register_form_t* register_form);

char* get_register_username(register_form_t* register_form);
char* get_register_email(register_form_t* register_form);
char* get_register_password(register_form_t* register_form);
char* get_register_confirm_password(register_form_t* register_form);
char get_register_gender(register_form_t* register_form);
char* get_register_contact_no(register_form_t* register_form);

#endif //TRAINTICKETINGSYSTEM_REGISTER_FORM_H
