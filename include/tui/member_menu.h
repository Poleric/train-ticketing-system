#ifndef TRAINTICKETINGSYSTEM_MEMBER_MENU_H
#define TRAINTICKETINGSYSTEM_MEMBER_MENU_H

#include <tui/utils/menu_utils.h>
#include <member.h>

current_menu_t member_login_menu(WINDOW* menu_window);
current_menu_t member_menu(WINDOW* menu_window, member_t* member);
void member_registration_menu(WINDOW* menu_window, member_vector_t* members);

#endif //TRAINTICKETINGSYSTEM_MEMBER_MENU_H
