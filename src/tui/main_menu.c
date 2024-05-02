#include <tui/main_menu.h>
#include <tui/member_menu.h>
#include <tui/staff_menu.h>
#include <locale.h>

void init_color_pairs() {
    /* Initialize preset colors pairs
     * 1 - Green background
     * 2 - Cyan background
     * 3 - Yellow background
     */
    use_default_colors();
    assume_default_colors(-1, -1);
    init_pair(COLOR_1, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_2, COLOR_CYAN, COLOR_BLACK);
    init_pair(SELECTED, COLOR_YELLOW, COLOR_BLACK);
    init_pair(ERROR, COLOR_WHITE, COLOR_RED);
}

void init_menu() {
    setlocale(LC_ALL, "");
    initscr();
    start_color(); init_color_pairs();
    raw();  // get input by chars
    noecho();  // don't display input
}

void start_menu() {
    init_menu();

    current_menu_t current_menu = MEMBER_MENU;
    while (current_menu != EXIT_MENU) {
        switch (current_menu) {
            case MEMBER_MENU:
                current_menu = member_login_menu(stdscr);
                break;
            case STAFF_MENU:
                current_menu = staff_login_menu(stdscr);
                break;
            default:
                break;
        }
    }

    endwin();
}






