#include <tui/form/register_form.h>
#include <string.h>
#include <utils.h>

#define REGISTER_USERNAME_LABEL         "Username         :"
#define REGISTER_EMAIL_LABEL            "Email address    :"
#define REGISTER_PASSWORD_LABEL         "Password         :"
#define REGISTER_CONFIRM_PASSWORD_LABEL "Confirm Password :"
#define REGISTER_GENDER_LABEL           "Gender (M / F)   :"
#define REGISTER_CONTACT_NO_LABEL       "Contact No       :"

#define REGISTER_LABEL_FIELD_LENGTH strlen(REGISTER_USERNAME_LABEL)
#define REGISTER_FIELD_GAP 1

#define REGISTER_FORM_SIDE_MARGIN_PERCENTAGE 0.1
#define REGISTER_FORM_TOP_MARGIN_PERCENTAGE 0.2

void init_register_form(register_form_t* register_form, WINDOW* form_window, const char* header, const char* form_header) {
    const int FIELD_LABEL_WIDTH = REGISTER_LABEL_FIELD_LENGTH;

    init_form(
            &register_form->form,
            form_window,
            6,
            MAX(
                    getmaxx(form_window) - FIELD_LABEL_WIDTH - REGISTER_FIELD_GAP - (int)(getmaxx(form_window) * REGISTER_FORM_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    register_form->form.field_label_width = FIELD_LABEL_WIDTH;

    register_form->header = header;
    register_form->header_y = 0;

    register_form->form_header = form_header;
    register_form->form_header_y = (int)(getmaxy(form_window) * REGISTER_FORM_TOP_MARGIN_PERCENTAGE);

    register_form->form.fields[0].label = REGISTER_USERNAME_LABEL;
    register_form->form.fields[0].y = register_form->form_header_y + 2;
    register_form->form.fields[0].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[0].x = get_centered_x_start(register_form->form.window, register_form->form.fields[0].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[1].label = REGISTER_EMAIL_LABEL;
    register_form->form.fields[1].y = register_form->form_header_y + 3;
    register_form->form.fields[1].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[1].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[2].label = REGISTER_PASSWORD_LABEL;
    register_form->form.fields[2].y = register_form->form_header_y + 5;
    register_form->form.fields[2].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[2].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[3].label = REGISTER_CONFIRM_PASSWORD_LABEL;
    register_form->form.fields[3].y = register_form->form_header_y + 6;
    register_form->form.fields[3].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[3].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[4].label = REGISTER_GENDER_LABEL;
    register_form->form.fields[4].y = register_form->form_header_y + 8;
    register_form->form.fields[4].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[4].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[5].label = REGISTER_CONTACT_NO_LABEL;
    register_form->form.fields[5].y = register_form->form_header_y + 9;
    register_form->form.fields[5].offset_x = FIELD_LABEL_WIDTH + REGISTER_FIELD_GAP;
    register_form->form.fields[5].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);

    register_form->form_message_y = register_form->form_header_y + 11;

    register_form->footer_y = getmaxy(form_window) - 1;
}

void print_register_fields(register_form_t* register_form) {
    int old_row = register_form->form.selection_row;
    register_form->form.selection_row = 0;
    for (int i = 0; i < register_form->form.number_of_fields; i++) {
        print_current_field_label(&register_form->form);
        print_current_field_buffer(&register_form->form);
        register_form->form.selection_row++;
    }
    register_form->form.selection_row = old_row;
}

void print_register_form_header(register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->header_y, get_centered_x_start(register_form->form.window, (int)strlen(register_form->header)));
    wprintw(register_form->form.window, "%s", register_form->header);

    move_to_x(register_form->form.window, 0);
    wchgat(register_form->form.window, register_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_register_form_form_header(register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->form_header_y, get_centered_x_start(register_form->form.window, (int)strlen(register_form->form_header)));
    wprintw(register_form->form.window, "%s", register_form->form_header);

    const int width = register_form->form.fields[0].offset_x + (int)register_form->form.buffer_length;
    move_to_x(register_form->form.window, register_form->form.fields[0].x);
    wchgat(register_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_register_form_footer(register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->footer_y, 0);
    wchgat(register_form->form.window, register_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_register_form_message(register_form_t* register_form, const char* message, short color_pair) {
    wmove(register_form->form.window, register_form->form_message_y, 0);
    wclrtoeol(register_form->form.window);

    move_to_x(register_form->form.window, get_centered_x_start(register_form->form.window, (int)strlen(message)));
    wattron(register_form->form.window, COLOR_PAIR(color_pair));
    wprintw(register_form->form.window, "%s", message);
    wattroff(register_form->form.window, COLOR_PAIR(color_pair));
}

void display_register_form(register_form_t* register_form, short color_pair) {
    print_register_form_header(register_form, color_pair);

    print_register_form_form_header(register_form, color_pair);

    print_register_fields(register_form);

    print_register_form_footer(register_form, color_pair);

    move_cursor_to_input_field(&register_form->form);

    wrefresh(register_form->form.window);
}

void free_register_form(register_form_t* register_form) {
    cleanup_form(&register_form->form);
}

char* get_register_username(register_form_t* register_form) {
    return register_form->form.fields[0].buffer;
}

char* get_register_email(register_form_t* register_form) {
    return register_form->form.fields[1].buffer;
}

char* get_register_password(register_form_t* register_form) {
    return register_form->form.fields[2].buffer;
}

char* get_register_confirm_password(register_form_t* register_form) {
    return register_form->form.fields[3].buffer;
}

char get_register_gender(register_form_t* register_form) {
    return register_form->form.fields[4].buffer[0];
}

char* get_register_contact_no(register_form_t* register_form) {
    return register_form->form.fields[5].buffer;
}

