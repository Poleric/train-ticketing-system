#include <tui/form/form.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <utils.h>
#include <assert.h>


void init_form(form_t* form, WINDOW* form_window, int n_buffer, int buffer_length) {
    getmaxyx(form_window, form->height, form->width);

    form->buffer_length = buffer_length;

    form->number_of_fields = n_buffer;
    form->fields = calloc(sizeof(form_field_t), form->number_of_fields);
    for (int i = 0; i < form->number_of_fields; i++) {
        form->fields[i].y = 0;
        form->fields[i].x = 0;
        form->fields[i].buffer = NULL;

        form->fields[i].offset_x = 0;
        form->fields[i].buffer = calloc(sizeof (char), form->buffer_length);
        form->fields[i].pos = 0;
    }

    form->field_label_width = 0;

    form->selection_row = 0;

    form->window = form_window;

    keypad(form->window, true);
}

void move_cursor_to_input_field(form_t* form) {
    assert(form->selection_row < form->number_of_fields);

    form_field_t* form_field = form->fields + form->selection_row;
    wmove(form->window, form_field->y, form_field->x + form_field->offset_x + form_field->pos);
}

void move_cursor_to_start_of_input_field(form_t* form) {
    assert(form->selection_row < form->number_of_fields);

    form_field_t* form_field = form->fields + form->selection_row;
    wmove(form->window, form_field->y, form_field->x + form_field->offset_x);
}

void move_cursor_to_end_of_input_field(form_t* form) {
    assert(form->selection_row < form->number_of_fields);

    form_field_t* form_field = form->fields + form->selection_row;
    form_field->pos = (int)strlen(form_field->buffer);
    move_cursor_to_input_field(form);
}

void print_current_field_buffer(form_t* form) {
    assert(form->selection_row < form->number_of_fields);

    move_cursor_to_start_of_input_field(form);
    wclrtoeol(form->window);

    move_cursor_to_start_of_input_field(form);
    wprintw(form->window, "%s", form->fields[form->selection_row].buffer);

    move_cursor_to_start_of_input_field(form);
    wchgat(form->window, (int)form->buffer_length, A_UNDERLINE, 0, 0);
}

void print_current_field_label(form_t* form) {
    assert(form->selection_row < form->number_of_fields);

    form_field_t* form_field = form->fields + form->selection_row;
    assert(form_field->label);

    wmove(form->window, form_field->y, form_field->x);

    wprintw(form->window, "%s", form_field->label);
}

void clear_field_buffers(form_t* form, int n_field) {
    assert(n_field < form->number_of_fields);

    form_field_t* form_field = form->fields + n_field;
    memset(form_field->buffer, 0, form->buffer_length);
    form_field->pos = 0;
}

void backspace_action_field(form_t* form) {
    delete_char(form->fields[form->selection_row].buffer, --form->fields[form->selection_row].pos);
}

void delete_action_field(form_t* form) {
    delete_char(form->fields[form->selection_row].buffer, form->fields[form->selection_row].pos);
}

void add_action_field(form_t* form, int ch) {
    add_char(form->fields[form->selection_row].buffer, (char)ch, form->fields[form->selection_row].pos++);
}

form_action_t form_driver(form_t* form, int ch) {
    switch (ch) {
        case KEY_F(1):
        case CTRL('R'):
            return REGISTER_ACTION;

        #ifdef EMAIL_RECOVERY_FEATURE
        case KEY_F(2):
            return FORGOT_PASSWORD_ACTION;
        #endif

        case KEY_F(3):
            return SWITCH_MENU_ACTION;

        case KEY_F(10):
            return RELOAD_ACTION;

        case KEY_ESC:
        case CTRL('C'):
            if (confirmation_menu(form->window, "Exit Menu?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;
            return REFRESH_SCREEN_ACTION;

        case KEY_UP:
            if (form->selection_row > 0) {
                form->selection_row--;
                move_cursor_to_input_field(form);
            }
            break;

        case KEY_DOWN:
            if (form->selection_row < form->number_of_fields - 1) {
                form->selection_row++;
                move_cursor_to_input_field(form);
            }
            break;

        case KEY_LEFT:
            if (form->fields[form->selection_row].pos > 0) {
                form->fields[form->selection_row].pos--;
                move_cursor_to_input_field(form);
            }
            break;

        case KEY_RIGHT:
            if (form->fields[form->selection_row].pos < strlen(form->fields[form->selection_row].buffer)) {
                form->fields[form->selection_row].pos++;
                move_cursor_to_input_field(form);
            }
            break;

        case KEY_ENTER:
        case '\n':
        case '\r':
            if (form->selection_row < form->number_of_fields - 1) {  // same as key down
                form->selection_row++;
                move_cursor_to_end_of_input_field(form);
            }
            else {  // submit
                return SUBMIT_ACTION;
            }
            break;

        case KEY_BACKSPACE:
        case 127:
        case '\b':
            if (form->fields[form->selection_row].pos > 0) {
                backspace_action_field(form);
                print_current_field_buffer(form);
                move_cursor_to_input_field(form);
            }
            break;

        case KEY_DC:
            if ((strlen(form->fields[form->selection_row].buffer) - form->fields[form->selection_row].pos) > 0) {
                delete_action_field(form);
                print_current_field_buffer(form);
                move_cursor_to_input_field(form);
            }
            break;

        default:
            if (isprint(ch))
                if ((strlen(form->fields[form->selection_row].buffer) < form->buffer_length)) {
                    add_action_field(form, ch);
                    print_current_field_buffer(form);
                    move_cursor_to_input_field(form);
                }
            break;
    }
    wrefresh(form->window);

    return NO_ACTION;
}

void free_form(form_t* form) {
    for (int i = 0; i < form->number_of_fields; i++)
        free(form->fields[i].buffer);
    free(form->fields);
}

void reset_form(form_t* form) {
    form->selection_row = 0;
    for (int i = 0; i < form->number_of_fields; i++)
        clear_field_buffers(form, i);
}

void cleanup_form(form_t* form) {
    reset_form(form);

    wclear(form->window);
    wrefresh(form->window);
    keypad(form->window, false);

    free_form(form);
}