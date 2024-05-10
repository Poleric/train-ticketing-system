#include <tui/table/staff_schedule_table.h>

void init_staff_schedule_table(WINDOW* window, schedule_table_t* schedule_table, weekly_schedule_t* weekly_schedules) {
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

    schedule_table->table.number_of_footers = 7;
    schedule_table->table.footers = calloc(schedule_table->table.number_of_footers, sizeof(const char*));
    schedule_table->table.footers[0] = "[q] Quit";
    schedule_table->table.footers[1] = "[← →] Change Day";
    schedule_table->table.footers[2] = "[↑↓] Scroll";
    schedule_table->table.footers[3] = "";
    schedule_table->table.footers[4] = "[D] Delete";
    schedule_table->table.footers[5] = "[E] Edit";
    schedule_table->table.footers[6] = "[A] Add";

    schedule_table->table.footer_widths = calloc(schedule_table->table.number_of_footers, sizeof (int));
    schedule_table->table.footer_widths[0] = 1;
    schedule_table->table.footer_widths[1] = 1;
    schedule_table->table.footer_widths[2] = 1;
    schedule_table->table.footer_widths[3] = 1;
    schedule_table->table.footer_widths[4] = 1;
    schedule_table->table.footer_widths[5] = 1;
    schedule_table->table.footer_widths[6] = 1;

    schedule_table->weekly_schedule = weekly_schedules;

    schedule_table->table.window = window;

    schedule_table->table.number_of_display_lines = getmaxy(window) - 3;

    schedule_table->table.width = 0;
    for (int i = 0; i < schedule_table->table.number_of_columns; i++) {
        schedule_table->table.width += schedule_table->table.column_widths[i];
    }

    scale_table_to_window(&schedule_table->table);

    schedule_table->table.current_line = 0;
    schedule_table->table.selected_line = 0;

    split_tm(tm_now(), &schedule_table->today_date, NULL, (int *)&schedule_table->selected_wday);

    schedule_table->selected_date = schedule_table->today_date;

    keypad(schedule_table->table.window, true);
    curs_set(0);
}