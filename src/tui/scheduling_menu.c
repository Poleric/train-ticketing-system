#include <tui/scheduling_menu.h>
#include <tui/utils/tui_utils.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_DAY_OF_WEEK SUNDAY
#define SELECTED_DAY_PADDING 5

void init_schedule_table(WINDOW* window, schedule_table_t* schedule_table, weekly_schedule_t* weekly_schedules) {
    schedule_table->table.number_of_columns = 6;
    schedule_table->table.headers = calloc(schedule_table->table.number_of_columns, sizeof(const char*));
    schedule_table->table.headers[0] = "From Station";
    schedule_table->table.headers[1] = "To Station";
    schedule_table->table.headers[2] = "Departure Time";
    schedule_table->table.headers[3] = "Arrival Time";
    schedule_table->table.headers[4] = "Available";
    schedule_table->table.headers[5] = "Train ID";

    schedule_table->table.column_widths = calloc(schedule_table->table.number_of_columns, sizeof (int));
    schedule_table->table.column_widths[0] = 13;
    schedule_table->table.column_widths[1] = 13;
    schedule_table->table.column_widths[2] = 15;
    schedule_table->table.column_widths[3] = 15;
    schedule_table->table.column_widths[4] = 10;
    schedule_table->table.column_widths[5] = 8;

    schedule_table->table.number_of_footers = 4;
    schedule_table->table.footers = calloc(schedule_table->table.number_of_footers, sizeof(const char*));
    schedule_table->table.footers[0] = "[q] Quit";
    schedule_table->table.footers[1] = "[← →] Change Day";
    schedule_table->table.footers[2] = "[↑↓] Scroll";
    schedule_table->table.footers[3] = "[Enter] View Details";

    schedule_table->table.footer_widths = calloc(schedule_table->table.number_of_footers, sizeof (int));
    schedule_table->table.footer_widths[0] = 1;
    schedule_table->table.footer_widths[1] = 1;
    schedule_table->table.footer_widths[2] = 1;
    schedule_table->table.footer_widths[3] = 1;

    schedule_table->weekly_schedule = weekly_schedules;

    schedule_table->table.window = window;
    keypad(schedule_table->table.window, true);

    schedule_table->table.number_of_display_lines = getmaxy(window) - 3;

    schedule_table->table.width = 0;
    for (int i = 0; i < schedule_table->table.number_of_columns; i++) {
        schedule_table->table.width += schedule_table->table.column_widths[i];
    }

    scale_to_screen_size(&schedule_table->table);

    schedule_table->table.current_line = 0;
    schedule_table->table.selected_line = 0;

    schedule_table->selected_wday = FIRST_DAY_OF_WEEK;
}

void print_day_header(schedule_table_t* schedule_table, short color_pair, short selected_color_pair) {
    int selected_wday_x = 0, wday_text_len = 0, _;
    (void)_;  // supress unused warning

    for (int i = 0; i < 7; i++) {
        tm_wday_t tm_wday = (i + FIRST_DAY_OF_WEEK) % 7;
        const char* day_text = tm_wday_to_text(tm_wday);
        if (tm_wday == schedule_table->selected_wday) {
            getyx(schedule_table->table.window, _, selected_wday_x);
            wprintw(schedule_table->table.window, "%s", day_text);
            move_offset_x(schedule_table->table.window, SELECTED_DAY_PADDING);
            wday_text_len = (int)strlen(day_text) + SELECTED_DAY_PADDING;
        } else {
            wprintw(schedule_table->table.window, "%.3s", day_text);
            move_offset_x(schedule_table->table.window, 1);
        }
    }
    int max_x;
    getyx(schedule_table->table.window, _, max_x);

    // highlight entire table
    move_to_x(schedule_table->table.window, 0);
    wchgat(schedule_table->table.window, max_x, A_STANDOUT, color_pair, NULL);

    move_to_x(schedule_table->table.window, selected_wday_x);
    wchgat(schedule_table->table.window, wday_text_len, A_STANDOUT, selected_color_pair, NULL);
}

