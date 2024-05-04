#include <tui/menu/menu.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void init_menu(menu_t* menu, WINDOW* menu_window, short default_color_pair, attr_t selected_attr, short selected_color_pair) {
    getmaxyx(menu_window, menu->height, menu->width);

    menu->selected_option = 0;

    menu->window = menu_window;

    menu->default_color_pair = default_color_pair;
    menu->selected_attr = selected_attr;
    menu->selected_color_pair = selected_color_pair;

    curs_set(0);
    keypad(menu->window, true);
}

void unhighlight_selected_option(menu_t* menu) {
    assert(menu->selected_option < menu->number_of_options);

    menu_option_t* option = menu->menu_options + menu->selected_option;
    int start_x = get_option_start_x(menu, option);

    mvwaddch(menu->window, option->y, start_x - 2, ' ');
    mvwchgat(menu->window, option->y, start_x, strlen(option->label), A_NORMAL, menu->default_color_pair, NULL);
}

void highlight_selected_option(menu_t* menu) {
    assert(menu->selected_option < menu->number_of_options);

    menu_option_t* option = menu->menu_options + menu->selected_option;
    int start_x = get_option_start_x(menu, option);

    mvwaddch(menu->window, option->y, start_x - 2, '>');
    mvwchgat(menu->window, option->y, start_x, strlen(option->label), menu->selected_attr, menu->selected_color_pair, NULL);
}

int get_option_start_x(menu_t* menu, menu_option_t* option) {
    if (option->x == CENTERED_X)
        return get_centered_x_start(menu->window, (int)strlen(option->label));
    return option->x;
}

void select_option(menu_t* menu, int new_option) {
    assert(new_option < menu->number_of_options);

    unhighlight_selected_option(menu);

    menu->selected_option = new_option;

    highlight_selected_option(menu);
}

void print_options(menu_t* menu) {
    for (int i = 0; i < menu->number_of_options; i++) {
        menu_option_t* option = menu->menu_options + i;
        int start_x = get_option_start_x(menu, option);
        mvwprintw(menu->window, option->y, start_x, "%s", option->label);
    }
}

form_action_t menu_driver(menu_t* menu, int ch) {
    switch (ch) {
        case 'q':
        case CTRL('C'):
            if (confirmation_menu(menu->window, "Logout?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;
            return REFRESH_SCREEN_ACTION;

        case KEY_UP:
            if (menu->selected_option > 0)
                select_option(menu, menu->selected_option - 1);
            break;

        case KEY_DOWN:
            if (menu->selected_option < menu->number_of_options - 1)
                select_option(menu, menu->selected_option + 1);
            break;

        case KEY_ENTER:
        case '\n':
        case '\r':
            return SUBMIT_ACTION;

        default:
            break;
    }
    wrefresh(menu->window);

    return NO_ACTION;
}

void cleanup_menu(menu_t* menu) {
    wclear(menu->window);
    wrefresh(menu->window);

    curs_set(1);
    keypad(menu->window, false);
}