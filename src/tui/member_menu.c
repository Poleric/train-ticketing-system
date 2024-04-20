#include <tui/member_menu.h>
#include <tui/template/login_menu.h>
#include <member.h>

// member_login -> member_menu   --┐
//   / \        -> register_menu --|
//    └----------------------------┘
current_menu_t member_login_menu(WINDOW* menu_window) {
    keypad(menu_window, TRUE);

    LOGIN_FORM* login_form = create_login_form(menu_window);

    current_menu_t current_menu = MEMBER_MENU;

    member_vector_t* members = init_members_vector();
    load_members(members);

    member_t* current_member;
    while (current_menu == MEMBER_MENU) {
        int ch = wgetch(menu_window);

        switch (form_driver(login_form, ch)) {
            case SUBMIT_ACTION:
                current_member = login_as(members, get_username(login_form), get_password(login_form));
                if (current_member) {

                    reset_login_form(login_form);
                    wclear(menu_window);

                    current_menu = member_menu(menu_window, current_member);

                    print_form(login_form);
                    wrefresh(menu_window);
                }
                else {
                    store_last_pos(menu_window);
                    mvwprintw(menu_window, 3, 0, "Wrong username or password");
                    restore_last_pos(menu_window);
                }
                break;

            case SWITCH_MENU_ACTION:
                current_menu = STAFF_MENU;
                break;

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
                for (int i = 0; i < members->num_of_members; i++)
                    free_member(members->array[i]);
                load_members(members);
                break;

            default:
                break;
        }
    }

    cleanup_login_form(login_form);
    free_members_vector(members);

    keypad(menu_window, FALSE);
    return current_menu;
}

current_menu_t member_menu(WINDOW* menu_window, member_t* member) {
    current_menu_t current_menu = MEMBER_MENU;

    wprintw(menu_window, "Welcome %s", member->username);
    wrefresh(menu_window);

    if (wgetch(menu_window) == KEY_ESC)
        current_menu = EXIT_MENU;

    wclear(menu_window);
    return current_menu;
}