void print_schedule_row(schedule_table_t* schedule_table, schedule_t * schedule) {
    int offset_x = 0;
    char depart_time[7], eta[7];

    move_offset_x(schedule_table->table.window, get_offset_for_centered((int)strlen(schedule->from_station_id), schedule_table->table.column_widths[0]));
    wprintw(schedule_table->table.window, "%s", schedule->from_station_id);
    offset_x += schedule_table->table.column_widths[0];

    move_to_x(schedule_table->table.window, offset_x);

    move_offset_x(schedule_table->table.window, get_offset_for_centered((int)strlen(schedule->to_station_id), schedule_table->table.column_widths[1]));
    wprintw(schedule_table->table.window, "%s", schedule->to_station_id);
    offset_x += schedule_table->table.column_widths[1];

    move_to_x(schedule_table->table.window, offset_x);

    to_time(schedule->departure_time, depart_time, 7);
    move_offset_x(schedule_table->table.window, get_offset_for_centered((int)strlen(depart_time), schedule_table->table.column_widths[2]));
    wprintw(schedule_table->table.window, "%s", depart_time);
    offset_x += schedule_table->table.column_widths[2];

    move_to_x(schedule_table->table.window, offset_x);

    to_time(schedule->eta, eta, 7);
    move_offset_x(schedule_table->table.window, get_offset_for_centered((int)strlen(eta), schedule_table->table.column_widths[3]));
    wprintw(schedule_table->table.window, "%s", eta);
    offset_x += schedule_table->table.column_widths[3];

    move_to_x(schedule_table->table.window, offset_x);

//    move_offset_x(schedule_table->table.pad, get_offset_for_centered((int)strlen(schedule->from_station_id), schedule_table->table.column_widths[4]));
//    wprintw(schedule_table->table.pad, "%d", get_number_of_available_seats);
    offset_x += schedule_table->table.column_widths[4];

    move_to_x(schedule_table->table.window, offset_x);

    move_offset_x(schedule_table->table.window, get_offset_for_centered((int)strlen(schedule->train_id), schedule_table->table.column_widths[5]));
    wprintw(schedule_table->table.window, "%s", schedule->train_id);
}

void display_schedules(schedule_table_t* schedule_table) {
    print_day_header(schedule_table, 2, 3);

    move_to_next_line(schedule_table->table.window, 0);

    print_table_header(&schedule_table->table, 1);

    move_to_next_line(schedule_table->table.window, 0);

    schedule_vector_t schedule_vector = schedule_table->weekly_schedule->days[schedule_table->selected_wday];
    for (
        int i = schedule_table->table.current_line;
        i < schedule_vector.n_elements && i < schedule_table->table.current_line + schedule_table->table.number_of_display_lines;
        i++
    ) {
        print_schedule_row(schedule_table, schedule_vector.array[i]);
        move_to_next_line(schedule_table->table.window, 0);
    }

    highlight_selected_row(&schedule_table->table, 2, 3);

    print_table_footer(&schedule_table->table, 1);

    wrefresh(schedule_table->table.window);
}

void free_schedule_table(schedule_table_t* schedule_table) {
    keypad(schedule_table->table.window, false);
    free_table(&schedule_table->table);
}

void schedule_menu() {
    curs_set(0);

    WINDOW* window = newwin(LINES, COLS, 0, 0);

    weekly_schedule_t weekly_schedule;
    schedule_table_t schedule_table;

    init_weekly_schedule(&weekly_schedule);
    load_weekly_schedule(&weekly_schedule, "schedule.txt");

    init_schedule_table(window, &schedule_table, &weekly_schedule);

    schedule_table.selected_wday = FIRST_DAY_OF_WEEK;
    bool exit = false;
    do {
        display_schedules(&schedule_table);
        switch (wgetch(schedule_table.table.window)) {
            case 'q':
                exit = true;
                break;
            case KEY_LEFT:
                if (schedule_table.selected_wday == 0)
                    schedule_table.selected_wday = 6;
                else
                    schedule_table.selected_wday--;
                schedule_table.selected_wday %= 7;
                break;
            case KEY_RIGHT:
                schedule_table.selected_wday++;
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
        }
        wclear(schedule_table.table.window);
    } while (!exit);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);

    curs_set(1);
}
