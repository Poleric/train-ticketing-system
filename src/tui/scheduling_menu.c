#include <tui/scheduling_menu.h>
#include <stdlib.h>
#include <string.h>


void print_header(table_t* table) {
    for (int i = 0; i < table->number_of_columns; i++) {
        move_offset_x(table->pad, table->column_padding);
        wprintw(table->pad, "%s", table->headers[i]);
        move_offset_x(table->pad, table->column_padding);
    }
}

void print_schedule_row(table_t* table, schedule_t * schedule) {
    int offset_x = 0;
    char depart_time[7], eta[7];

    waddch(table->pad, ACS_VLINE);
//    move_offset_x(table->pad, table->column_padding);
    move_offset_x(table->pad, get_offset_for_centered((int)strlen(schedule->from_station_id), table->column_widths[0]));
    wprintw(table->pad, "%s", schedule->from_station_id);
    offset_x += table->column_widths[0] + 1;

    move_offset_x(table->pad, get_offset_for_centered((int)strlen(schedule->from_station_id), table->column_widths[0]) - 1);
    wprintw(table->pad, "%s", schedule->to_station_id);
    offset_x += table->column_widths[1] + 1;

    move_to_x(table->pad, offset_x);

    waddch(table->pad, ACS_VLINE);
    move_offset_x(table->pad, table->column_padding);
    wprintw(table->pad, "%s", to_time(schedule->departure_time, depart_time, 7));
    offset_x += table->column_widths[2] + 1;

    move_to_x(table->pad, offset_x);

    waddch(table->pad, ACS_VLINE);
    move_offset_x(table->pad, table->column_padding);
    wprintw(table->pad, "%s", to_time(schedule->eta, eta, 7));
    offset_x += table->column_widths[3] + 1;

    move_to_x(table->pad, offset_x);

    waddch(table->pad, ACS_VLINE);
    move_offset_x(table->pad, table->column_padding);
    wprintw(table->pad, "%s", schedule->train_id);
    offset_x += table->column_widths[4] + 1;

    move_to_x(table->pad, offset_x);

    waddch(table->pad, ACS_VLINE);
}

void display_schedules(table_t* table, schedule_vector_t* schedule_vector) {
    wmove(table->pad, 0, 0);

    print_header(table);
    move_to_x(table->pad, table->column_widths[0] + 1); waddch(table->pad, ACS_BTEE);

    move_to_next_line(table->pad, 0);
    for (int i = 0; i < schedule_vector->n_elements; i++) {
        print_schedule_row(table, schedule_vector->array[i]);
        move_to_next_line(table->pad, 0);
    }

    move_to_x(table->pad, table->column_widths[0] + 1); waddch(table->pad, ACS_HLINE);

    prefresh(table->pad, table->current_line, table->current_col, 0, 0, LINES, COLS);
}

void init_schedule_table(table_t* table, int n_lines, int column_padding) {
    table->number_of_columns = 5;
    table->headers = calloc(table->number_of_columns, sizeof(const char*));
    table->headers[0] = "From Station";
    table->headers[1] = "To Station  ";
    table->headers[2] = "Departure Time";
    table->headers[3] = "Arrival Time  ";
    table->headers[4] = "Train ID";
    table->column_padding = column_padding;

    table->max_lines = n_lines;
    table->max_cols = 1;
    table->column_widths = calloc(table->number_of_columns, sizeof (int));
    for (int i = 0; i < table->number_of_columns; i++) {
        table->column_widths[i] = (int) strlen(table->headers[i]) + table->column_padding * 2;
        table->max_cols += table->column_widths[i] + 1;
    }

    table->current_line = table->current_col = 0;

    table->pad = newpad(LINES, COLS);
}

void free_schedule_table(table_t* table) {
    free(table->headers);
    free(table->column_widths);
    delwin(table->pad);
}

void schedule_menu() {
    weekly_schedule_t weekly_schedule;
    table_t schedule_table;

    init_weekly_schedule(&weekly_schedule);
    load_weekly_schedule(&weekly_schedule, "schedule.txt");

    init_schedule_table(&schedule_table, weekly_schedule.days[0].n_elements + 10, 3);

    display_schedules(&schedule_table, weekly_schedule.days);
    wgetch(schedule_table.pad);

    free_schedule_table(&schedule_table);
    free_weekly_schedules(&weekly_schedule);
}
