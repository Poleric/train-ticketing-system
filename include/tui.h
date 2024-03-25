#include <stdlib.h>
#include <staff.h>
#include <member.h>

#ifndef TRAINTICKETINGSYSTEM_TUI_H
#define TRAINTICKETINGSYSTEM_TUI_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <curses.h>
#else
#include <ncursesw/ncurses.h>
#endif

int start_menu();
int display_member_login();
int display_member_menu(struct Staff current_staff);
int display_staff_login();
int display_staff_menu(struct Member current_member);


#endif //TRAINTICKETINGSYSTEM_TUI_H
