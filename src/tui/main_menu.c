#include <tui/main_menu.h>
#include <tui/member_menu.h>
#include <tui/staff_menu.h>
#include <string.h>

void init_color_pairs() {
    /* Initialize preset colors pairs
     * 1 - Green background
     * 2 - Cyan background
     * 3 - Yellow background
     */
    use_default_colors();
    assume_default_colors(-1, -1);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}

void init_menu() {
    initscr();
    start_color(); init_color_pairs();
    raw();  // get input by chars
    noecho();  // don't display input
}

void start_menu() {
    init_menu();

    WINDOW* inner_window = newwin(
            LINES - INNER_MENU_MARGIN * 2,
            COLS - INNER_MENU_MARGIN * 2,
            INNER_MENU_MARGIN,
            INNER_MENU_MARGIN);

    current_menu_t menu = MEMBER_MENU;
    while (menu) {
        switch (menu) {
            case MEMBER_MENU:
                menu = member_menu(inner_window);
                break;
            case STAFF_MENU:
                menu = staff_menu(inner_window);
                break;
            default:
                break;
        }
    }

    delwin(inner_window);

    endwin();
}






