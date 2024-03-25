#include <tui.h>
#include <string.h>


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
    cbreak();  // get input by chars
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

current_menu_t member_menu() {
    switch (getch()) {
        case KEY_ESC:
        case CTRL('C'):
            store_last_pos(stdscr);
            if (confirmation_menu("Exit Menu?") == EXIT_SUCCESS)
                return EXIT;
            restore_last_pos(stdscr);
    }
    return MEMBER_MENU;
}

current_menu_t staff_menu() {
    switch (getch()) {
        case KEY_ESC:
        case CTRL('C'):
            if (confirmation_menu("Exit Menu?") == EXIT_SUCCESS)
                return EXIT;
    }
    return STAFF_MENU;
}

int confirmation_menu(const char* message) {
    WINDOW* confirmation_window;

    confirmation_window = newwin(
            CONFIRMATION_WINDOW_HEIGHT,
            CONFIRMATION_WINDOW_WIDTH,
            get_centered_y_start(stdscr, CONFIRMATION_WINDOW_HEIGHT),
            get_centered_x_start(stdscr, CONFIRMATION_WINDOW_WIDTH)
            );
    keypad(confirmation_window, TRUE);  // enable arrow keys
    curs_set(0);  // hide cursor

    // message
    mvwaddstr(confirmation_window,
              0,
              get_centered_x_start(confirmation_window, (int)strlen(message)),
              message);

    // display options
    int option_start_y = CONFIRMATION_WINDOW_HEIGHT - 1;
    int option_start_x = get_centered_x_start(confirmation_window, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH);
    mvwaddstr(confirmation_window,
              option_start_y,
              option_start_x,
              "Yes  No");

    int exit = 0;
    int selection = CONFIRMATION_DEFAULT_OPTION;
    do {
        // clear highlight
        mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, 0, 0, NULL);

        if (selection)  // highlight yes
            mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH, A_STANDOUT, 0, NULL);
        else  // highlight no
            mvwchgat(confirmation_window, option_start_y, option_start_x + CONFIRMATION_YES_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, CONFIRMATION_NO_LENGTH, A_STANDOUT, 0, NULL);
        wrefresh(confirmation_window);

        switch (wgetch(confirmation_window)) {
            case KEY_LEFT:
                selection = 1;
                break;
            case KEY_RIGHT:
                selection = 0;
                break;
            case KEY_ENTER:
            case KEY_ESC:
            case '\n':
            case '\r':
                exit = 1;
                break;
            default:
                break;
        }
    } while (!exit);

    wclear(confirmation_window);
    wrefresh(confirmation_window);
    delwin(confirmation_window);
    curs_set(1);  // return cursor
    return !selection;
}



