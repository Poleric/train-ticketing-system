#include <tui/form/member_details_form.h>
#include <string.h>

void init_member_details_form(register_form_t * register_form, WINDOW* form_window, const char* header, const char* form_header, member_t* member) {
    init_register_form(register_form, form_window, header, form_header);

    strncpy(register_form->form.fields[0].buffer, member->username, register_form->form.buffer_length);
    strncpy(register_form->form.fields[1].buffer, member->email, register_form->form.buffer_length);
    register_form->form.fields[4].buffer[0] = member->gender;
    strncpy(register_form->form.fields[5].buffer, member->contact_no, register_form->form.buffer_length);
}
