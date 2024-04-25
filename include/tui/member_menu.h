#ifndef TRAINTICKETINGSYSTEM_MEMBER_MENU_H
#define TRAINTICKETINGSYSTEM_MEMBER_MENU_H

#include "tui/utils/tui_utils.h"
#include "tui/utils/tui_common.h"
#include <member.h>

current_menu_t member_login_menu(WINDOW* menu_window);
current_menu_t member_menu(WINDOW* menu_window, member_t* member);

#endif //TRAINTICKETINGSYSTEM_MEMBER_MENU_H
