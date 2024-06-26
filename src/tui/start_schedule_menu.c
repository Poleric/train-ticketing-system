#include <tui/start_menu.h>
#include <tui/table/schedule_table.h>
#include <tui/menu/seat_menu.h>
#include <tui/table/staff_schedule_table.h>
#include <tui/form/schedule_details_form.h>
#include <string.h>

#define CREATE_SCHEDULE_HEADER "Create new schedule"
#define EDIT_SCHEDULE_HEADER "Editing schedule"

void view_schedule_menu(WINDOW* menu_window, member_t* member) {
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
                if (diff_date(schedule_table.selected_date, schedule_table.today_date) <= 0)
                    break;

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
            case '\n':
            case '\r':
                schedule_t* schedule = get_selected_schedule(&schedule_table);
                if (schedule == NULL)
                    break;

                if (get_schedule_number_of_free_seats(TICKETS_FILEPATH, schedule, schedule_table.selected_date) > 1) {
                    clear_table(&schedule_table.table);
                    view_seat_menu(menu_window, member, schedule_table.selected_date,
                                   get_selected_schedule(&schedule_table));
                    display_schedules(&schedule_table);

                    // uh cleanup somehow affect member_menu_window
                    keypad(schedule_window, true);
                    curs_set(0);
                }
                break;
        }
        wclear(schedule_table.table.window);
    } while (!exit);

    save_weekly_schedule(&weekly_schedule, SCHEDULES_FILEPATH);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);
}

void view_seat_menu(WINDOW* menu_window, member_t* member, dt_date_t date, schedule_t* schedule) {
    WINDOW* seat_menu_window;
    seat_menu_t seat_menu;

    seat_menu_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    char header_message[50];

    init_seat_menu(&seat_menu, seat_menu_window, TITLE, header_message, date, schedule);
    snprintf(header_message, 50, "Carriage %d", get_current_page(&seat_menu.menu));
    display_seat_menu(&seat_menu, COLOR_1);

    bool exit = false;
    while (!exit) {
        switch (int_menu_driver(&seat_menu.menu, wgetch(seat_menu_window))) {
            case SUBMIT_ACTION:
                int number_of_selected = get_number_of_selected(&seat_menu.menu);
                if (number_of_selected < 0)
                    break;

                if (confirmation_menu(seat_menu_window, "Confirm booking tickets?") == EXIT_FAILURE) {
                    display_seat_menu(&seat_menu, COLOR_1);
                    break;
                }

                char price_text[40];
                snprintf(price_text, 25, "That will be $%.2lf total", number_of_selected * schedule->price);
                if (confirmation_menu(seat_menu_window, price_text) == EXIT_FAILURE) {
                    display_seat_menu(&seat_menu, COLOR_1);
                    break;
                }

                for (int i = 0; i < seat_menu.menu.number_of_options; i++)
                    if (seat_menu.menu.menu_options[i].selected)
                        book_ticket(TICKETS_FILEPATH, date, schedule, member->username, time(NULL), seat_menu.menu.menu_options[i].label);
                exit = true;

                confirmation_menu(seat_menu_window, "Successfully booked");
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

            case REFRESH_SCREEN_ACTION:
                snprintf(header_message, 50, "Carriage %d", get_current_page(&seat_menu.menu));
                display_seat_menu(&seat_menu, COLOR_1);
                break;

            default:
                break;
        }
        // uh cleanup somehow affect member_menu_window
        keypad(seat_menu_window, true);
        curs_set(0);
    }

    free_seat_menu(&seat_menu);

    delwin(seat_menu_window);
}

void view_staff_schedule_menu(WINDOW* menu_window, staff_t* staff) {
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

    init_staff_schedule_table(schedule_window, &schedule_table, &weekly_schedule);

    bool exit = false;
    do {
        display_schedules(&schedule_table);
        switch (wgetch(schedule_table.table.window)) {
            case 'q':
            case CTRL('C'):
                exit = true;
                break;

            case KEY_LEFT:
                if (diff_date(schedule_table.selected_date, schedule_table.today_date) <= 0)
                    break;

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

            case 'd':
            case 'D':
                if (confirmation_menu(schedule_window, "Delete selected schedule?") == EXIT_SUCCESS)
                    delete_schedule_by_info(&weekly_schedule.days[schedule_table.selected_wday], get_selected_schedule(&schedule_table));

                keypad(schedule_window, true);
                curs_set(0);
                break;

            case 'e':
            case 'E':
                wclear(schedule_window);
                curs_set(1);

                edit_schedule_menu(menu_window, &weekly_schedule, get_selected_schedule(&schedule_table), schedule_table.selected_wday);

                curs_set(0);
                break;

            case 'a':
            case 'A':
                wclear(schedule_window);
                curs_set(1);

                create_schedule_menu(menu_window, &weekly_schedule);

                curs_set(0);
                break;
        }
        wclear(schedule_table.table.window);
    } while (!exit);

    save_weekly_schedule(&weekly_schedule, SCHEDULES_FILEPATH);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);
}


