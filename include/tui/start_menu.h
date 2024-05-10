#ifndef TRAINTICKETINGSYSTEM_TUI_H
#define TRAINTICKETINGSYSTEM_TUI_H

#include <tui/utils/tui_utils.h>
#include <tui/utils/menu_utils.h>
#include <schedule.h>
#include <member.h>
#include <staff.h>

#define TITLE "Hambalang Star Rail"

void reload_members(member_vector_t* members);
void reload_staff(staff_vector_t* staff);

void init_color_pairs();
void start_menu();

current_menu_t member_login_menu(WINDOW* menu_window);
void member_menu(WINDOW* menu_window, member_vector_t* members, member_t* member);
void view_member_details_menu(WINDOW* menu_window, member_vector_t* members, member_t* member);
void member_registration_menu(WINDOW* menu_window, member_vector_t* members);
#ifdef EMAIL_RECOVERY_FEATURE
void member_forgot_password(WINDOW* menu_window, member_vector_t* members);
#endif
void member_feedback_form(WINDOW* menu_window, member_t* member);

current_menu_t staff_login_menu(WINDOW* menu_window);
void staff_menu(WINDOW* menu_window, staff_vector_t* staffs, staff_t* staff);
void staff_registration_menu(WINDOW* menu_window, staff_vector_t* staffs);

void view_schedule_menu(WINDOW* menu_window, member_t* member);

void view_ticket_menu(WINDOW* menu_window, member_t* member);

void view_seat_menu(WINDOW* menu_window, member_t* member, dt_date_t date, schedule_t* schedule);

#endif //TRAINTICKETINGSYSTEM_TUI_H
