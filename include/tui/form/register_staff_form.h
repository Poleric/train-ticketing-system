#ifndef TRAINTICKETINGSYSTEM_REGISTER_STAFF_FORM_H
#define TRAINTICKETINGSYSTEM_REGISTER_STAFF_FORM_H

#include <tui/form/form.h>

typedef struct StaffRegisterForm staff_register_form_t;

struct StaffRegisterForm {
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

void init_staff_register_form(staff_register_form_t* register_form, WINDOW* form_window, const char* header, const char* form_header);

void scale_staff_register_form_to_window(staff_register_form_t* register_form);

void print_staff_register_fields(staff_register_form_t* register_form);
void print_staff_register_form_header(staff_register_form_t* register_form, short color_pair);
void print_staff_register_form_form_header(staff_register_form_t* register_form, short color_pair);
void print_staff_register_form_footer(staff_register_form_t* register_form, short color_pair);
void print_staff_register_form_message(staff_register_form_t* register_form, const char* message, short color_pair);

void display_staff_register_form(staff_register_form_t* register_form, short color_pair);

void free_staff_register_form(staff_register_form_t* register_form);

char* get_staff_register_username(staff_register_form_t* register_form);
char* get_staff_register_email(staff_register_form_t* register_form);
char* get_staff_register_password(staff_register_form_t* register_form);
char* get_staff_register_confirm_password(staff_register_form_t* register_form);
double get_staff_register_salary(staff_register_form_t* register_form);
char* get_staff_register_contact_no(staff_register_form_t* register_form);
char* get_staff_register_position(staff_register_form_t* register_form);

bool validate_staff_register_email(staff_register_form_t* register_form);
bool validate_staff_register_same_password(staff_register_form_t* register_form);
bool validate_staff_register_salary(staff_register_form_t* register_form);
        bool validate_staff_register_contact_no(staff_register_form_t* register_form);
#endif //TRAINTICKETINGSYSTEM_REGISTER_STAFF_FORM_H
