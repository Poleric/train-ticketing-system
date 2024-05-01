#ifndef TRAINTICKETINGSYSTEM_STAFF_MENU_H
#define TRAINTICKETINGSYSTEM_STAFF_MENU_H

#include <tui/utils/menu_utils.h>
#include <staff.h>

current_menu_t staff_login_menu(WINDOW* menu_window);
current_menu_t staff_menu(WINDOW* menu_window, staff_t* current_staff);

#endif //TRAINTICKETINGSYSTEM_STAFF_MENU_H
