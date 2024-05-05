#ifndef TRAINTICKETINGSYSTEM_INT_MENU_H
#define TRAINTICKETINGSYSTEM_INT_MENU_H

#include <tui/menu/menu.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct IntMenuOption int_menu_option_t;
typedef struct IntMenu int_menu_t;

struct IntMenuOption {
    int label;
    int y, x;  // x = -1 mean centered
    bool disabled;
    bool selected;
};


struct IntMenu {
    int width, height;

    int number_of_options;
    int_menu_option_t* menu_options;

    int options_per_row;
    int rows_per_page;

    int selected_option;

    short default_color_pair;
    attr_t selected_attr;
    short selected_color_pair;

    WINDOW* window;
};

void init_int_menu(int_menu_t* menu, WINDOW* menu_window, short default_color_pair, attr_t selected_attr, short selected_color_pair);

void unhighlight_int_menu_selected_option(int_menu_t* menu);
void highlight_int_menu_selected_option(int_menu_t* menu);
int get_int_menu_option_start_x(int_menu_t* menu, int_menu_option_t* option);
void select_int_menu_option(int_menu_t* menu, int new_option);
void print_int_menu_options(int_menu_t* menu);

int get_previous_not_disabled_int_option(int_menu_t* menu, size_t offset, size_t step);
int get_next_not_disabled_int_option(int_menu_t* menu, size_t offset, size_t step);

form_action_t int_menu_driver(int_menu_t* menu, int ch);

int get_current_page(int_menu_t* menu);

void cleanup_int_menu(int_menu_t* menu);

#endif //TRAINTICKETINGSYSTEM_INT_MENU_H
