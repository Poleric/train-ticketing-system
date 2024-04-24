#include <tui/staff_menu.h>
#include <tui/template/login_form.h>
#include <tui/menu_utils.h>

//void reload_staff(staff_vector_t* staff) {
//    for (int i = 0; i < staff->num_of_staff; i++)
//        free_staff(staff->array[i]);
//    load_staff(staff);
//}

// staff_login -> staff_menu   --┐
//   / \        -> register_menu --|
//    └----------------------------┘
current_menu_t staff_login_menu(WINDOW* menu_window) {
    WINDOW* login_window = newwin(
            10,
            50,
            1,
            get_centered_x_start(menu_window, 50));

    keypad(login_window, TRUE);

    FORM* login_form = create_login_form(login_window);

    print_login_menu("Staff menu", menu_window, login_form);

//    staff_vector_t* staff = init_staff_vector();
//    load_staff(staff);

    staff_t* current_staff;

    current_menu_t current_menu = STAFF_MENU;
    while (current_menu == STAFF_MENU) {
        int ch = wgetch(login_window);

        switch (form_driver(login_form, ch)) {
            case SUBMIT_ACTION:
//                current_staff = login_as(staff, get_username(login_form), get_password(login_form));
//                if (current_staff) {
//                    clear_current_menu(menu_window, login_form);
//
//                    current_menu = staff_menu(menu_window, current_staff);
//
//                    print_login_menu("Staff menu", menu_window, login_form);
//                }
//                else {
//                    store_last_pos(login_window);
//                    mvwprintw(login_window, 3, 0, "Wrong username or password");
//                    restore_last_pos(login_window);
//                }
                break;

            case SWITCH_MENU_ACTION:
                clear_current_menu(menu_window, login_form);

                current_menu = MEMBER_MENU;
                break;

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
//                reload_staff(staff);
                break;

            default:
                break;
        }
    }

    cleanup_form(login_form);
//    free_staff_vector(staff);
    delwin(login_window);

    return current_menu;
}

current_menu_t staff_menu(WINDOW* menu_window, staff_t* staff) {
    current_menu_t current_menu = MEMBER_MENU;

    wprintw(menu_window, "Welcome %s", staff->username);
    wrefresh(menu_window);

    if (wgetch(menu_window) == KEY_ESC)
        current_menu = EXIT_MENU;

    wclear(menu_window);
    wrefresh(menu_window);
    return current_menu;
}
