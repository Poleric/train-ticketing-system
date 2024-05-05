#ifndef TRAINTICKETINGSYSTEM_SEAT_MENU_H
#define TRAINTICKETINGSYSTEM_SEAT_MENU_H

#include <tui/menu/int_menu.h>
#include <schedule.h>
#include <ticket.h>

typedef struct SeatMenu seat_menu_t;

struct SeatMenu {
    int_menu_t menu;

    const char* header;
    int header_y;

    char* menu_header;
    int menu_header_y;

    const char** footers;
    int* footer_widths;
    int number_of_footers;
    int footer_y;

    dt_date_t date;
    schedule_t* schedule;
};

void init_seat_menu(seat_menu_t* seat_menu, WINDOW* menu_window, const char* header, char* menu_header, dt_date_t date, schedule_t* schedule);
void init_seats_locations(seat_menu_t* seat_menu);

void scale_seat_menu_to_window(seat_menu_t* seat_menu);

void print_seat_menu_header(seat_menu_t* seat_menu, short color_pair);
void print_seat_menu_menu_header(seat_menu_t* seat_menu, short color_pair);
void print_seat_menu_footer(seat_menu_t* seat_menu, short color_pair);

void display_seat_menu(seat_menu_t* seat_menu, short color_pair);

int get_current_selected_seat(seat_menu_t* seat_menu);

void free_seat_menu(seat_menu_t* seat_menu);

#endif //TRAINTICKETINGSYSTEM_SEAT_MENU_H
