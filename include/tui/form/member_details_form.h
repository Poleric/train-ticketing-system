#ifndef TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H
#define TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H

#include <tui/form/form.h>
#include <tui/form/register_member_form.h>
#include <member.h>

void init_member_details_form(register_form_t* register_form, WINDOW* form_window, const char* header, const char* form_header, member_t* member);

#endif //TRAINTICKETINGSYSTEM_MEMBER_DETAILS_FORM_H
