#ifndef TRAINTICKETINGSYSTEM_MENU_H
#define TRAINTICKETINGSYSTEM_MENU_H

#include <tui/form/form.h>

typedef struct MenuOption menu_option_t;
typedef struct Menu menu_t;

enum {
    CENTERED_X = -1
};

struct MenuOption {
    const char* label;
    int y, x;  // x = -1 mean centered
};


struct Menu {
    int width, height;

    int number_of_options;
    menu_option_t* menu_options;

    int selected_option;

    short default_color_pair;
    attr_t selected_attr;
    short selected_color_pair;

    WINDOW* window;
};

void init_menu(menu_t* menu, WINDOW* menu_window, short default_color_pair, attr_t selected_attr, short selected_color_pair);
void highlight_selected_option(menu_t* menu, attr_t attr, short color_pair);

int get_option_start_x(menu_t* menu, menu_option_t* option);
void select_option(menu_t* menu, int new_option);

void print_options(menu_t* menu);
form_action_t menu_driver(menu_t* menu, int ch);
void cleanup_menu(menu_t* menu);

#endif //TRAINTICKETINGSYSTEM_MENU_H
