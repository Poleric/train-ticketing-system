#include <tui/start_menu.h>
#include <tui/form/register_form.h>
#include <tui/form/login_form.h>
#include <tui/table/schedule_table.h>
#include <tui/menu/member_menu.h>
#include <locale.h>
#include <stdlib.h>

#define TITLE "Hambalang Star Rail"
#define MEMBER_LOGIN_HEADER "Login as Member"
#define MEMBER_REGISTRATION_HEADER "Registration"
#define STAFF_LOGIN_HEADER "Login as Staff"

void reload_members(member_vector_t* members) {
    for (int i = 0; i < members->num_of_members; i++)
        free_member(members->array[i]);
    members->num_of_members = 0;
    load_members(members, MEMBERS_FILEPATH);
}

void reload_staff(staff_vector_t* staff) {
    for (int i = 0; i < staff->num_of_staff; i++)
        free_staff(staff->array[i]);
    load_staff(staff, STAFFS_FILEPATH);
}

void init_color_pairs() {
    /* Initialize preset colors pairs
     * 1 - Green background
     * 2 - Cyan background
     * 3 - Yellow background
     */
    use_default_colors();
    assume_default_colors(-1, -1);
    init_pair(COLOR_1, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_2, COLOR_CYAN, COLOR_BLACK);
    init_pair(BACKGROUND, -1, COLOR_WHITE);
    init_pair(SELECTED, COLOR_YELLOW, COLOR_BLACK);
    init_pair(ERROR, COLOR_WHITE, COLOR_RED);
    init_pair(GOOD, COLOR_WHITE, COLOR_GREEN);
}

void start_menu() {
    // init
    setlocale(LC_ALL, "");
    initscr();
    start_color(); init_color_pairs();
    raw();  // get input by chars
    noecho();  // don't display input

    current_menu_t current_menu = MEMBER_MENU;
    while (current_menu != EXIT_MENU) {
        switch (current_menu) {
            case MEMBER_MENU:
                current_menu = member_login_menu(stdscr);
                break;
            case STAFF_MENU:
                current_menu = staff_login_menu(stdscr);
                break;
            case EXIT_MENU:
                break;
        }
    }

    endwin();
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

    init_login_form(&login_form, login_window, TITLE, MEMBER_LOGIN_HEADER);
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

                    member_menu(menu_window, current_member);

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

void member_menu(WINDOW* menu_window, member_t* member) {
    WINDOW* member_menu_window;
    member_menu_t member_menu;

    member_menu_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    char header_message[50];
    snprintf(header_message, 50, "Welcome %s", member->username);

    init_member_menu(&member_menu, member_menu_window, TITLE, header_message);
    display_member_menu(&member_menu, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (menu_driver(&member_menu.menu, wgetch(member_menu_window))) {
            case SUBMIT_ACTION:
                cleanup_menu(&member_menu.menu);

                switch (member_menu.menu.selected_option) {
                    case 0:  // book ticket
                        schedule_menu(menu_window, member);
                        break;
                    case 1:  // view own ticket
                        view_ticket_menu(menu_window, member);
                        break;
                    case 2:  // view own details
                        view_member_details_menu(menu_window, member);
                        break;
                    case 3:  // send feedback
                        member_feedback_form(menu_window, member);
                        break;
                    case 4:  // logout
                        exit = true;
                        break;
                }

                if (!exit) display_member_menu(&member_menu, COLOR_1);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            case REFRESH_SCREEN_ACTION:
                display_member_menu(&member_menu, COLOR_1);
                break;

            default:
                break;
        }

        // uh cleanup somehow affect member_menu_window
        keypad(member_menu_window, true);
        curs_set(0);
    }

    free_member_menu(&member_menu);

    delwin(member_menu_window);
}

void view_member_details_menu(WINDOW* menu_window, member_t* member) {

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

    init_register_form(&register_form, register_window, TITLE, MEMBER_REGISTRATION_HEADER);
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

void member_feedback_form(WINDOW* menu_window, member_t* member) {

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

                    current_menu = staff_menu(menu_window, current_staff);

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

current_menu_t staff_menu(WINDOW* menu_window, staff_t* staff) {
    current_menu_t current_menu = STAFF_MENU;

    wprintw(menu_window, "Welcome %s", staff->username);
    wrefresh(menu_window);

    if (wgetch(menu_window) == KEY_ESC)
        current_menu = EXIT_MENU;

    wclear(menu_window);
    wrefresh(menu_window);
    return current_menu;
}


void schedule_menu(WINDOW* menu_window, member_t* member) {
    WINDOW* schedule_window;
    weekly_schedule_t weekly_schedule;
    schedule_table_t schedule_table;

    schedule_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_weekly_schedule(&weekly_schedule);
    load_weekly_schedule(&weekly_schedule, SCHEDULES_FILEPATH);

    init_schedule_table(schedule_window, &schedule_table, &weekly_schedule);

    bool exit = false;
    do {
        display_schedules(&schedule_table);
        switch (wgetch(schedule_table.table.window)) {
            case 'q':
            case CTRL('C'):
                exit = true;
                break;
            case KEY_LEFT:
                if (schedule_table.selected_wday == 0)
                    schedule_table.selected_wday = 6;
                else
                    schedule_table.selected_wday--;
                schedule_table.selected_date = date_add_days(schedule_table.selected_date, -1);
                schedule_table.selected_wday %= 7;
                break;
            case KEY_RIGHT:
                schedule_table.selected_wday++;
                schedule_table.selected_date = date_add_days(schedule_table.selected_date, 1);
                schedule_table.selected_wday %= 7;
                break;
            case KEY_UP:  // TODO: fix scrolling
                if (schedule_table.table.current_line > 0 &&
                    schedule_table.table.selected_line < schedule_table.weekly_schedule->days[schedule_table.selected_wday].n_elements - schedule_table.table.number_of_display_lines + 1
                        )
                    schedule_table.table.current_line--;
                if (schedule_table.table.selected_line > 0)
                    schedule_table.table.selected_line--;
                break;
            case KEY_DOWN:
                if (schedule_table.table.current_line < schedule_table.weekly_schedule->days[schedule_table.selected_wday].n_elements - schedule_table.table.number_of_display_lines)
                    schedule_table.table.current_line++;
                if (schedule_table.table.selected_line < schedule_table.weekly_schedule->days[schedule_table.selected_wday].n_elements - 1)
                    schedule_table.table.selected_line++;
                break;
            case KEY_ENTER:

                break;
        }
        wclear(schedule_table.table.window);
    } while (!exit);

    save_weekly_schedule(&weekly_schedule, SCHEDULES_FILEPATH);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);
}

void view_ticket_menu(WINDOW* menu_window, member_t* member) {

}
