#ifndef TRAINTICKETINGSYSTEM_STAFF_DETAILS_FORM_H
#define TRAINTICKETINGSYSTEM_STAFF_DETAILS_FORM_H

#include <tui/form/register_staff_form.h>
#include <staff.h>

void init_staff_details_form(staff_register_form_t * staff_register_form, WINDOW* form_window, const char* header, const char* form_header, staff_t* member);


#endif //TRAINTICKETINGSYSTEM_STAFF_DETAILS_FORM_H
