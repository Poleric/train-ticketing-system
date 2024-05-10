#include <tui/start_menu.h>
#include <tui/menu/staff_menu.h>
#include <tui/form/register_staff_form.h>
#include <tui/form/staff_details_form.h>
#include <tui/table/staff_table.h>
#include <string.h>

#define STAFF_LOGIN_HEADER "Login as Staff"
#define STAFF_REGISTRATION_HEADER "Staff Registration"

void reload_staff(staff_vector_t* staff) {
    for (int i = 0; i < staff->num_of_staff; i++)
        free_staff(staff->array[i]);
    load_staff(staff, STAFFS_FILEPATH);
}

// staff_login -> staff_menu   --┐
//   / \        -> register_menu --|
//    └----------------------------┘
current_menu_t staff_login_menu(WINDOW* menu_window) {
    WINDOW* login_window;
    login_form_t login_form;

    staff_vector_t * staffs;
    staff_t * current_staff;

    login_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_login_form(&login_form, login_window, TITLE, STAFF_LOGIN_HEADER);

    // remove register and forgot password
    login_form.footers[1] = "";
    login_form.footers[2] = "";

    display_login_form(&login_form, COLOR_2);

    staffs = init_staff_vector();
    load_staff(staffs, STAFFS_FILEPATH);

    current_menu_t current_menu = STAFF_MENU;
    while (current_menu == STAFF_MENU) {
        switch (form_driver(&login_form.form, wgetch(login_window))) {
            case SUBMIT_ACTION:
                current_staff = login_as_staff(staffs, get_username(&login_form), get_password(&login_form));
                if (current_staff) {
                    clear_login_menu(&login_form);

                    staff_menu(menu_window, staffs, current_staff);

                    display_login_form(&login_form, COLOR_2);
                }
                else {
                    store_last_pos(login_window);

                    print_login_form_message(&login_form, "Incorrect username or password", ERROR);

                    restore_last_pos(login_window);
                }
                break;

            case SWITCH_MENU_ACTION:
                clear_login_menu(&login_form);

                current_menu = MEMBER_MENU;
                break;

            case EXIT_FORM_ACTION:
                current_menu = EXIT_MENU;
                break;

            case RELOAD_ACTION:
                reload_staff(staffs);
                break;

            case REFRESH_SCREEN_ACTION:
                display_login_form(&login_form, COLOR_2);
                break;

            default:
                break;
        }
    }

    write_staff(staffs, STAFFS_FILEPATH);

    free_login_form(&login_form);
    free_staff_vector(staffs);
    delwin(login_window);

    return current_menu;
}

void staff_menu(WINDOW* menu_window, staff_vector_t* staffs, staff_t* staff) {
    WINDOW* staff_menu_window;
    staff_menu_t staff_menu;

    staff_menu_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    char header_message[50];
    snprintf(header_message, 50, "Welcome %s", staff->username);

    init_staff_menu(&staff_menu, staff_menu_window, TITLE, header_message);
    display_staff_menu(&staff_menu, COLOR_2);

    bool exit = false;
    while (!exit) {
        switch (menu_driver(&staff_menu.menu, wgetch(staff_menu_window))) {
            case SUBMIT_ACTION:  // view staffs
                cleanup_menu(&staff_menu.menu);

                switch (staff_menu.menu.selected_option) {
                    case 0:
                        cleanup_menu(&staff_menu.menu);

                        view_staffs_menu(menu_window, staffs, staff);

                        display_staff_menu(&staff_menu, COLOR_2);
                        break;
                    case 1:  // view schedule
                        cleanup_menu(&staff_menu.menu);

                        view_staff_schedule_menu(menu_window, staff);

                        display_staff_menu(&staff_menu, COLOR_2);
                        break;
                    case 2:  // edit self
                        cleanup_menu(&staff_menu.menu);

                        view_staff_details_menu(menu_window, staffs, staff);

                        display_staff_menu(&staff_menu, COLOR_2);
                    case 3:  // view all tickets
                        break;
                    case 4:  // logout
                        exit = true;
                        break;
                }

                if (!exit) display_staff_menu(&staff_menu, COLOR_2);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            case REFRESH_SCREEN_ACTION:
                display_staff_menu(&staff_menu, COLOR_2);
                break;

            default:
                break;
        }

        // uh cleanup somehow affect member_menu_window
        keypad(staff_menu_window, true);
        curs_set(0);
    }

    free_staff_menu(&staff_menu);

    delwin(staff_menu_window);
}

void staff_registration_menu(WINDOW* menu_window, staff_vector_t* staffs) {
    staff_register_form_t staff_register_form;
    WINDOW* staff_register_window;

    staff_register_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_staff_register_form(&staff_register_form, staff_register_window, TITLE, STAFF_REGISTRATION_HEADER);
    display_staff_register_form(&staff_register_form, COLOR_2);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&staff_register_form.form, wgetch(staff_register_window))) {
            case SUBMIT_ACTION:
                if (is_staff_exists(staffs, get_staff_register_username(&staff_register_form))) {
                    print_staff_register_form_message(&staff_register_form, "Username is taken", ERROR);
                    break;
                }

                if (!validate_staff_register_email(&staff_register_form)) {
                    print_staff_register_form_message(&staff_register_form, "Email is invalid", ERROR);
                    break;
                }

                if (!validate_staff_register_same_password(&staff_register_form)) {
                    print_staff_register_form_message(&staff_register_form, "Password does not match", ERROR);
                    break;
                }

                if (!validate_staff_register_salary(&staff_register_form)) {
                    print_staff_register_form_message(&staff_register_form, "Invalid salary", ERROR);
                    break;
                }

                if (!validate_staff_register_contact_no(&staff_register_form)) {
                    print_staff_register_form_message(&staff_register_form, "Invalid position", ERROR);
                    break;
                }

                create_staff_record(
                        staffs,
                        get_staff_register_username(&staff_register_form),
                        get_staff_register_password(&staff_register_form),
                        get_staff_register_email(&staff_register_form),
                        get_staff_register_contact_no(&staff_register_form),
                        get_staff_register_position(&staff_register_form),
                        0,
                        get_staff_register_salary(&staff_register_form)
                );

                print_staff_register_form_message(&staff_register_form, "Successfully registered. Press any key to continue.", GOOD);
                exit = true;

                wgetch(staff_register_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(staff_register_form.form.window, "Exit registration ?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_staff_register_form(&staff_register_form, COLOR_2);
                break;

            default:
                break;
        }
    }

    write_staff(staffs, MEMBERS_FILEPATH);

    free_staff_register_form(&staff_register_form);

    delwin(staff_register_window);
}

