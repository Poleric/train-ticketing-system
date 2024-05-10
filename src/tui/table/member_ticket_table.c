#include <tui/table/member_ticket_table.h>
#include <tui/utils/tui_utils.h>
#include <tui/utils/menu_utils.h>
#include <string.h>
#include <ticket.h>

void init_member_ticket_table(WINDOW* window, member_ticket_table_t* member_ticket_table, member_t* member) {
    member_ticket_table->table.number_of_columns = 5;
    member_ticket_table->table.headers = calloc(member_ticket_table->table.number_of_columns, sizeof(const char*));
    member_ticket_table->table.headers[0] = "Train ID";
    member_ticket_table->table.headers[1] = "Departure Date";
    member_ticket_table->table.headers[2] = "Departure Time";
    member_ticket_table->table.headers[3] = "Ordered on";
    member_ticket_table->table.headers[4] = "Seat No";

    member_ticket_table->table.column_widths = calloc(member_ticket_table->table.number_of_columns, sizeof (int));
    member_ticket_table->table.column_widths[0] = 10;
    member_ticket_table->table.column_widths[1] = 10;
    member_ticket_table->table.column_widths[2] = 10;
    member_ticket_table->table.column_widths[3] = 14;
    member_ticket_table->table.column_widths[4] = 6;

    member_ticket_table->table.number_of_footers = 5;
    member_ticket_table->table.footers = calloc(member_ticket_table->table.number_of_footers, sizeof(const char*));
    member_ticket_table->table.footers[0] = "[q] Quit";
    member_ticket_table->table.footers[1] = "";
    member_ticket_table->table.footers[2] = "[↑↓] Scroll";
    member_ticket_table->table.footers[3] = "[D] Refund";
    member_ticket_table->table.footers[4] = "[D] Change Seat";

    member_ticket_table->table.footer_widths = calloc(member_ticket_table->table.number_of_footers, sizeof (int));
    member_ticket_table->table.footer_widths[0] = 1;
    member_ticket_table->table.footer_widths[1] = 1;
    member_ticket_table->table.footer_widths[2] = 1;
    member_ticket_table->table.footer_widths[3] = 1;
    member_ticket_table->table.footer_widths[4] = 1;

    member_ticket_table->member = member;
    member_ticket_table->tickets = init_train_ticket_vector();
    get_ticket_history(member_ticket_table->tickets, TICKETS_FILEPATH, member->username);

    member_ticket_table->table.window = window;

    member_ticket_table->table.number_of_display_lines = getmaxy(window) - 2;

    member_ticket_table->table.width = 0;
    for (int i = 0; i < member_ticket_table->table.number_of_columns; i++) {
        member_ticket_table->table.width += member_ticket_table->table.column_widths[i];
    }

    scale_table_to_window(&member_ticket_table->table);

    member_ticket_table->table.current_line = 0;
    member_ticket_table->table.selected_line = 0;

    keypad(member_ticket_table->table.window, true);
    curs_set(0);
}

void print_member_ticket_table_row(member_ticket_table_t* member_ticket_table, train_ticket_t* ticket) {
    int offset_x = 0;
    dt_date_t departure_date, order_date;
    dt_time_t departure_time, order_time;
    dt_from_timestamp(ticket->timestamp, &departure_date, &departure_time, NULL);
    dt_from_timestamp(ticket->order_timestamp, &order_date, &order_time, NULL);

    move_offset_x(member_ticket_table->table.window, get_offset_for_centered((int)strlen(ticket->train_id), member_ticket_table->table.column_widths[0]));
    wprintw(member_ticket_table->table.window, "%s", ticket->train_id);
    offset_x += member_ticket_table->table.column_widths[0];

    move_to_x(member_ticket_table->table.window, offset_x);

    move_offset_x(member_ticket_table->table.window, get_offset_for_centered(10, member_ticket_table->table.column_widths[1]));
    wprintw(member_ticket_table->table.window, "%d-%d-%d", departure_date.tm_year, departure_date.tm_mon, departure_date.tm_mday);
    offset_x += member_ticket_table->table.column_widths[1];

    move_to_x(member_ticket_table->table.window, offset_x);

    move_offset_x(member_ticket_table->table.window, get_offset_for_centered(5, member_ticket_table->table.column_widths[2]));
    wprintw(member_ticket_table->table.window, "%d:%d", departure_time.tm_hour, departure_time.tm_min);
    offset_x += member_ticket_table->table.column_widths[2];

    move_to_x(member_ticket_table->table.window, offset_x);

    move_offset_x(member_ticket_table->table.window, get_offset_for_centered(16, member_ticket_table->table.column_widths[3]));
    wprintw(member_ticket_table->table.window, "%d-%d-%d %d:%d", order_date.tm_year, order_date.tm_mon, order_date.tm_mday, order_time.tm_hour, order_time.tm_min);
    offset_x += member_ticket_table->table.column_widths[3];

    move_to_x(member_ticket_table->table.window, offset_x);

    move_offset_x(member_ticket_table->table.window, get_offset_for_centered(get_number_of_digits_d(ticket->seat), member_ticket_table->table.column_widths[4]));
    wprintw(member_ticket_table->table.window, "%d", ticket->seat);
}

void display_tickets(member_ticket_table_t* member_ticket_table) {
    wmove(member_ticket_table->table.window, 0, 0);

    print_table_header(&member_ticket_table->table, COLOR_1);

    move_to_next_line(member_ticket_table->table.window, 0);

    for (
            int i = member_ticket_table->table.current_line;
            i < member_ticket_table->tickets->num_of_tickets && i < member_ticket_table->table.current_line + member_ticket_table->table.number_of_display_lines;
            i++
            ) {
        print_member_ticket_table_row(member_ticket_table, member_ticket_table->tickets->array + i);
        move_to_next_line(member_ticket_table->table.window, 0);
    }

    highlight_selected_row(&member_ticket_table->table, 1, SELECTED);

    print_table_footer(&member_ticket_table->table, COLOR_1);

    wrefresh(member_ticket_table->table.window);
}

void free_member_ticket_table(member_ticket_table_t* member_ticket_table) {
    keypad(member_ticket_table->table.window, false);
    free_table(&member_ticket_table->table);
    free_train_ticket_vector(member_ticket_table->tickets);

    curs_set(1);
}

