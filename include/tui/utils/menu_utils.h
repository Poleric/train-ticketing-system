#ifndef TRAINTICKETINGSYSTEM_MENU_UTILS_H
#define TRAINTICKETINGSYSTEM_MENU_UTILS_H

#include "tui/template/login_form.h"

void clear_current_menu(WINDOW* menu_window, FORM* form);
void print_login_menu(const char* header_text, WINDOW* menu_window, FORM* login_form);

#endif //TRAINTICKETINGSYSTEM_MENU_UTILS_H
