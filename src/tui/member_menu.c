#include <tui/member_menu.h>
#include <tui/template/login_menu.h>
#include <stdlib.h>

void after_login(char *username, char* password) {
    printw("Logged in as: %s, with password: %s", username, password);
}


current_menu_t member_menu(WINDOW* menu_window) {
    keypad(menu_window, TRUE);

    LOGIN_FORM* login_form = create_login_form(menu_window);

    bool exit = FALSE;
    login_form_action_t action;
    char* username, *password;
    while (!exit) {
        int ch = wgetch(menu_window);
        action = form_driver(login_form, ch);
        switch (action) {
            case SUBMIT_ACTION:
                if ((strcmp(login_form->field_buffers[0], "abc") == 0 && strcmp(login_form->field_buffers[1], "abc") == 0) ||
                    (strcmp(login_form->field_buffers[0], "MINORI") == 0 && strcmp(login_form->field_buffers[1], "MINORI") == 0)) {
                    username = strdup(login_form->field_buffers[0]);
                    password = strdup(login_form->field_buffers[1]);
                    exit = TRUE;
                } else {
                    store_last_pos(menu_window);
                    mvwprintw(menu_window, 3, 0, "Wrong password");
                    restore_last_pos(menu_window);
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
        if (strcmp(username, "MINORI") == 0)
            addstr(MINORI);
        else
            after_login(username, password);
        refresh();
        getch();
        free(username);
        free(password);
    } else if (action == SWITCH_MENU_ACTION) {
        printw("Switched menu");
        refresh();
        getch();
        return STAFF_MENU;
    } else if (action == EXIT_FORM_ACTION) {
        return EXIT_MENU;
    }

    keypad(menu_window, FALSE);
    return MEMBER_MENU;
}
