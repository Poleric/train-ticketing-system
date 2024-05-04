#ifndef TRAINTICKETINGSYSTEM_MEMBER_MENU_H
#define TRAINTICKETINGSYSTEM_MEMBER_MENU_H

#include <tui/menu/menu.h>

typedef struct MemberMenu member_menu_t;

struct MemberMenu {
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

void init_member_menu(member_menu_t* member_menu, WINDOW* menu_window, const char* header, char* menu_header);

void scale_member_menu_to_window(member_menu_t* member_menu);

void print_member_menu_header(member_menu_t* member_menu, short color_pair);
void print_member_menu_menu_header(member_menu_t* member_menu, short color_pair);
void print_member_menu_footer(member_menu_t* member_menu, short color_pair);

void display_member_menu(member_menu_t* member_menu, short color_pair);

void free_member_menu(member_menu_t* member_menu);

#endif //TRAINTICKETINGSYSTEM_MEMBER_MENU_H
