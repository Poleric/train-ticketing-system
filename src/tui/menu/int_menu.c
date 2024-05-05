#include <tui/menu/int_menu.h>
#include <tui/utils/menu_utils.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <utils.h>

void init_int_menu(int_menu_t* menu, WINDOW* menu_window, short default_color_pair, attr_t selected_attr, short selected_color_pair) {
    getmaxyx(menu_window, menu->height, menu->width);

    menu->selected_option = 0;

    menu->window = menu_window;

    menu->default_color_pair = default_color_pair;
    menu->selected_attr = selected_attr;
    menu->selected_color_pair = selected_color_pair;

    curs_set(0);
    keypad(menu->window, true);
}

void unhighlight_int_menu_selected_option(int_menu_t* menu) {
    assert(menu->selected_option < menu->number_of_options);

    int_menu_option_t* option = menu->menu_options + menu->selected_option;
    int start_x = get_int_menu_option_start_x(menu, option);

    attr_t attr = A_NORMAL;
    short color_pair = menu->default_color_pair;
    if (option->disabled) {
        attr = A_STANDOUT;
        color_pair = ERROR;
    }
    if (option->selected)
        color_pair = menu->selected_color_pair;

    mvwchgat(menu->window, option->y, start_x, get_number_of_digits_d(option->label), attr, color_pair, NULL);
}

void highlight_int_menu_selected_option(int_menu_t* menu) {
    assert(menu->selected_option < menu->number_of_options);

    int_menu_option_t* option = menu->menu_options + menu->selected_option;
    int start_x = get_int_menu_option_start_x(menu, option);

    mvwchgat(menu->window, option->y, start_x, get_number_of_digits_d(option->label), menu->selected_attr, menu->selected_color_pair, NULL);
}

int get_int_menu_option_start_x(int_menu_t* menu, int_menu_option_t* option) {
    if (option->x == CENTERED_X)
        return get_centered_x_start(menu->window, get_number_of_digits_d(option->label));
    return option->x;
}

void select_int_menu_option(int_menu_t* menu, int new_option) {
    assert(new_option < menu->number_of_options);

    unhighlight_int_menu_selected_option(menu);

    menu->selected_option = new_option;

    highlight_int_menu_selected_option(menu);
}

void print_int_menu_options(int_menu_t* menu) {
    const int grid_per_page = menu->rows_per_page * menu->options_per_row;
    for (
            int i = menu->selected_option / grid_per_page * grid_per_page;
            i < (menu->selected_option / grid_per_page + 1) * grid_per_page && i < menu->number_of_options;
            i++
            ) {
        int_menu_option_t* option = menu->menu_options + i;
        if (option->disabled)
            wattron(menu->window, COLOR_PAIR(ERROR));
        if (option->selected)
            wattron(menu->window, COLOR_PAIR(menu->selected_color_pair));
        mvwprintw(menu->window, option->y, get_int_menu_option_start_x(menu, option), "%-3d", option->label);
        wattroff(menu->window, COLOR_PAIR(ERROR));
        wattroff(menu->window, COLOR_PAIR(menu->selected_color_pair));
    }
}

int get_previous_not_disabled_int_option(int_menu_t* menu, size_t offset, size_t step) {
    for (int i = menu->selected_option - (int)offset; i >= 0 ; i -= (int)step)
        if (!menu->menu_options[i].disabled)
            return i;
    return -1;
}

int get_next_not_disabled_int_option(int_menu_t* menu, size_t offset, size_t step) {
    for (int i = menu->selected_option + (int)offset; i < menu->number_of_options; i += (int)step)
        if (!menu->menu_options[i].disabled)
            return i;
    return -1;
}

form_action_t int_menu_driver(int_menu_t* menu, int ch) {
    int current_page = get_current_page(menu);
    int i;
    switch (ch) {
        case 'q':
        case CTRL('C'):
            if (confirmation_menu(menu->window, "Logout?") == EXIT_SUCCESS)
                return EXIT_FORM_ACTION;
            return REFRESH_SCREEN_ACTION;

        case KEY_UP:
            i = get_previous_not_disabled_int_option(menu, 4, 1);
            if (i > -1)
                select_int_menu_option(menu, MAX(0, i));
            if (get_current_page(menu) != current_page)
                return REFRESH_SCREEN_ACTION;
            break;

        case KEY_DOWN:
            i = get_next_not_disabled_int_option(menu, 4, 1);
            if (i > -1)
                select_int_menu_option(menu, MIN(menu->number_of_options - 1, i));
            if (get_current_page(menu) != current_page)
                return REFRESH_SCREEN_ACTION;
            break;

        case KEY_LEFT:
            i = get_previous_not_disabled_int_option(menu, 1, 1);
            if (i > -1)
                select_int_menu_option(menu, i);
            if (get_current_page(menu) != current_page)
                return REFRESH_SCREEN_ACTION;
            break;

        case KEY_RIGHT:
            i = get_next_not_disabled_int_option(menu, 1, 1);
            if (i > -1)
                select_int_menu_option(menu, i);
            if (get_current_page(menu) != current_page)
                return REFRESH_SCREEN_ACTION;
            break;

        case 's':
            menu->menu_options[menu->selected_option].selected = !menu->menu_options[menu->selected_option].selected;
            return REFRESH_SCREEN_ACTION;

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

int get_current_page(int_menu_t* menu) {
    return (menu->selected_option + 1) / (menu->rows_per_page * menu->options_per_row) + 1;
}

int get_number_of_selected(int_menu_t* int_menu) {
    int n = 0;
    for (int i = 0; i < int_menu->number_of_options; i++)
        if (int_menu->menu_options[i].selected)
            n++;
    return n;
}

void cleanup_int_menu(int_menu_t* menu) {
    wclear(menu->window);
    wrefresh(menu->window);

    curs_set(1);
    keypad(menu->window, false);
}