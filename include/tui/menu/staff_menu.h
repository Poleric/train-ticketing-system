#ifndef STAFF_MENU_H
#define STAFF_MENU_H

#include <tui/menu/menu.h>

typedef struct StaffMenu staff_menu_t;

struct StaffMenu {
    menu_t menu;

    const char* header;
    int header_y;

    char* menu_header;
    int menu_header_y;

    const char** footers;
    int* footer_widths;
    int number_of_footers;
    int footer_y;
};

void init_staff_menu(staff_menu_t* staff_menu, WINDOW* menu_window, const char* header, char* menu_header);

void scale_staff_menu_to_window(staff_menu_t* staff_menu);

void print_staff_menu_header(staff_menu_t* staff_menu, short color_pair);
void print_staff_menu_menu_header(staff_menu_t* staff_menu, short color_pair);
void print_staff_menu_footer(staff_menu_t* staff_menu, short color_pair);

void display_staff_menu(staff_menu_t* staff_menu, short color_pair);

void free_staff_menu(staff_menu_t* staff_menu);

#endif //STAFF_MENU_H
