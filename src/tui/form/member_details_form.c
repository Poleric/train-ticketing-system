#include <tui/form/member_details_form.h>
#include <string.h>
#include <stdlib.h>
#include <utils.h>
#include <ctype.h>

#define MEMBER_USERNAME_LABEL         "Username         :"
#define MEMBER_EMAIL_LABEL            "Email address    :"
#define MEMBER_PASSWORD_LABEL         "Password         :"
#define MEMBER_CONFIRM_PASSWORD_LABEL "Confirm Password :"
#define MEMBER_GENDER_LABEL           "Gender (M / F)   :"
#define MEMBER_CONTACT_NO_LABEL       "Contact No       :"

#define MEMBER_DETAILS_LABEL_FIELD_LENGTH strlen(MEMBER_USERNAME_LABEL)
#define MEMBER_DETAILS_FIELD_GAP 1

#define MEMBER_DETAILS_FORM_SIDE_MARGIN_PERCENTAGE 0.1
#define MEMBER_DETAILS_FORM_TOP_MARGIN_PERCENTAGE 0.2

void init_member_details_form(member_details_form_t* member_details_form, WINDOW* form_window, const char* header, const char* form_header, member_t* member) {
    init_form(
            &member_details_form->form,
            form_window,
            6,
            MAX(
                    getmaxx(form_window) - MEMBER_DETAILS_LABEL_FIELD_LENGTH - MEMBER_DETAILS_FIELD_GAP - (int)(getmaxx(form_window) * MEMBER_DETAILS_FORM_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    member_details_form->form.field_label_width = MEMBER_DETAILS_LABEL_FIELD_LENGTH;

    member_details_form->header = header;
    member_details_form->header_y = 0;

    member_details_form->form_header = form_header;
    member_details_form->form_header_y = (int)(getmaxy(form_window) * MEMBER_DETAILS_FORM_TOP_MARGIN_PERCENTAGE);

    member_details_form->form.fields[0].label = MEMBER_USERNAME_LABEL;
    member_details_form->form.fields[0].y = member_details_form->form_header_y + 2;
    member_details_form->form.fields[0].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[0].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[0].offset_x + (int)member_details_form->form.buffer_length);
    strncpy(member_details_form->form.fields[0].buffer, member->username, member_details_form->form.buffer_length);
    member_details_form->form.fields[1].label = MEMBER_EMAIL_LABEL;
    member_details_form->form.fields[1].y = member_details_form->form_header_y + 3;
    member_details_form->form.fields[1].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[1].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[1].offset_x + (int)member_details_form->form.buffer_length);
    strncpy(member_details_form->form.fields[1].buffer, member->email, member_details_form->form.buffer_length);
    member_details_form->form.fields[2].label = MEMBER_PASSWORD_LABEL;
    member_details_form->form.fields[2].y = member_details_form->form_header_y + 5;
    member_details_form->form.fields[2].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[2].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[1].offset_x + (int)member_details_form->form.buffer_length);
    member_details_form->form.fields[3].label = MEMBER_CONFIRM_PASSWORD_LABEL;
    member_details_form->form.fields[3].y = member_details_form->form_header_y + 6;
    member_details_form->form.fields[3].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[3].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[1].offset_x + (int)member_details_form->form.buffer_length);
    member_details_form->form.fields[4].label = MEMBER_GENDER_LABEL;
    member_details_form->form.fields[4].y = member_details_form->form_header_y + 8;
    member_details_form->form.fields[4].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[4].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[1].offset_x + (int)member_details_form->form.buffer_length);
    member_details_form->form.fields[4].buffer[0] = member->gender;
    member_details_form->form.fields[5].label = MEMBER_CONTACT_NO_LABEL;
    member_details_form->form.fields[5].y = member_details_form->form_header_y + 9;
    member_details_form->form.fields[5].offset_x = MEMBER_DETAILS_LABEL_FIELD_LENGTH + MEMBER_DETAILS_FIELD_GAP;
    member_details_form->form.fields[5].x = get_centered_x_start(member_details_form->form.window, member_details_form->form.fields[1].offset_x + (int)member_details_form->form.buffer_length);
    strncpy(member_details_form->form.fields[5].buffer, member->contact_no, member_details_form->form.buffer_length);

    member_details_form->number_of_footers = 2;
    member_details_form->footers = calloc(member_details_form->number_of_footers, sizeof(const char*));
    member_details_form->footers[0] = "[Ctrl + C] Quit";
    member_details_form->footers[1] = "";

    member_details_form->footer_widths = calloc(member_details_form->number_of_footers, sizeof(int));
    member_details_form->footer_widths[0] = 1;
    member_details_form->footer_widths[1] = 4;

    member_details_form->form_message_y = member_details_form->form_header_y + 11;

    member_details_form->footer_y = getmaxy(form_window) - 1;

    member_details_form->member = member;

    scale_member_details_form_to_window(member_details_form);
}

