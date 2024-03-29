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
    keypad(stdscr, TRUE);

    LOGIN_FORM* login_form = create_login_form(stdscr);

    bool exit = FALSE;
    login_form_action_t action;
    char* username, *password;
    while (!exit) {
        int ch = getch();
        action = form_driver(login_form, ch);
        switch (action) {
            case SUBMIT_ACTION:
                if (strcmp(login_form->field_buffers[0], "abc") == 0 && strcmp(login_form->field_buffers[1], "abc") == 0) {
                    username = strdup(login_form->field_buffers[0]);
                    password = strdup(login_form->field_buffers[1]);
                    exit = TRUE;
                } else {
                    store_last_pos(stdscr);
                    mvprintw(3, 0, "Wrong password");
                    restore_last_pos(stdscr);
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

    endwin();

    return 0;
}
