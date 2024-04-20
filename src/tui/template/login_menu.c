#include <tui/template/login_menu.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static int LOGIN_FORM_LINES = 0, LOGIN_FORM_COLS = 0;
static int selection_row = 0, selection_col = 0;

LOGIN_FORM* init_login_form(WINDOW* form_window) {
    LOGIN_FORM* login_form = malloc(sizeof (LOGIN_FORM));

    if (login_form == NULL)
        return NULL;

    getmaxyx(form_window, LOGIN_FORM_LINES, LOGIN_FORM_COLS);
    login_form->window = form_window;
    login_form->buffer_length = LOGIN_FORM_COLS - LOGIN_LABEL_FIELD_LENGTH - LOGIN_FIELD_GAP;
    login_form->n_buffer = 2;
    login_form->field_buffers = malloc(login_form->n_buffer * sizeof (char *));
    for (int i = 0; i < login_form->n_buffer; i++)
        login_form->field_buffers[i] = calloc(sizeof (char), login_form->buffer_length);

    return login_form;
}

void print_login_fields(LOGIN_FORM* login_form, bool underline) {
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

void move_cursor_to_input_field(LOGIN_FORM* login_form, int field_n, int pos) {
    /*
     * field_n: 0 - username field, 1 - password field
     */
    int y, x;

    y = field_n;
    x = LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP + pos;

    wmove(login_form->window, y, x);
}

int move_cursor_to_end_of_input_field(LOGIN_FORM* login_form, int field_n) {
    int pos = (int)strlen(login_form->field_buffers[field_n]);
    move_cursor_to_input_field(login_form, field_n, pos);
    return pos;
}

void print_input_field_buffer(LOGIN_FORM* login_form, int field_n) {
    int y, x;

    y = field_n;
    x = LOGIN_LABEL_FIELD_LENGTH + LOGIN_FIELD_GAP;

    wmove(login_form->window, y, x); wclrtoeol(login_form->window);
    mvwprintw(login_form->window, y, x, "%s", login_form->field_buffers[field_n]);
    wmove(login_form->window, y, x); wchgat(login_form->window, -1, A_UNDERLINE, 0, 0);
}

void delete_char(char* buffer, int pos) {
    /* unsafe
     * helper function to remove character in middle and shift all char
     */
    buffer += pos;
    while (*(buffer++)) {
        *(buffer - 1) = *buffer;
    }
    *buffer = 0;
}


void add_char(char* buffer, char ch, int pos) {
    /* unsafe
     * helper function to add character in middle and shift all char
     */
    size_t i = strlen(buffer);
    buffer += i;
    *(buffer + 1) = 0;
    while (i-- != pos) {
        *buffer = *(buffer - 1);
        buffer--;
    }
    *buffer = ch;
}

login_form_action_t form_driver(LOGIN_FORM* login_form, int ch) {
    switch (ch) {
        case KEY_F(1):
            return SWITCH_MENU_ACTION;

        case KEY_F(2):
            return RELOAD_ACTION;

        case KEY_ESC:
        case CTRL('C'):
            store_last_pos(login_form->window);
            if (confirmation_menu(login_form->window, "Exit Menu?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;

            restore_last_pos(login_form->window);
            break;

        case CTRL('R'):
            return REGISTER_ACTION;

        case KEY_DOWN:
            if (selection_row < 1) {
                selection_row++;
                selection_col = move_cursor_to_end_of_input_field(login_form, selection_row);
            }
            break;

        case KEY_UP:
            if (selection_row > 0) {
                selection_row--;
                selection_col = move_cursor_to_end_of_input_field(login_form, selection_row);
            }
            break;

        case KEY_RIGHT:
            if (selection_col < strlen(login_form->field_buffers[selection_row])) {
                selection_col++;
                move_cursor_to_input_field(login_form, selection_row, selection_col);
            }
            break;

        case KEY_LEFT:
            if (selection_col > 0) {
                selection_col--;
                move_cursor_to_input_field(login_form, selection_row, selection_col);
            }
            break;

        case KEY_ENTER:
        case '\n':
        case '\r':
            if (selection_row < 1) {  // same as key down
                selection_row++;
                selection_col = move_cursor_to_end_of_input_field(login_form, selection_row);
            }
            else {  // submit
                return SUBMIT_ACTION;
            }
            break;

        case KEY_BACKSPACE:
        case 127:
        case '\b':
            if (selection_col > 0) {
                delete_char(login_form->field_buffers[selection_row], --selection_col);
                print_input_field_buffer(login_form, selection_row);
                move_cursor_to_input_field(login_form, selection_row, selection_col);
            }
            break;

        case KEY_DC:
            if ((strlen(login_form->field_buffers[selection_row]) - selection_col) > 0) {
                delete_char(login_form->field_buffers[selection_row], selection_col);
                print_input_field_buffer(login_form, selection_row);
                move_cursor_to_input_field(login_form, selection_row, selection_col);
            }
            break;

        default:
            if (isprint(ch))
                if ((strlen(login_form->field_buffers[selection_row]) < login_form->buffer_length)) {
                    add_char(login_form->field_buffers[selection_row], (char)ch, selection_col++);
                    print_input_field_buffer(login_form, selection_row);
                    move_cursor_to_input_field(login_form, selection_row, selection_col);
                }
            break;
    }
    wrefresh(login_form->window);

    return NO_ACTION;
}

void free_login_form(LOGIN_FORM* login_form) {
    for (int i = 0; i < login_form->n_buffer; i++)
        free(login_form->field_buffers[i]);
    free(login_form->field_buffers);
    free(login_form);
}

void cleanup_login_form(LOGIN_FORM* login_form) {
    selection_row = 0;
    selection_col = 0;
    wclear(login_form->window);
    wrefresh(login_form->window);
    free_login_form(login_form);
}

/*
 * Helpers
 */

LOGIN_FORM* create_login_form(WINDOW* form_window) {

    LOGIN_FORM* login_form = init_login_form(form_window);

    if (login_form == NULL)
        return NULL;

    print_login_fields(login_form, TRUE);
    move_cursor_to_end_of_input_field(login_form, 0);

    wrefresh(login_form->window);

    return login_form;
}
