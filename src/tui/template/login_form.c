#include <tui/template/login_form.h>

FORM* create_login_form(WINDOW* form_window) {
    FORM* login_form = init_form(form_window, 2, LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP);

    if (login_form == NULL)
        return NULL;

    print_login_form(login_form);

    wrefresh(login_form->window);

    return login_form;
}

void print_login_fields(FORM* login_form, bool underline) {
    /*
     * Prints labels and the input fields
     */

    wmove(login_form->window, 0, 0);
    wprintw(login_form->window, LOGIN_USERNAME_LABEL);
    if (underline) {
        wadd_chars(login_form->window, ' ', LOGIN_FIELD_GAP);
        wchgat(login_form->window, -1, A_UNDERLINE, 0, 0);
    }

    wmove(login_form->window, 1, 0);
    wprintw(login_form->window, LOGIN_PASSWORD_LABEL);
    if (underline) {
        wadd_chars(login_form->window, ' ', LOGIN_FIELD_GAP);
        wchgat(login_form->window, -1, A_UNDERLINE, 0, 0);
    }
}

void print_login_form(FORM* login_form) {
    print_login_fields(login_form, TRUE);
    move_cursor_to_end_of_input_field(login_form, 0);
}

char* get_username(FORM* login_form) {
    return login_form->field_buffers[0];
}

char* get_password(FORM* login_form) {
    return login_form->field_buffers[1];
}
