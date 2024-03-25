#include <stdlib.h>
#include <staff.h>
#include <member.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <curses.h>
#else
#include <ncursesw/ncurses.h>
#endif

#ifndef TRAINTICKETINGSYSTEM_TUI_H
#define TRAINTICKETINGSYSTEM_TUI_H

#define CTRL(c) ((c) & 0x1f)
#define KEY_ESC 27

typedef enum CurrentMenu {
    EXIT = 0,
    MEMBER_MENU = 1,
    STAFF_MENU = 2
} current_menu_t;

void init_color_pairs();

void start_menu();
current_menu_t member_menu();
current_menu_t staff_menu();

int confirmation_menu(const char* message);

// utils

void store_last_pos(WINDOW* window);
void restore_last_pos(WINDOW* window);
int get_offset_for_centered(int length, int total_length);
int get_centered_x_start(WINDOW* window, int length);
int get_centered_y_start(WINDOW* window, int length);


#endif //TRAINTICKETINGSYSTEM_TUI_H
