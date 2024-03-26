#include <tui/member_menu.h>
#include <string.h>
#include <stdlib.h>


current_menu_t member_menu() {
    enum CurrentMenu return_menu = MEMBER_MENU;

    FIELD* fields[3] = {
            new_field(1, 20, 2, LOGIN_FIELD_OFFSET_X + 2, 0, 0),
            new_field(1, 20, 3, LOGIN_FIELD_OFFSET_X + 2, 0, 0)
    };

    FORM* login_form = new_form(fields);

    set_field_back(fields[0], A_UNDERLINE);
    field_opts_off(fields[0], O_AUTOSKIP);
    set_field_back(fields[1], A_UNDERLINE);
    field_opts_off(fields[1], O_AUTOSKIP);
    field_opts_off(fields[1], O_PUBLIC);

    post_form(login_form);
    mvprintw(1, get_centered_x_start(stdscr, 11), "Member Login");
    box(stdscr, 0, 0);
    mvprintw(2, 2, LOGIN_USERNAME_LABEL);
    mvprintw(3, 2, LOGIN_PASSWORD_LABEL);
    move(2, LOGIN_FIELD_OFFSET_X + 2);
    refresh();

    int selection_index = 0, ch;
    while (return_menu == MEMBER_MENU) {
        ch = getch();
        switch (ch) {
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
            case KEY_ENTER:
            case '\n':
            case '\r':
                if (selection_index < 1) {
                    form_driver(login_form, REQ_NEXT_FIELD);
                    form_driver(login_form, REQ_END_LINE);
                    selection_index++;
                }
                else {
                    store_last_pos(stdscr);
                    // clear line
                    mvprintw(5, 2, "                                ");

                    // store input
                    form_driver(login_form, REQ_VALIDATION);

                    if (strcmp(field_buffer(fields[1], 0), "abc\n") == 0) {
                        mvprintw(5, 2, "Login successful");
                    }
                    else {
                        mvprintw(5, 2, "Wrong password");
                    }
                    refresh();

                    restore_last_pos(stdscr);
                }
                break;
            case KEY_ESC:
            case CTRL('C'):
                store_last_pos(stdscr);
                if (confirmation_menu("Exit Menu?") == EXIT_SUCCESS)
                    return_menu = EXIT;
                restore_last_pos(stdscr);
                break;
            case KEY_BACKSPACE:
                form_driver(login_form, REQ_DEL_PREV);
                break;
            case KEY_F(1):
                return_menu = STAFF_MENU;
                break;
            default:
                form_driver(login_form, ch);
                break;
        }
    }

    unpost_form(login_form);
    free_form(login_form);
    for (int i = 0; ;i++) {
        if (!fields[i])
            break;
        free_field(fields[i]);
    }

    return return_menu;
}