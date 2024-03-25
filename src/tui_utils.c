#include <tui.h>

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
