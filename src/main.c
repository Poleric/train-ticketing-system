#include <tui/template/login_menu.h>
#include <stdlib.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define strdup _strdup
#endif

void after_login(char *username, char* password) {
    printw("Logged in as: %s, with password: %s", username, password);
}


int main() {
    initscr();
    raw();
    noecho();

    WINDOW* login_win = derwin(
            stdscr,
            10,
            50,
            0,
            get_centered_x_start(stdscr, 50)
            );
    keypad(login_win, TRUE);

    LOGIN_FORM* login_form = create_login_form(login_win);

    bool exit = FALSE;
    login_form_action_t action;
    char* username, *password;
    while (!exit) {
        int ch = wgetch(login_win);
        action = form_driver(login_form, ch);
        switch (action) {
            case SUBMIT_ACTION:
                if (strcmp(login_form->field_buffers[0], "abc") == 0 && strcmp(login_form->field_buffers[1], "abc") == 0) {
                    username = strdup(login_form->field_buffers[0]);
                    password = strdup(login_form->field_buffers[1]);
                    exit = TRUE;
                } else {
                    store_last_pos(login_win);
                    mvwprintw(login_win, 3, 0, "Wrong password");
                    restore_last_pos(login_win);
                }
                break;

            case SWITCH_MENU_ACTION:
            case EXIT_FORM_ACTION:
                exit = TRUE;
                break;

            default:
                break;
        }
    }

    cleanup_login_form(login_form);

    if (action == SUBMIT_ACTION) {
        after_login(username, password);
        refresh();
        getch();
        free(username);
        free(password);
    } else if (action == SWITCH_MENU_ACTION) {
        printw("Switched menu");
        refresh();
        getch();
    }

    delwin(login_win);
    endwin();

    return 0;
}
