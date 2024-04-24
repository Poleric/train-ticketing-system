#include <tui/menu_utils.h>

void clear_current_menu(WINDOW* menu_window, FORM* form) {
    reset_form(form);
    wclear(form->window);
    wclear(menu_window);
}

void print_login_menu(const char* header_text, WINDOW* menu_window, FORM* login_form) {
    // header
    mvwprintw(menu_window, 0, get_centered_x_start(menu_window, (int)strlen(header_text)), "%s", header_text);
    wrefresh(menu_window);
    // form
    print_login_form(login_form);
    wrefresh(login_form->window);
}