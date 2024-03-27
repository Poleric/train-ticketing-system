#include <tui/main_menu.h>
#include <tui/member_menu.h>
#include <tui/staff_menu.h>

void init_color_pairs() {
    /* Initialize preset colors pairs
     * 1 - Green background
     * 2 - Cyan background
     * 3 - Yellow background
     */
    use_default_colors();
//    assume_default_colors(-1, -1);
//    init_pair(1, COLOR_GREEN, COLOR_BLACK);
//    init_pair(2, COLOR_CYAN, COLOR_BLACK);
//    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
}

void start_menu() {
    initscr();
    start_color(); init_color_pairs();
    raw();  // get input by chars
    noecho();  // don't display input
    keypad(stdscr, TRUE);

    current_menu_t menu = MEMBER_MENU;
    while (menu) {
        switch (menu) {
            case MEMBER_MENU:
                menu = member_menu();
                break;
            case STAFF_MENU:
                menu = staff_menu();
                break;
            default:
                break;
        }
    }

    endwin();
}






