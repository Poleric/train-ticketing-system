#include <tui/member_menu.h>
#include <tui/form/login_form.h>
#include <tui/utils/menu_utils.h>

void reload_members(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    members->num_of_members = 0;
    load_members(members);
}

// member_login -> member_menu   --┐
//   / \        -> register_menu --|
//    └----------------------------┘
current_menu_t member_login_menu(WINDOW* menu_window) {
    WINDOW* login_window;
    login_form_t login_form;

    member_vector_t* members;
    member_t* current_member;

    login_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_login_form(&login_form, login_window, "Login as Member");
    display_login_form(&login_form);

    members = init_members_vector();
    load_members(members);

    current_menu_t current_menu = MEMBER_MENU;
    while (current_menu == MEMBER_MENU) {
        switch (form_driver(&login_form.form, wgetch(login_window))) {
            case SUBMIT_ACTION:
                current_member = login_as_member(members, get_username(&login_form), get_password(&login_form));
                if (current_member) {
                    clear_login_menu(&login_form);

                    current_menu = member_menu(menu_window, current_member);

                    display_login_form(&login_form);
                }
                else {
                    store_last_pos(login_window);

                    print_login_form_message(&login_form, "Incorrect username or password", ERROR);

                    restore_last_pos(login_window);
                }
                break;

            case SWITCH_MENU_ACTION:
                clear_login_menu(&login_form);

                current_menu = STAFF_MENU;
                break;

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
                reload_members(members);
                break;

            default:
                break;
        }
    }

    free_login_form(&login_form);
    free_members_vector(members);
    delwin(login_window);

    return current_menu;
}

current_menu_t member_menu(WINDOW* menu_window, member_t* member) {
    current_menu_t current_menu = MEMBER_MENU;

    wprintw(menu_window, "Welcome %s", member->username);
    wrefresh(menu_window);

    if (wgetch(menu_window) == KEY_ESC)
        current_menu = EXIT_MENU;

    wclear(menu_window);
    wrefresh(menu_window);
    return current_menu;
}
