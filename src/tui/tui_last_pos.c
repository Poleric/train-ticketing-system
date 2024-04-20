#include <tui/tui_utils.h>

static int g_last_y, g_last_x;

void store_last_pos(WINDOW* window) {
    getyx(window, g_last_y, g_last_x);
}

void restore_last_pos(WINDOW* window) {
    wmove(window, g_last_y, g_last_x);
}
