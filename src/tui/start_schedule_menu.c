#include <tui/start_menu.h>
#include <tui/table/schedule_table.h>
#include <tui/menu/seat_menu.h>
#include <tui/table/staff_schedule_table.h>

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

                break;
        }
        wclear(schedule_table.table.window);
    } while (!exit);

    save_weekly_schedule(&weekly_schedule, SCHEDULES_FILEPATH);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);

}