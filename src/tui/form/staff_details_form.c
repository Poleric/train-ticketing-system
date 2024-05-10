#include <tui/form/staff_details_form.h>
#include <string.h>

void init_staff_details_form(staff_register_form_t * staff_register_form, WINDOW* form_window, const char* header, const char* form_header, staff_t* staff) {
    init_staff_register_form(staff_register_form, form_window, header, form_header);

    strncpy(staff_register_form->form.fields[0].buffer, staff->username, staff_register_form->form.buffer_length);
    strncpy(staff_register_form->form.fields[1].buffer, staff->email, staff_register_form->form.buffer_length);
    snprintf(staff_register_form->form.fields[4].buffer, staff_register_form->form.buffer_length, "%lf", staff->salary);
    strncpy(staff_register_form->form.fields[5].buffer, staff->contact_no, staff_register_form->form.buffer_length);
    strncpy(staff_register_form->form.fields[6].buffer, staff->position, staff_register_form->form.buffer_length);
}