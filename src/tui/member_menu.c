#include <tui/member_menu.h>
#include <tui/form/login_form.h>
#include <tui/form/register_form.h>
#include <tui/utils/menu_utils.h>

void reload_members(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    members->num_of_members = 0;
    load_members(members, MEMBERS_FILEPATH);
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

    init_login_form(&login_form, login_window, "Train Ticketing", "Login as Member");
    display_login_form(&login_form, COLOR_1);

    members = init_members_vector();
    load_members(members, MEMBERS_FILEPATH);

    current_menu_t current_menu = MEMBER_MENU;
    while (current_menu == MEMBER_MENU) {
        switch (form_driver(&login_form.form, wgetch(login_window))) {
            case SUBMIT_ACTION:
                current_member = login_as_member(members, get_username(&login_form), get_password(&login_form));
                if (current_member) {
                    clear_login_menu(&login_form);

                    current_menu = member_menu(menu_window, current_member);

                    display_login_form(&login_form, COLOR_1);
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

            case REGISTER_ACTION:
                clear_login_menu(&login_form);

                member_registration_menu(menu_window, members);

                display_login_form(&login_form, COLOR_1);
                break;

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
                reload_members(members);
                break;

            case REFRESH_SCREEN_ACTION:
                display_login_form(&login_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_members(members, MEMBERS_FILEPATH);

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

void member_registration_menu(WINDOW* menu_window, member_vector_t* members) {
    register_form_t register_form;
    WINDOW* register_window;

    register_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_register_form(&register_form, register_window, "Train Ticketing", "Registration");
    display_register_form(&register_form, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&register_form.form, wgetch(register_window))) {
            case SUBMIT_ACTION:
                if (is_member_exists(members, get_register_username(&register_form))) {
                    print_register_form_message(&register_form, "Username is taken", ERROR);
                    break;
                }

                if (!validate_email(&register_form)) {
                    print_register_form_message(&register_form, "Email is invalid", ERROR);
                    break;
                }

                if (!validate_same_password(&register_form)) {
                    print_register_form_message(&register_form, "Password does not match", ERROR);
                    break;
                }

                if (!validate_gender(&register_form)) {
                    print_register_form_message(&register_form, "Invalid gender", ERROR);
                    break;
                }

                if (!validate_contact_no(&register_form)) {
                    print_register_form_message(&register_form, "Invalid contact no", ERROR);
                    break;
                }

                create_member_record(
                        members,
                        get_register_username(&register_form),
                        get_register_password(&register_form),
                        get_register_gender(&register_form),
                        get_register_email(&register_form),
                        get_register_contact_no(&register_form),
                        NONE
                        );

                print_register_form_message(&register_form, "Successfully registered", GOOD);
                exit = true;

                wgetch(register_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(register_form.form.window, "Exit registration ?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_register_form(&register_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_members(members, MEMBERS_FILEPATH);

    free_register_form(&register_form);

    delwin(register_window);
}
