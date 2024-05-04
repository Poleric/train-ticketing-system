#ifndef TRAINTICKETINGSYSTEM_FORGOT_PASSWORD_FORM_H
#define TRAINTICKETINGSYSTEM_FORGOT_PASSWORD_FORM_H

#include <tui/form/form.h>

typedef struct ForgotPasswordForm forgot_password_form_t;

struct ForgotPasswordForm {
    form_t form;

    const char* header;
    int header_y;

    const char* form_header;
    int form_header_y;

    int form_message_y;

    const char **footers;
    int* footer_widths;
    int number_of_footers;
    int footer_y;
};

void init_forgot_password_form(forgot_password_form_t* forgot_password_form, WINDOW* form_window, const char* header, const char* form_header);
void init_reset_password_fields(forgot_password_form_t* forgot_password_form);

void scale_forgot_password_form_to_window(forgot_password_form_t* forgot_password_form);

void print_forgot_password_form_fields(forgot_password_form_t* forgot_password_form);
void print_forgot_password_form_header(forgot_password_form_t* forgot_password, short color_pair);
void print_forgot_password_form_form_header(forgot_password_form_t* forgot_password_form, short color_pair);
void print_forgot_password_form_footer(forgot_password_form_t* forgot_password_form, short color_pair);
void print_forgot_password_form_message(forgot_password_form_t* forgot_password_form, const char* message, short color_pair);

void display_forgot_password_form(forgot_password_form_t* forgot_password_form, short color_pair);

form_action_t forgot_password_form_driver(forgot_password_form_t* forgot_password_form, int ch);

void free_forgot_password_form(forgot_password_form_t* forgot_password_form);

char* get_recovery_email(forgot_password_form_t* forgot_password_form);
int get_recovery_code(forgot_password_form_t* forgot_password_form);

char* get_recovery_password(forgot_password_form_t* forgot_password_form);
char* get_recovery_confirm_password(forgot_password_form_t* forgot_password_form);
bool validate_new_password_same(forgot_password_form_t* forgot_password_form);

#endif //TRAINTICKETINGSYSTEM_FORGOT_PASSWORD_FORM_H
