#ifndef TRAINTICKETINGSYSTEM_TUI_H
#define TRAINTICKETINGSYSTEM_TUI_H

#include <tui/utils/tui_utils.h>
#include <tui/utils/menu_utils.h>
#include <member.h>
#include <staff.h>

void init_color_pairs();

void reload_staff(staff_vector_t* staff);

void init_menu();
void start_menu();

current_menu_t member_login_menu(WINDOW* menu_window);
void member_menu(WINDOW* menu_window, member_t* member);
void member_registration_menu(WINDOW* menu_window, member_vector_t* members);

current_menu_t staff_login_menu(WINDOW* menu_window);
current_menu_t staff_menu(WINDOW* menu_window, staff_t* current_staff);

void schedule_menu();


#endif //TRAINTICKETINGSYSTEM_TUI_H
