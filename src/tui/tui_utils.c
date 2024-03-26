#include <tui/tui_utils.h>
#include <string.h>

static int g_last_y, g_last_x;

void store_last_pos(WINDOW* window) {
    getyx(window, g_last_y, g_last_x);
}

void restore_last_pos(WINDOW* window) {
    wmove(window, g_last_y, g_last_x);
}

int get_offset_for_centered(int length, int total_length) {
    return (total_length  - length) / 2;
}

int get_centered_x_start(WINDOW* window, int length) {
    return get_offset_for_centered(length, getmaxx(window));
}

int get_centered_y_start(WINDOW* window, int length) {
    return get_offset_for_centered(length, getmaxy(window));
}

bool confirmation_menu(const char* message) {
    WINDOW* confirmation_window;
    int exit = FALSE, selection = CONFIRMATION_DEFAULT_OPTION;

    confirmation_window = newwin(
            CONFIRMATION_WINDOW_HEIGHT,
            CONFIRMATION_WINDOW_WIDTH,
            get_centered_y_start(stdscr, CONFIRMATION_WINDOW_HEIGHT),
            get_centered_x_start(stdscr, CONFIRMATION_WINDOW_WIDTH)
    );
    keypad(confirmation_window, TRUE);  // enable arrow keys

    curs_set(0);  // hide cursor

    // border
    box(confirmation_window, 0, 0);

    // message
    mvwaddstr(confirmation_window,
              1,
              get_centered_x_start(confirmation_window, (int)strlen(message)),
              message);

    // display options
    int option_start_y = CONFIRMATION_WINDOW_HEIGHT - 2;
    int option_start_x = get_centered_x_start(confirmation_window, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH);
    mvwaddstr(confirmation_window,
              option_start_y,
              option_start_x,
              "Yes  No");

    while (!exit) {
        // clear highlight
        mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH + CONFIRMATION_NO_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, 0, 0, NULL);

        if (selection)  // highlight yes
            mvwchgat(confirmation_window, option_start_y, option_start_x, CONFIRMATION_YES_LENGTH, A_STANDOUT, 0, NULL);
        else  // highlight no
            mvwchgat(confirmation_window, option_start_y, option_start_x + CONFIRMATION_YES_LENGTH + CONFIRMATION_OPTION_GAP_LENGTH, CONFIRMATION_NO_LENGTH, A_STANDOUT, 0, NULL);
        wrefresh(confirmation_window);

        switch (wgetch(confirmation_window)) {
            case KEY_LEFT:
                selection = TRUE;
                break;
            case KEY_RIGHT:
                selection = FALSE;
                break;
            case KEY_ENTER:
            case KEY_ESC:
            case '\n':
            case '\r':
                exit = TRUE;
                break;
            default:
                break;
        }
    }

    wclear(confirmation_window);
    delwin(confirmation_window);

    curs_set(1);  // return cursor
    return !selection;
}