#include <tui/template/form.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <utils.h>
#include <assert.h>


FORM* init_form(WINDOW* form_window, int n_buffer, int label_field_length) {
    FORM* form = malloc(sizeof (FORM));

    if (form == NULL)
        return NULL;

    form->window = form_window;
    getmaxyx(form_window, form->LOGIN_FORM_LINES, form->LOGIN_FORM_COLS);
    form->selection_col = form->selection_row = 0;
    form->buffer_length = form->LOGIN_FORM_COLS - label_field_length;
    assert(form->buffer_length > 0);

    form->field_start_x = label_field_length;
    form->n_buffer = n_buffer;
    form->field_buffers = calloc(sizeof(char*), form->n_buffer);
    for (int i = 0; i < form->n_buffer; i++)
        form->field_buffers[i] = calloc(sizeof (char), form->buffer_length);

    return form;
}

void move_cursor_to_input_field(FORM* form, int field_n, int pos) {
    /*
     * field_n: 0 - username field, 1 - password field
     */
    int y, x;

    y = field_n;
    x = form->field_start_x + pos;

    wmove(form->window, y, x);
}

int move_cursor_to_end_of_input_field(FORM* form, int field_n) {
    int pos = (int)strlen(form->field_buffers[field_n]);
    move_cursor_to_input_field(form, field_n, pos);
    return pos;
}

void print_input_field_buffer(FORM* form, int field_n) {
    int y, x;

    y = field_n;
    x = form->field_start_x;

    wmove(form->window, y, x); wclrtoeol(form->window);
    mvwprintw(form->window, y, x, "%s", form->field_buffers[field_n]);
    wmove(form->window, y, x); wchgat(form->window, -1, A_UNDERLINE, 0, 0);
}

form_action_t form_driver(FORM* form, int ch) {
    switch (ch) {
        case KEY_F(1):
            return SWITCH_MENU_ACTION;

        case KEY_F(2):
            return RELOAD_ACTION;

        case KEY_ESC:
        case CTRL('C'):
            store_last_pos(form->window);
            if (confirmation_menu(form->window, "Exit Menu?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;

            restore_last_pos(form->window);
            break;

        case CTRL('R'):
            return REGISTER_ACTION;

        case KEY_DOWN:
            if (form->selection_row < 1) {
                form->selection_row++;
                form->selection_col = move_cursor_to_end_of_input_field(form, form->selection_row);
            }
            break;

        case KEY_UP:
            if (form->selection_row > 0) {
                form->selection_row--;
                form->selection_col = move_cursor_to_end_of_input_field(form, form->selection_row);
            }
            break;

        case KEY_RIGHT:
            if (form->selection_col < strlen(form->field_buffers[form->selection_row])) {
                form->selection_col++;
                move_cursor_to_input_field(form, form->selection_row, form->selection_col);
            }
            break;

        case KEY_LEFT:
            if (form->selection_col > 0) {
                form->selection_col--;
                move_cursor_to_input_field(form, form->selection_row, form->selection_col);
            }
            break;

        case KEY_ENTER:
        case '\n':
        case '\r':
            if (form->selection_row < 1) {  // same as key down
                form->selection_row++;
                form->selection_col = move_cursor_to_end_of_input_field(form, form->selection_row);
            }
            else {  // submit
                return SUBMIT_ACTION;
            }
            break;

        case KEY_BACKSPACE:
        case 127:
        case '\b':
            if (form->selection_col > 0) {
                delete_char(form->field_buffers[form->selection_row], --form->selection_col);
                print_input_field_buffer(form, form->selection_row);
                move_cursor_to_input_field(form, form->selection_row, form->selection_col);
            }
            break;

        case KEY_DC:
            if ((strlen(form->field_buffers[form->selection_row]) - form->selection_col) > 0) {
                delete_char(form->field_buffers[form->selection_row], form->selection_col);
                print_input_field_buffer(form, form->selection_row);
                move_cursor_to_input_field(form, form->selection_row, form->selection_col);
            }
            break;

        default:
            if (isprint(ch))
                if ((strlen(form->field_buffers[form->selection_row]) < form->buffer_length)) {
                    add_char(form->field_buffers[form->selection_row], (char)ch, form->selection_col++);
                    print_input_field_buffer(form, form->selection_row);
                    move_cursor_to_input_field(form, form->selection_row, form->selection_col);
                }
            break;
    }
    wrefresh(form->window);

    return NO_ACTION;
}

void free_form(FORM* form) {
    for (int i = 0; i < form->n_buffer; i++)
        free(form->field_buffers[i]);
    free(form->field_buffers);
    free(form);
}

void clear_field_buffers(FORM* login_form, int field_index) {
    assert(field_index < login_form->n_buffer);
    for (int i = 0; i < login_form->buffer_length; i++)
        login_form->field_buffers[field_index][i] = 0;
}

void reset_form(FORM* form) {
    form->selection_row = 0;
    form->selection_col = 0;
    for (int i = 0; i < form->n_buffer; i++)
        clear_field_buffers(form, i);
}

void cleanup_form(FORM* form) {
    reset_form(form);
    wclear(form->window);
    wrefresh(form->window);
    free_form(form);
}