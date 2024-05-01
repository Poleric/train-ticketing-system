#include <tui/form/login_form.h>
#include <utils.h>
#include <string.h>

#define LOGIN_USERNAME_LABEL "Username :"
#define LOGIN_PASSWORD_LABEL "Password :"
#define LOGIN_LABEL_FIELD_LENGTH MAX(strlen(LOGIN_USERNAME_LABEL), strlen(LOGIN_PASSWORD_LABEL))
#define LOGIN_FIELD_GAP 1

#define LOGIN_FORM_MARGIN_PERCENTAGE 0.2

void init_login_form(login_form_t* login_form, WINDOW* form_window, const char* header) {
    const int FIELD_LABEL_WIDTH = LOGIN_LABEL_FIELD_LENGTH;

    init_form(
            &login_form->form,
            form_window,
            2,
            MAX(
                    getmaxx(form_window) - FIELD_LABEL_WIDTH - LOGIN_FIELD_GAP - (int)(getmaxx(form_window) * LOGIN_FORM_MARGIN_PERCENTAGE),
                    20
                    )
            );

    login_form->form.field_label_width = FIELD_LABEL_WIDTH;

    login_form->form.fields[0].label = LOGIN_USERNAME_LABEL;
    login_form->form.fields[0].y = 3;
    login_form->form.fields[0].offset_x = FIELD_LABEL_WIDTH + LOGIN_FIELD_GAP;
    login_form->form.fields[0].x = get_centered_x_start(login_form->form.window, login_form->form.fields[0].offset_x + (int)login_form->form.buffer_length);
    login_form->form.fields[1].label = LOGIN_PASSWORD_LABEL;
    login_form->form.fields[1].y = 4;
    login_form->form.fields[1].offset_x = FIELD_LABEL_WIDTH + LOGIN_FIELD_GAP;
    login_form->form.fields[1].x = get_centered_x_start(login_form->form.window, login_form->form.fields[1].offset_x + (int)login_form->form.buffer_length);

    login_form->header = header;

    login_form->number_of_footers = 4;
    login_form->footers = calloc(login_form->number_of_footers, sizeof(const char*));
    login_form->footers[0] = "[Ctrl + C] Quit";
    login_form->footers[1] = "[F1] Change Menu";
    login_form->footers[2] = "[F2] Forgot Password";
    login_form->footers[3] = "[F10] Refresh Menu";

    login_form->footer_widths = calloc(login_form->number_of_footers, sizeof(int));
    login_form->footer_widths[0] = 1;
    login_form->footer_widths[1] = 1;
    login_form->footer_widths[2] = 1;
    login_form->footer_widths[3] = 1;

    scale_login_form_to_window(login_form);
}

void scale_login_form_to_window(login_form_t* login_form) {
    int max_x = getmaxx(login_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(login_form->footer_widths, login_form->number_of_footers);
    for (int i = 0; i < login_form->number_of_footers; i++)
        login_form->footer_widths[i] = (int)((float)login_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_login_fields(login_form_t* login_form) {
    for (int i = 0; i < login_form->form.number_of_fields; i++) {
        print_current_field_label(&login_form->form);
        print_current_field_buffer(&login_form->form);
        login_form->form.selection_row++;
    }
    login_form->form.selection_row = 0;
}

void print_login_form_header(login_form_t* login_form, short color_pair) {
    wmove(login_form->form.window, 0, get_centered_x_start(login_form->form.window, (int)strlen(login_form->header)));
    wprintw(login_form->form.window, "%s", login_form->header);

    move_to_x(login_form->form.window, 0);
    wchgat(login_form->form.window, login_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_login_form_footer(login_form_t* login_form, short color_pair) {
    wmove(login_form->form.window, getmaxy(login_form->form.window) - 1, 0);

    int x = 0;
    for (int i = 0; i < login_form->number_of_footers; i++) {
        move_to_x(login_form->form.window, x);
        move_offset_x(login_form->form.window, get_offset_for_centered((int)strlen(login_form->footers[i]), login_form->footer_widths[i]));
        wprintw(login_form->form.window, "%s", login_form->footers[i]);
        x += login_form->footer_widths[i];
    }

    move_to_x(login_form->form.window, 0);
    wchgat(login_form->form.window, login_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_login_form_message(login_form_t* login_form, const char* message, short color_pair) {
    wmove(login_form->form.window, 6, 0);
    wclrtoeol(login_form->form.window);

    wmove(login_form->form.window, 6, get_centered_x_start(login_form->form.window, (int)strlen(message)));
    wattron(login_form->form.window, COLOR_PAIR(color_pair));
    wprintw(login_form->form.window, "%s", message);
    wattroff(login_form->form.window, COLOR_PAIR(color_pair));
}

void display_login_form(login_form_t* login_form) {
    print_login_form_header(login_form, 1);

    print_login_fields(login_form);

    print_login_form_footer(login_form, 1);

    move_cursor_to_start_of_input_field(&login_form->form);

    wrefresh(login_form->form.window);
}

void free_login_form(login_form_t* login_form) {
    cleanup_form(&login_form->form);

    free(login_form->footers);
    free(login_form->footer_widths);
}

char* get_username(login_form_t* login_form) {
    return login_form->form.fields[0].buffer;
}

char* get_password(login_form_t* login_form) {
    return login_form->form.fields[1].buffer;
}
