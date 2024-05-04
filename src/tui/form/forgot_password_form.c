#include <tui/form/forgot_password_form.h>
#include <string.h>
#include <utils.h>

#define FORGOT_PASSWORD_EMAIL_LABEL  "Email            :"
#define FORGOT_PASSWORD_CODE_LABEL   "Recovery code    :"
#define RESET_PASSWORD_LABEL         "New password     :"
#define RESET_CONFIRM_PASSWORD_LABEL "Confirm password :"

#define FORGOT_PASSWORD_LABEL_LENGTH strlen(FORGOT_PASSWORD_EMAIL_LABEL)
#define REGISTER_FIELD_GAP 1

#define FORGOT_PASSWORD_SIDE_MARGIN_PERCENTAGE 0.2
#define FORGOT_PASSWORD_TOP_MARGIN_PERCENTAGE 0.2

void init_forgot_password_form(forgot_password_form_t* forgot_password_form, WINDOW* form_window, const char* header, const char* form_header) {
    init_form(
            &forgot_password_form->form,
            form_window,
            2,
            MAX(
                    getmaxx(form_window) - FORGOT_PASSWORD_LABEL_LENGTH - REGISTER_FIELD_GAP - (int)(getmaxx(form_window) * FORGOT_PASSWORD_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    forgot_password_form->form.field_label_width = FORGOT_PASSWORD_LABEL_LENGTH;

    forgot_password_form->header = header;
    forgot_password_form->header_y = 0;

    forgot_password_form->form_header = form_header;
    forgot_password_form->form_header_y = (int)(getmaxy(form_window) * FORGOT_PASSWORD_TOP_MARGIN_PERCENTAGE);

    forgot_password_form->form.fields[0].label = FORGOT_PASSWORD_EMAIL_LABEL;
    forgot_password_form->form.fields[0].y = forgot_password_form->form_header_y + 2;
    forgot_password_form->form.fields[0].offset_x = FORGOT_PASSWORD_LABEL_LENGTH + REGISTER_FIELD_GAP;
    forgot_password_form->form.fields[0].x = get_centered_x_start(forgot_password_form->form.window, forgot_password_form->form.fields[0].offset_x + (int)forgot_password_form->form.buffer_length);
    forgot_password_form->form.fields[1].label = FORGOT_PASSWORD_CODE_LABEL;
    forgot_password_form->form.fields[1].y = forgot_password_form->form_header_y + 6;
    forgot_password_form->form.fields[1].offset_x = FORGOT_PASSWORD_LABEL_LENGTH + REGISTER_FIELD_GAP;
    forgot_password_form->form.fields[1].x = get_centered_x_start(forgot_password_form->form.window, forgot_password_form->form.fields[1].offset_x + (int)forgot_password_form->form.buffer_length);

    forgot_password_form->number_of_footers = 2;
    forgot_password_form->footers = calloc(forgot_password_form->number_of_footers, sizeof(const char*));
    forgot_password_form->footers[0] = "[Ctrl + C] Quit";
    forgot_password_form->footers[1] = "";

    forgot_password_form->footer_widths = calloc(forgot_password_form->number_of_footers, sizeof(int));
    forgot_password_form->footer_widths[0] = 1;
    forgot_password_form->footer_widths[1] = 4;

    forgot_password_form->form_message_y = forgot_password_form->form_header_y + 4;

    forgot_password_form->footer_y = getmaxy(form_window) - 1;

    scale_forgot_password_form_to_window(forgot_password_form);
}

void init_reset_password_fields(forgot_password_form_t* forgot_password_form) {
    forgot_password_form->form.fields[0].label = RESET_PASSWORD_LABEL;
    forgot_password_form->form.fields[0].y = forgot_password_form->form_header_y + 8;
    forgot_password_form->form.fields[1].label = RESET_CONFIRM_PASSWORD_LABEL;
    forgot_password_form->form.fields[1].y = forgot_password_form->form_header_y + 9;

    memset(forgot_password_form->form.fields[0].buffer, 0, forgot_password_form->form.buffer_length);
    forgot_password_form->form.fields[0].pos = 0;
    memset(forgot_password_form->form.fields[1].buffer, 0, forgot_password_form->form.buffer_length);
    forgot_password_form->form.fields[1].pos = 0;

    forgot_password_form->form.selection_row = 0;

    forgot_password_form->form_message_y = forgot_password_form->form_header_y + 11;
}

void scale_forgot_password_form_to_window(forgot_password_form_t* forgot_password_form) {
    int max_x = getmaxx(forgot_password_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(forgot_password_form->footer_widths, forgot_password_form->number_of_footers);
    for (int i = 0; i < forgot_password_form->number_of_footers; i++)
        forgot_password_form->footer_widths[i] = (int)((float)forgot_password_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_forgot_password_form_fields(forgot_password_form_t* forgot_password_form) {
    int old_row = forgot_password_form->form.selection_row;
    forgot_password_form->form.selection_row = 0;
    for (int i = 0; i < forgot_password_form->form.number_of_fields; i++) {
        print_current_field_label(&forgot_password_form->form);
        print_current_field_buffer(&forgot_password_form->form);
        forgot_password_form->form.selection_row++;
    }
    forgot_password_form->form.selection_row = old_row;
}

void print_forgot_password_form_header(forgot_password_form_t* forgot_password, short color_pair) {
    wmove(forgot_password->form.window, forgot_password->header_y, get_centered_x_start(forgot_password->form.window, (int)strlen(forgot_password->header)));
    wprintw(forgot_password->form.window, "%s", forgot_password->header);

    move_to_x(forgot_password->form.window, 0);
    wchgat(forgot_password->form.window, forgot_password->form.width, A_STANDOUT, color_pair, NULL);
}

void print_forgot_password_form_form_header(forgot_password_form_t* forgot_password_form, short color_pair) {
    wmove(forgot_password_form->form.window, forgot_password_form->form_header_y, get_centered_x_start(forgot_password_form->form.window, (int)strlen(forgot_password_form->form_header)));
    wprintw(forgot_password_form->form.window, "%s", forgot_password_form->form_header);

    const int width = forgot_password_form->form.fields[0].offset_x + (int)forgot_password_form->form.buffer_length;
    move_to_x(forgot_password_form->form.window, forgot_password_form->form.fields[0].x);
    wchgat(forgot_password_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_forgot_password_form_footer(forgot_password_form_t* forgot_password_form, short color_pair) {
    wmove(forgot_password_form->form.window, forgot_password_form->footer_y, 0);

    int x = 0;
    for (int i = 0; i < forgot_password_form->number_of_footers; i++) {
        move_to_x(forgot_password_form->form.window, x);
        move_offset_x(forgot_password_form->form.window, get_offset_for_centered((int)strlen(forgot_password_form->footers[i]), forgot_password_form->footer_widths[i]));
        wprintw(forgot_password_form->form.window, "%s", forgot_password_form->footers[i]);
        x += forgot_password_form->footer_widths[i];
    }

    move_to_x(forgot_password_form->form.window, 0);
    wchgat(forgot_password_form->form.window, forgot_password_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_forgot_password_form_message(forgot_password_form_t* forgot_password_form, const char* message, short color_pair) {
    wmove(forgot_password_form->form.window, forgot_password_form->form_message_y, 0);
    wclrtoeol(forgot_password_form->form.window);

    move_to_x(forgot_password_form->form.window, get_centered_x_start(forgot_password_form->form.window, (int)strlen(message)));
    wattron(forgot_password_form->form.window, COLOR_PAIR(color_pair));
    wprintw(forgot_password_form->form.window, "%s", message);
    wattroff(forgot_password_form->form.window, COLOR_PAIR(color_pair));
}

void display_forgot_password_form(forgot_password_form_t* forgot_password_form, short color_pair) {
    print_forgot_password_form_header(forgot_password_form, color_pair);

    print_forgot_password_form_form_header(forgot_password_form, color_pair);

    print_forgot_password_form_fields(forgot_password_form);

    print_forgot_password_form_footer(forgot_password_form, color_pair);

    move_cursor_to_input_field(&forgot_password_form->form);

    wrefresh(forgot_password_form->form.window);
}

form_action_t forgot_password_form_driver(forgot_password_form_t* forgot_password_form, int ch) {
    // super scuffed
    form_action_t action;
    int old_selection = forgot_password_form->form.selection_row;
    action = form_driver(&forgot_password_form->form, ch);

    if (forgot_password_form->form.selection_row - old_selection == 1)
        return RELOAD_ACTION;
    return action;
}

void free_forgot_password_form(forgot_password_form_t* forgot_password_form) {
    cleanup_form(&forgot_password_form->form);
    free(forgot_password_form->footers);
    free(forgot_password_form->footer_widths);
}

char* get_recovery_email(forgot_password_form_t* forgot_password_form) {
    return forgot_password_form->form.fields[0].buffer;
}

int get_recovery_code(forgot_password_form_t* forgot_password_form) {
    return (int)strtol(forgot_password_form->form.fields[1].buffer, NULL, 10);
}

char* get_recovery_password(forgot_password_form_t* forgot_password_form) {
    return forgot_password_form->form.fields[0].buffer;
}

char* get_recovery_confirm_password(forgot_password_form_t* forgot_password_form) {
    return forgot_password_form->form.fields[1].buffer;
}

bool validate_new_password_same(forgot_password_form_t* forgot_password_form) {
    return strcmp(get_recovery_password(forgot_password_form), get_recovery_confirm_password(forgot_password_form)) == 0;
}