void view_staff_details_menu(WINDOW* menu_window, staff_vector_t* staffs, staff_t* staff) {
    staff_register_form_t register_form;
    WINDOW* register_form_window;

    register_form_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_staff_details_form(&register_form, register_form_window, TITLE, STAFF_REGISTRATION_HEADER, staff);
    display_staff_register_form(&register_form, COLOR_2);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&register_form.form, wgetch(register_form_window))) {
            case SUBMIT_ACTION:
                if (is_staff_exists(staffs, get_staff_register_username(&register_form)) &&
                    strcmp(staff->username, get_staff_register_username(&register_form)) != 0) {
                    print_staff_register_form_message(&register_form, "Username is taken", ERROR);
                    break;
                }

                if (!validate_staff_register_email(&register_form)) {
                    print_staff_register_form_message(&register_form, "Email is invalid", ERROR);
                    break;
                }

                if (!validate_staff_register_same_password(&register_form) &&
                    strlen(get_staff_register_password(&register_form))) {
                    print_staff_register_form_message(&register_form, "Password does not match", ERROR);
                    break;
                }

                if (!validate_staff_register_salary(&register_form)) {
                    print_staff_register_form_message(&register_form, "Invalid salary amount", ERROR);
                    break;
                }

                if (!validate_staff_register_contact_no(&register_form)) {
                    print_staff_register_form_message(&register_form, "Invalid contact no", ERROR);
                    break;
                }

                // saving
                if (strcmp(staff->username, get_staff_register_username(&register_form)) != 0) {
                    free(staff->username);
                    staff->username = strdup(get_staff_register_username(&register_form));
                }
                if (strcmp(staff->email, get_staff_register_username(&register_form)) != 0) {
                    free(staff->email);
                    staff->email = strdup(get_staff_register_email(&register_form));
                }
                if (strlen(get_staff_register_password(&register_form)) > 0) {
                    hash_message(get_staff_register_password(&register_form), staff->hashed_password);
                }
                staff->salary = get_staff_register_salary(&register_form);
                if (strcmp(staff->contact_no, get_staff_register_contact_no(&register_form)) != 0) {
                    free(staff->contact_no);
                    staff->contact_no = strdup(get_staff_register_contact_no(&register_form));
                }
                if (strcmp(staff->position, get_staff_register_position(&register_form)) != 0) {
                    free(staff->position);
                    staff->position = get_staff_register_position(&register_form);
                }

                exit = true;
                print_staff_register_form_message(&register_form, "Details saved. Press any key", GOOD);
                wgetch(register_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(register_form.form.window, "Return to menu?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_staff_register_form(&register_form, COLOR_1);
                break;

            default:
                break;
        }
    }

    write_staff(staffs, MEMBERS_FILEPATH);

    free_staff_register_form(&register_form);

    delwin(register_form_window);
}

void view_staffs_menu(WINDOW* menu_window, staff_vector_t* staffs, staff_t* current_staff) {
    WINDOW* staff_table_window;
    staff_table_t staff_table;

    staff_table_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_staff_table(staff_table_window, &staff_table, staffs, current_staff);

    bool exit = false;
    do {
        display_staffs(&staff_table);
        switch (wgetch(staff_table.table.window)) {
            case 'q':
            case CTRL('C'):
                exit = true;
                break;

            case KEY_UP:  // TODO: fix scrolling
                if (staff_table.table.current_line > 0 &&
                    staff_table.table.selected_line < staffs->num_of_staff - staff_table.table.number_of_display_lines + 1
                        )
                    staff_table.table.current_line--;
                if (staff_table.table.selected_line > 0)
                    staff_table.table.selected_line--;
                break;
            case KEY_DOWN:
                if (staff_table.table.current_line < staffs->num_of_staff - staff_table.table.number_of_display_lines)
                    staff_table.table.current_line++;
                if (staff_table.table.selected_line < staffs->num_of_staff - 1)
                    staff_table.table.selected_line++;
                break;
            case 'R':
            case 'r':
                wclear(staff_table_window);
                curs_set(1);
                staff_registration_menu(menu_window, staffs);
                curs_set(0);
                break;

            case 'D':
            case 'd':
                if (confirmation_menu(staff_table_window, "Delete the selected staff?") == EXIT_SUCCESS)
                    delete_staff(staffs, get_selected_staff(&staff_table)->username);
                curs_set(0);
                break;
            case 'E':
            case 'e':
                wclear(staff_table_window);
                curs_set(1);
                view_staff_details_menu(menu_window, staffs, get_selected_staff(&staff_table));
                curs_set(0);
                break;
        }
        wclear(staff_table.table.window);
    } while (!exit);

    free_staff_table(&staff_table);
}