#include <tui/form/register_staff_form.h>
#include <string.h>
#include <utils.h>
#include <ctype.h>
#include "tui/form/register_form.h"

#define STAFF_REGISTER_LABEL_1  "Username         :"
#define STAFF_REGISTER_LABEL_2  "Email address    :"
#define STAFF_REGISTER_LABEL_3  "Password         :"
#define STAFF_REGISTER_LABEL_4  "Confirm Password :"
#define STAFF_REGISTER_LABEL_5  "Salary           :"
#define STAFF_REGISTER_LABEL_6  "Contact No       :"
#define STAFF_REGISTER_LABEL_7  "Position         :"

#define STAFF_REGISTER_LABEL_FIELD_LENGTH strlen(STAFF_REGISTER_LABEL_1)
#define STAFF_REGISTER_FIELD_GAP 1

#define STAFF_REGISTER_FORM_SIDE_MARGIN_PERCENTAGE 0.1
#define STAFF_REGISTER_FORM_TOP_MARGIN_PERCENTAGE 0.2

void init_staff_register_form(staff_register_form_t* register_form, WINDOW* form_window, const char* header, const char* form_header) {
    const int FIELD_LABEL_WIDTH = STAFF_REGISTER_LABEL_FIELD_LENGTH;

    init_form(
            &register_form->form,
            form_window,
            7,
            MAX(
                    getmaxx(form_window) - FIELD_LABEL_WIDTH - STAFF_REGISTER_FIELD_GAP - (int)(getmaxx(form_window) * STAFF_REGISTER_FORM_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    register_form->form.field_label_width = FIELD_LABEL_WIDTH;

    register_form->header = header;
    register_form->header_y = 0;

    register_form->form_header = form_header;
    register_form->form_header_y = (int)(getmaxy(form_window) * STAFF_REGISTER_FORM_TOP_MARGIN_PERCENTAGE);

    register_form->form.fields[0].label = STAFF_REGISTER_LABEL_1;
    register_form->form.fields[0].y = register_form->form_header_y + 2;
    register_form->form.fields[0].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[0].x = get_centered_x_start(register_form->form.window, register_form->form.fields[0].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[1].label = STAFF_REGISTER_LABEL_2;
    register_form->form.fields[1].y = register_form->form_header_y + 3;
    register_form->form.fields[1].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[1].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[2].label = STAFF_REGISTER_LABEL_3;
    register_form->form.fields[2].y = register_form->form_header_y + 5;
    register_form->form.fields[2].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[2].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[3].label = STAFF_REGISTER_LABEL_4;
    register_form->form.fields[3].y = register_form->form_header_y + 6;
    register_form->form.fields[3].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[3].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[4].label = STAFF_REGISTER_LABEL_5;
    register_form->form.fields[4].y = register_form->form_header_y + 8;
    register_form->form.fields[4].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[4].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);
    register_form->form.fields[5].label = STAFF_REGISTER_LABEL_6;
    register_form->form.fields[5].y = register_form->form_header_y + 9;
    register_form->form.fields[5].offset_x = FIELD_LABEL_WIDTH + STAFF_REGISTER_FIELD_GAP;
    register_form->form.fields[5].x = get_centered_x_start(register_form->form.window, register_form->form.fields[1].offset_x + (int)register_form->form.buffer_length);

    register_form->number_of_footers = 2;
    register_form->footers = calloc(register_form->number_of_footers, sizeof(const char*));
    register_form->footers[0] = "[Ctrl + C] Quit";
    register_form->footers[1] = "";

    register_form->footer_widths = calloc(register_form->number_of_footers, sizeof(int));
    register_form->footer_widths[0] = 1;
    register_form->footer_widths[1] = 4;

    register_form->form_message_y = register_form->form_header_y + 11;

    register_form->footer_y = getmaxy(form_window) - 1;

    scale_staff_register_form_to_window(register_form);
}

void scale_staff_register_form_to_window(staff_register_form_t* register_form) {
    int max_x = getmaxx(register_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(register_form->footer_widths, register_form->number_of_footers);
    for (int i = 0; i < register_form->number_of_footers; i++)
        register_form->footer_widths[i] = (int)((float)register_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_staff_register_fields(staff_register_form_t* register_form) {
    int old_row = register_form->form.selection_row;
    register_form->form.selection_row = 0;
    for (int i = 0; i < register_form->form.number_of_fields; i++) {
        print_current_field_label(&register_form->form);
        print_current_field_buffer(&register_form->form);
        register_form->form.selection_row++;
    }
    register_form->form.selection_row = old_row;
}

void print_staff_register_form_header(staff_register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->header_y, get_centered_x_start(register_form->form.window, (int)strlen(register_form->header)));
    wprintw(register_form->form.window, "%s", register_form->header);

    move_to_x(register_form->form.window, 0);
    wchgat(register_form->form.window, register_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_staff_register_form_form_header(staff_register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->form_header_y, get_centered_x_start(register_form->form.window, (int)strlen(register_form->form_header)));
    wprintw(register_form->form.window, "%s", register_form->form_header);

    const int width = register_form->form.fields[0].offset_x + (int)register_form->form.buffer_length;
    move_to_x(register_form->form.window, register_form->form.fields[0].x);
    wchgat(register_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_staff_register_form_footer(staff_register_form_t* register_form, short color_pair) {
    wmove(register_form->form.window, register_form->footer_y, 0);

    int x = 0;
    for (int i = 0; i < register_form->number_of_footers; i++) {
        move_to_x(register_form->form.window, x);
        move_offset_x(register_form->form.window, get_offset_for_centered((int)strlen(register_form->footers[i]), register_form->footer_widths[i]));
        wprintw(register_form->form.window, "%s", register_form->footers[i]);
        x += register_form->footer_widths[i];
    }

    move_to_x(register_form->form.window, 0);
    wchgat(register_form->form.window, register_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_staff_register_form_message(staff_register_form_t* register_form, const char* message, short color_pair) {
    wmove(register_form->form.window, register_form->form_message_y, 0);
    wclrtoeol(register_form->form.window);

    move_to_x(register_form->form.window, get_centered_x_start(register_form->form.window, (int)strlen(message)));
    wattron(register_form->form.window, COLOR_PAIR(color_pair));
    wprintw(register_form->form.window, "%s", message);
    wattroff(register_form->form.window, COLOR_PAIR(color_pair));
}

void display_staff_register_form(staff_register_form_t* register_form, short color_pair) {
    print_staff_register_form_header(register_form, color_pair);

    print_staff_register_form_form_header(register_form, color_pair);

    print_staff_register_fields(register_form);

    print_staff_register_form_footer(register_form, color_pair);

    move_cursor_to_input_field(&register_form->form);

    wrefresh(register_form->form.window);
}

void free_staff_register_form(staff_register_form_t* register_form) {
    cleanup_form(&register_form->form);
    free(register_form->footers);
    free(register_form->footer_widths);
}

char* get_staff_register_username(staff_register_form_t* register_form) {
    return register_form->form.fields[0].buffer;
}

char* get_staff_register_email(staff_register_form_t* register_form) {
    return register_form->form.fields[1].buffer;
}

char* get_staff_register_password(staff_register_form_t* register_form) {
    return register_form->form.fields[2].buffer;
}

char* get_staff_register_confirm_password(staff_register_form_t* register_form) {
    return register_form->form.fields[3].buffer;
}

double get_staff_register_salary(staff_register_form_t* register_form) {
    return strtod(register_form->form.fields[4].buffer, NULL);
}

char* get_staff_register_contact_no(staff_register_form_t* register_form) {
    return register_form->form.fields[5].buffer;
}

char* get_staff_register_position(staff_register_form_t* register_form) {
    return register_form->form.fields[6].buffer;
}

bool validate_staff_register_email(staff_register_form_t* register_form) {
    // contains @
    char* p = get_staff_register_email(register_form);
    while (*p) {
        if (*p == '@') return true;
        p++;
    }
    return false;
}

bool validate_staff_register_same_password(staff_register_form_t* register_form) {
    return strcmp(get_staff_register_password(register_form), get_staff_register_confirm_password(register_form)) == 0;
}

bool validate_staff_register_salary(staff_register_form_t* register_form) {
    return get_staff_register_salary(register_form) > 0;
}

bool validate_staff_register_contact_no(staff_register_form_t* register_form) {
    /*
     * Allow empty
     */
    char* p = get_staff_register_contact_no(register_form);
    while (*p) {
        if (isalpha(*p)) return false;
        p++;
    }
    return true;
}

