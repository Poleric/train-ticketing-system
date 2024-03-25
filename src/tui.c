#include <tui.h>


int start_menu() {
    initscr();
    cbreak();  // get input by chars
    keypad(stdscr, TRUE);

    getch();

    endwin();

    return EXIT_SUCCESS;
}

int display_member_login() {
    return EXIT_SUCCESS;
}


int display_member_menu(struct Staff current_staff) {
    return EXIT_SUCCESS;
}

int display_staff_login() {
    return EXIT_SUCCESS;

}


int display_staff_menu(struct Member current_member) {
    return EXIT_SUCCESS;
}


