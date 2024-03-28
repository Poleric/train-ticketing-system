#include <tui/template/login_menu.h>
#include <stdlib.h>
#include <string.h>


static FIELD** init_login_fields (int input_field_length) {
    FIELD** fields = calloc(sizeof(FIELD*), 3);
    fields[0] = new_field(1, input_field_length, 0, LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP, 0, 0);
    fields[1] = new_field(1, input_field_length, 1, LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP, 0, 0);
    fields[2] = NULL;

    field_opts_off(fields[0], O_AUTOSKIP);
    field_opts_off(fields[1], O_AUTOSKIP | O_PUBLIC);

    set_field_back(fields[0], A_UNDERLINE);
    set_field_back(fields[1], A_UNDERLINE);

    return fields;
}

FORM* init_login_form(WINDOW* form_window, const char* header_message, bool box, int margin_from_sides) {
    bool have_header = header_message != NULL;
    int WINDOW_LINES, WINDOW_COLS;
    getmaxyx(form_window, WINDOW_LINES, WINDOW_COLS);

    FIELD** login_fields = init_login_fields(WINDOW_COLS - LOGIN_LABEL_FIELD_LENGTH - LOGIN_FIELD_GAP - margin_from_sides * 2 - box * 2);

    FORM* form = new_form(login_fields);
    set_form_userptr(form, login_fields);  // userptr is the login field to free later

    WINDOW* form_fields_win = derwin(
            form_window,
            WINDOW_LINES - margin_from_sides * 2 - have_header - box * 2,
            WINDOW_COLS - margin_from_sides * 2 - box * 2,
            margin_from_sides + have_header + box,
            margin_from_sides + box
            );

    set_form_win(form, form_window);
    set_form_sub(form, form_fields_win);

    keypad(form_fields_win, TRUE);

    post_form(form);

    if (box)
        box(form_window, 0, 0);

    if (header_message)
        print_login_form_header(form_window, header_message, box);

    print_login_field_labels(form_fields_win);

    wrefresh(form_window);
    wrefresh(form_fields_win);

    return form;
}

static void print_login_form_header(WINDOW* window, const char* header_message, bool box) {
    mvwprintw(window, box, get_centered_x_start(window, (int)strlen(header_message)), "%s", header_message);
    mvwchgat(window, box, box, getmaxx(window) - box * 2, A_STANDOUT, 0, 0);
}

static void print_login_field_labels(WINDOW* form_fields_window) {
    mvwprintw(form_fields_window, 0, 0, LOGIN_USERNAME_LABEL);
    mvwprintw(form_fields_window, 1, 0, LOGIN_PASSWORD_LABEL);
    wmove(form_fields_window, 0, LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP);
}

login_form_action_t login_form_driver(FORM* login_form, int ch, char** username_buffer, char** password_buffer) {
    static int selection_index = 0;

    switch (ch) {
        case KEY_F(1):
            return SWITCH_MENU_ACTION;

        case KEY_ESC:
        case CTRL('C'):
            store_last_pos(stdscr);
            if (confirmation_menu(form_sub(login_form), "Exit Menu?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;
            restore_last_pos(stdscr);
            break;

        case KEY_DOWN:
            if (selection_index < 1) {
                form_driver(login_form, REQ_NEXT_FIELD);
                form_driver(login_form, REQ_END_LINE);
                selection_index++;
            }
            break;

        case KEY_UP:
            if (selection_index > 0) {
                form_driver(login_form, REQ_PREV_FIELD);
                form_driver(login_form, REQ_END_LINE);
                selection_index--;
            }
            break;

        case KEY_LEFT:
            form_driver(login_form, REQ_PREV_CHAR);
            break;

        case KEY_RIGHT:
            form_driver(login_form, REQ_NEXT_CHAR);
            break;

        case KEY_ENTER:
        case '\n':
        case '\r':
            if (selection_index < 1) {
                form_driver(login_form, REQ_NEXT_FIELD);
                form_driver(login_form, REQ_END_LINE);
                selection_index++;
            } else {  // submit
                // store input
                form_driver(login_form, REQ_VALIDATION);

                FIELD **fields = form_fields(login_form);
                *username_buffer = trim_whitespaces(field_buffer(fields[0], 0));
                *password_buffer = trim_whitespaces(field_buffer(fields[1], 0));
                return LOGIN_ACTION;
            }
            break;

        case KEY_BACKSPACE:
        case 127:
            form_driver(login_form, REQ_DEL_PREV);
            break;

        case KEY_DC:
            form_driver(login_form, REQ_DEL_CHAR);
            break;

        case KEY_CTRL_BACKSPACE:
            form_driver(login_form, REQ_DEL_WORD);
            break;

        default:
            form_driver(login_form, ch);
            break;
    }
    wrefresh(form_win(login_form));
    wrefresh(form_sub(login_form));

    return NO_ACTION;
}

void print_message_below_form(FORM* login_form, const char* message, int offset_y, int offset_x, bool center) {
    int start_y, start_x;

    WINDOW* fields_win = form_sub(login_form);
    store_last_pos(fields_win);

    start_y = 2 + offset_y;
    if (center)
        start_x = get_centered_x_start(fields_win, (int)strlen(message) + offset_x);
    else
        start_x = offset_x;

    mvwprintw(fields_win, start_y, start_x, "%s", message);
    restore_last_pos(fields_win);
    wrefresh(fields_win);
}

void cleanup_login_form(FORM* login_form) {
    /*
     * Clear and free memory
     */
    unpost_form(login_form);

    WINDOW* window = form_win(login_form);
    WINDOW* sub_win = form_sub(login_form);
    wclear(window); wclear(sub_win);
    wrefresh(window); wrefresh(sub_win);

    FIELD** login_fields = form_userptr(login_form);

    free_form(login_form);
    free_login_fields(login_fields);

    delwin(sub_win);
}

static void free_login_fields(FIELD** login_fields) {
    for (int i = 0; login_fields[i]; i++)
        free_field(login_fields[i]);
    free(login_fields);
}