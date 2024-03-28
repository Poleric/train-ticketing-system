#include <tui/template/login_menu.h>
#include <string.h>
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

    WINDOW* login_win = derwin(stdscr, LINES, COLS, 0, get_centered_x_start(stdscr, COLS));
    FORM* login_form = init_login_form(login_win, "Member login", TRUE, 3);

    char *username, *password;
    login_form_action_t action;
    bool exit = FALSE;
    while (!exit) {
        action = login_form_driver(login_form, wgetch(form_sub(login_form)), &username, &password);
        switch (action) {
            case LOGIN_ACTION:
                if (strcmp(username, "abc") == 0 && strcmp(password, "abc") == 0) {
                    username = strdup(username);
                    password = strdup(password);
                    exit = TRUE;
                } else
                    print_message_below_form(login_form, "Wrong password", 1, 0, FALSE);
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

    if (action == LOGIN_ACTION) {
        after_login(username, password);
        refresh();
        getch();
        free(username);
        free(password);
    }
    else if (action == SWITCH_MENU_ACTION) {
        printw("Switched menu");
        refresh();
        getch();
    }

    delwin(login_win);
    endwin();
    return 0;
}