void scale_member_details_form_to_window(member_details_form_t* member_details_form) {
    int max_x = getmaxx(member_details_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(member_details_form->footer_widths, member_details_form->number_of_footers);
    for (int i = 0; i < member_details_form->number_of_footers; i++)
        member_details_form->footer_widths[i] = (int)((float)member_details_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_member_details_form_fields(member_details_form_t* member_details_form) {
    int old_row = member_details_form->form.selection_row;
    member_details_form->form.selection_row = 0;
    for (int i = 0; i < member_details_form->form.number_of_fields; i++) {
        print_current_field_label(&member_details_form->form);
        print_current_field_buffer(&member_details_form->form);
        member_details_form->form.selection_row++;
    }
    member_details_form->form.selection_row = old_row;
}

void print_member_details_form_header(member_details_form_t* member_details_form, short color_pair) {
    wmove(member_details_form->form.window, member_details_form->header_y, get_centered_x_start(member_details_form->form.window, (int)strlen(member_details_form->header)));
    wprintw(member_details_form->form.window, "%s", member_details_form->header);

    move_to_x(member_details_form->form.window, 0);
    wchgat(member_details_form->form.window, member_details_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_member_details_form_form_header(member_details_form_t* member_details_form, short color_pair) {
    wmove(member_details_form->form.window, member_details_form->form_header_y, get_centered_x_start(member_details_form->form.window, (int)strlen(member_details_form->form_header)));
    wprintw(member_details_form->form.window, "%s", member_details_form->form_header);

    const int width = member_details_form->form.fields[0].offset_x + (int)member_details_form->form.buffer_length;
    move_to_x(member_details_form->form.window, member_details_form->form.fields[0].x);
    wchgat(member_details_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_member_details_form_footer(member_details_form_t* member_details_form, short color_pair) {
    wmove(member_details_form->form.window, member_details_form->footer_y, 0);

    int x = 0;
    for (int i = 0; i < member_details_form->number_of_footers; i++) {
        move_to_x(member_details_form->form.window, x);
        move_offset_x(member_details_form->form.window, get_offset_for_centered((int)strlen(member_details_form->footers[i]), member_details_form->footer_widths[i]));
        wprintw(member_details_form->form.window, "%s", member_details_form->footers[i]);
        x += member_details_form->footer_widths[i];
    }

    move_to_x(member_details_form->form.window, 0);
    wchgat(member_details_form->form.window, member_details_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_member_details_form_message(member_details_form_t* member_details_form, const char* message, short color_pair) {
    wmove(member_details_form->form.window, member_details_form->form_message_y, 0);
    wclrtoeol(member_details_form->form.window);

    move_to_x(member_details_form->form.window, get_centered_x_start(member_details_form->form.window, (int)strlen(message)));
    wattron(member_details_form->form.window, COLOR_PAIR(color_pair));
    wprintw(member_details_form->form.window, "%s", message);
    wattroff(member_details_form->form.window, COLOR_PAIR(color_pair));
}

void display_member_details_form(member_details_form_t* member_details_form, short color_pair) {
    print_member_details_form_header(member_details_form, color_pair);

    print_member_details_form_form_header(member_details_form, color_pair);

    print_member_details_form_fields(member_details_form);

    print_member_details_form_footer(member_details_form, color_pair);

    move_cursor_to_input_field(&member_details_form->form);

    wrefresh(member_details_form->form.window);
}

void free_member_details_form(member_details_form_t* member_details_form) {
    cleanup_form(&member_details_form->form);
    free(member_details_form->footers);
    free(member_details_form->footer_widths);
}

char* get_member_details_username(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[0].buffer;
}

char* get_member_details_email(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[1].buffer;
}

char* get_member_details_password(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[2].buffer;
}

char* get_member_details_confirm_password(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[3].buffer;
}

char get_member_details_gender(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[4].buffer[0];
}

char* get_member_details_contact_no(member_details_form_t* member_details_form) {
    return member_details_form->form.fields[5].buffer;
}

bool validate_member_details_email(member_details_form_t* member_details_form) {
    // contains @
    char* p = get_member_details_email(member_details_form);
    while (*p) {
        if (*p == '@') return true;
        p++;
    }
    return false;
}

bool validate_member_details_same_password(member_details_form_t* member_details_form) {
    return strcmp(get_member_details_password(member_details_form), get_member_details_confirm_password(member_details_form)) == 0;
}

bool validate_member_details_gender(member_details_form_t* member_details_form) {
    return get_member_details_gender(member_details_form) == 'M' || get_member_details_gender(member_details_form) == 'F';
}

bool validate_member_details_contact_no(member_details_form_t* member_details_form) {
    /*
     * Allow empty
     */
    char* p = get_member_details_contact_no(member_details_form);
    while (*p) {
        if (isalpha(*p)) return false;
        p++;
    }
    return true;
}
