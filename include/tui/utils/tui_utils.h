#ifndef TRAINTICKETINGSYSTEM_TUI_UTILS_H
#define TRAINTICKETINGSYSTEM_TUI_UTILS_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <curses.h>
#else
#include <ncursesw/ncurses.h>
#endif

#define CTRL(c) ((c) & 0x1f)
#define KEY_ESC 27

# define CONFIRMATION_WINDOW_WIDTH 30
# define CONFIRMATION_WINDOW_HEIGHT 5
# define CONFIRMATION_DEFAULT_OPTION FALSE  // TRUE = left, FALSE = right
# define CONFIRMATION_YES_LENGTH 3
# define CONFIRMATION_NO_LENGTH 2
# define CONFIRMATION_OPTION_GAP_LENGTH 2

void store_last_pos(WINDOW* window);
void restore_last_pos(WINDOW* window);
int get_offset_for_centered(int length, int total_length);
int get_centered_x_start(WINDOW* window, int length);
int get_centered_y_start(WINDOW* window, int length);
void wadd_chars(WINDOW* window, int ch, int n);
char* trim_whitespaces(char *str);

bool confirmation_menu(WINDOW* window, const char* message);

void move_to_next_line(WINDOW* window, int x);
void move_to_x(WINDOW* window, int x);
void move_offset_x(WINDOW* window, int offset_x);

#endif //TRAINTICKETINGSYSTEM_TUI_UTILS_H
