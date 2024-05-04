#ifndef TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H
#define TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H

#include <tui/form/form.h>
#include <member.h>

typedef struct MemberDetailsForm member_details_form_t;

struct MemberDetailsForm {
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

    member_t* member;
};

void init_member_details_form(member_details_form_t* member_details_form, WINDOW* form_window, const char* header, const char* form_header, member_t* member);

void scale_member_details_form_to_window(member_details_form_t* member_details_form);

void print_member_details_form_fields(member_details_form_t* member_details_form);
void print_member_details_form_header(member_details_form_t* member_details_form, short color_pair);
void print_member_details_form_form_header(member_details_form_t* member_details_form, short color_pair);
void print_member_details_form_footer(member_details_form_t* member_details_form, short color_pair);
void print_member_details_form_message(member_details_form_t* member_details_form, const char* message, short color_pair);

void display_member_details_form(member_details_form_t* member_details_form, short color_pair);

void free_member_details_form(member_details_form_t* member_details_form);

char* get_member_details_username(member_details_form_t* member_details_form);
char* get_member_details_email(member_details_form_t* member_details_form);
char* get_member_details_password(member_details_form_t* member_details_form);
char* get_member_details_confirm_password(member_details_form_t* member_details_form);
char get_member_details_gender(member_details_form_t* member_details_form);
char* get_member_details_contact_no(member_details_form_t* member_details_form);

bool validate_member_details_email(member_details_form_t* member_details_form);
bool validate_member_details_same_password(member_details_form_t* member_details_form);
bool validate_member_details_gender(member_details_form_t* member_details_form);
bool validate_member_details_contact_no(member_details_form_t* member_details_form);

#endif //TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H