void create_schedule_menu(WINDOW* menu_window, weekly_schedule_t* weekly_schedules) {
    schedule_form_t schedule_form;
    WINDOW* schedule_form_window;

    schedule_form_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_schedule_form(&schedule_form, schedule_form_window, TITLE, CREATE_SCHEDULE_HEADER);
    display_schedule_form(&schedule_form, COLOR_2);

    schedule_t* schedule;

    bool exit = false;
    while (!exit) {
        switch (form_driver(&schedule_form.form, wgetch(schedule_form_window))) {
            case SUBMIT_ACTION:
                if (!validate_schedule_form_train_id(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Train ID is invalid", ERROR);
                    break;
                }

                if (!validate_schedule_form_from_station_id(&schedule_form) || !validate_schedule_form_to_station_id(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Station ID", ERROR);
                    break;
                }

                if (!validate_schedule_form_weekday(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid weekday", ERROR);
                    break;
                }

                if (!validate_schedule_form_departure_time(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Departure Time", ERROR);
                    break;
                }

                if (!validate_schedule_form_eta(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Estimated Arrival Time", ERROR);
                    break;
                }

                if (!validate_schedule_form_number_of_seats(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Number of seats", ERROR);
                    break;
                }

                if (!validate_schedule_form_price(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid price", ERROR);
                    break;
                }

                schedule = create_schedule(
                        get_schedule_form_train_id(&schedule_form),
                        get_schedule_form_from_station_id(&schedule_form),
                        get_schedule_form_to_station_id(&schedule_form),
                        get_schedule_form_departure_time(&schedule_form),
                        get_schedule_form_eta(&schedule_form),
                        get_schedule_form_number_of_seats(&schedule_form),
                        get_schedule_form_price(&schedule_form)
                );
                add_schedule(weekly_schedules->days + get_schedule_form_weekday(&schedule_form), schedule);

                print_schedule_form_message(&schedule_form, "Successfully registered. Press any key to continue.", GOOD);
                exit = true;

                wgetch(schedule_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(schedule_form.form.window, "Exit?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_schedule_form(&schedule_form, COLOR_2);
                break;

            default:
                break;
        }
    }

    free_schedule_form(&schedule_form);

    delwin(schedule_form_window);
}

void edit_schedule_menu(WINDOW* menu_window, weekly_schedule_t* weekly_schedules, schedule_t* schedule, tm_wday_t weekday) {
    schedule_form_t schedule_form;
    WINDOW* schedule_form_window;

    schedule_form_window = derwin(
            menu_window,
            LINES,
            COLS,
            0,
            0
    );

    init_schedule_details_form(&schedule_form, schedule_form_window, TITLE, EDIT_SCHEDULE_HEADER, schedule, weekday);
    display_schedule_form(&schedule_form, COLOR_2);

    bool exit = false;
    while (!exit) {
        switch (form_driver(&schedule_form.form, wgetch(schedule_form_window))) {
            case SUBMIT_ACTION:
                if (!validate_schedule_form_train_id(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Train ID is invalid", ERROR);
                    break;
                }

                if (!validate_schedule_form_from_station_id(&schedule_form) || !validate_schedule_form_to_station_id(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Station ID", ERROR);
                    break;
                }

                if (!validate_schedule_form_weekday(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid weekday", ERROR);
                    break;
                }

                if (!validate_schedule_form_departure_time(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Departure Time", ERROR);
                    break;
                }

                if (!validate_schedule_form_eta(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Estimated Arrival Time", ERROR);
                    break;
                }

                if (!validate_schedule_form_number_of_seats(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid Number of seats", ERROR);
                    break;
                }

                if (!validate_schedule_form_price(&schedule_form)) {
                    print_schedule_form_message(&schedule_form, "Invalid price", ERROR);
                    break;
                }

                // updating
                strncpy(schedule->train_id, get_schedule_form_train_id(&schedule_form), 5);
                strncpy(schedule->from_station_id, get_schedule_form_from_station_id(&schedule_form), 4);
                strncpy(schedule->to_station_id, get_schedule_form_to_station_id(&schedule_form), 4);
                schedule->departure_time = get_schedule_form_departure_time(&schedule_form);
                schedule->eta = get_schedule_form_eta(&schedule_form);
                schedule->n_seats = get_schedule_form_number_of_seats(&schedule_form);
                schedule->price = get_schedule_form_price(&schedule_form);

                print_schedule_form_message(&schedule_form, "Edited. Press any key to continue.", GOOD);
                exit = true;

                wgetch(schedule_form.form.window);
                break;

            case EXIT_FORM_ACTION:
                exit = true;
                break;

                // refresh even if not change screen
            case REGISTER_ACTION:
                if (confirmation_menu(schedule_form.form.window, "Exit?") == EXIT_SUCCESS) {
                    exit = true;
                    break;
                }

            case REFRESH_SCREEN_ACTION:
                display_schedule_form(&schedule_form, COLOR_2);
                break;

            default:
                break;
        }
    }

    free_schedule_form(&schedule_form);

    delwin(schedule_form_window);
}