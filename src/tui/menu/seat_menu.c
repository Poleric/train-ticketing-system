#include <tui/menu/seat_menu.h>
#include <tui/utils/menu_utils.h>
#include <string.h>

#define SEAT_MENU_TOP_MARGIN_PERCENTAGE 0.1
#define SEAT_MENU_SIDE_MARGIN_PERCENTAGE 0.2
#define SEAT_GRID_GAP 1
#define GRID_WIDTH 3
#define GRID_HEIGHT 1
#define SEATS_PER_ROW 4
#define SEAT_GRID_WIDTH (SEATS_PER_ROW * GRID_WIDTH + (SEATS_PER_ROW - 1) * SEAT_GRID_GAP)
#define ROWS_PER_PAGE 10

void init_seat_menu(seat_menu_t* seat_menu, WINDOW* menu_window, const char* header, char* menu_header, dt_date_t date, schedule_t* schedule) {
    init_int_menu(
            &seat_menu->menu,
            menu_window,
            -1,
            A_STANDOUT,
            SELECTED
    );
    seat_menu->menu.rows_per_page = ROWS_PER_PAGE;
    seat_menu->menu.options_per_row = SEATS_PER_ROW;

    seat_menu->header = header;
    seat_menu->header_y = 0;

    seat_menu->menu_header = menu_header;
    seat_menu->menu_header_y = (int)(getmaxy(menu_window) * SEAT_MENU_TOP_MARGIN_PERCENTAGE);

    seat_menu->menu.number_of_options = get_schedule_number_of_free_seats(TICKETS_FILEPATH, schedule, date);

    seat_menu->number_of_footers = 4;
    seat_menu->footers = calloc(seat_menu->number_of_footers, sizeof(const char*));
    seat_menu->footers[0] = "[↑↓] Scroll";
    seat_menu->footers[1] = "";
    seat_menu->footers[2] = "[s] Select Seat";
    seat_menu->footers[3] = "[Enter] Book";

    seat_menu->footer_widths = calloc(seat_menu->number_of_footers, sizeof(int));
    seat_menu->footer_widths[0] = 1;
    seat_menu->footer_widths[1] = 1;
    seat_menu->footer_widths[2] = 1;
    seat_menu->footer_widths[3] = 1;

    seat_menu->footer_y = getmaxy(menu_window) - 1;

    seat_menu->date = date;
    seat_menu->schedule = schedule;

    init_seats_locations(seat_menu);

    scale_seat_menu_to_window(seat_menu);
}

void init_seats_locations(seat_menu_t* seat_menu) {
    seat_menu->menu.number_of_options = seat_menu->schedule->n_seats;
    seat_menu->menu.menu_options = calloc(seat_menu->menu.number_of_options, sizeof(int_menu_option_t));

    train_ticket_vector_t* booked_tickets = init_train_ticket_vector();
    get_tickets_from_details(booked_tickets, TICKETS_FILEPATH, seat_menu->schedule->train_id, time_t_from_dt(seat_menu->date, seat_menu->schedule->departure_time));
    train_ticket_t** sorted_booked_tickets = sort_tickets_by_seat_no(booked_tickets);
    train_ticket_t** ticket_p = sorted_booked_tickets;

    const int start_x = get_centered_x_start(seat_menu->menu.window, SEAT_GRID_WIDTH);
    const int start_y = seat_menu->menu_header_y + 2;
    for (int i = 0, k = 0; i < seat_menu->schedule->n_seats; i++) {
        bool disabled = false;
        if (k < booked_tickets->num_of_tickets && i == ticket_p[k]->seat) {
            k++;
            disabled = true;
        }
        seat_menu->menu.menu_options[i] = (int_menu_option_t){
                i + 1,
                start_y + (i % (int)(SEATS_PER_ROW * ROWS_PER_PAGE) / SEATS_PER_ROW) * (GRID_HEIGHT + SEAT_GRID_GAP),  // magic numbers
                start_x + ((i % SEATS_PER_ROW) * (GRID_WIDTH + SEAT_GRID_GAP)),
                disabled,
                false
        };
    }

    free(sorted_booked_tickets);
    free_train_ticket_vector(booked_tickets);
}

void scale_seat_menu_to_window(seat_menu_t* seat_menu) {
    int max_x = getmaxx(seat_menu->menu.window);

    // scale up / down all column widths
    const int footer_max = sum_d(seat_menu->footer_widths, seat_menu->number_of_footers);
    for (int i = 0; i < seat_menu->number_of_footers; i++)
        seat_menu->footer_widths[i] = (int)((float)seat_menu->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_seat_menu_header(seat_menu_t* seat_menu, short color_pair) {
    wmove(seat_menu->menu.window, seat_menu->header_y, get_centered_x_start(seat_menu->menu.window, (int)strlen(seat_menu->header)));
    wprintw(seat_menu->menu.window, "%s", seat_menu->header);

    move_to_x(seat_menu->menu.window, 0);
    wchgat(seat_menu->menu.window, seat_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void print_seat_menu_menu_header(seat_menu_t* seat_menu, short color_pair) {
    wmove(seat_menu->menu.window, seat_menu->menu_header_y, get_centered_x_start(seat_menu->menu.window, (int)strlen(seat_menu->menu_header)));
    wprintw(seat_menu->menu.window, "%s", seat_menu->menu_header);

    const int start_x = (int)(seat_menu->menu.width * SEAT_MENU_SIDE_MARGIN_PERCENTAGE);
    const int width = seat_menu->menu.width - 2 * start_x;
    move_to_x(seat_menu->menu.window, start_x);
    wchgat(seat_menu->menu.window, width, A_STANDOUT, color_pair, NULL);
}

void print_seat_menu_footer(seat_menu_t* seat_menu, short color_pair) {
    wmove(seat_menu->menu.window, seat_menu->footer_y, 0);

    int x = 0;
    for (int i = 0; i < seat_menu->number_of_footers; i++) {
        move_to_x(seat_menu->menu.window, x);
        move_offset_x(seat_menu->menu.window, get_offset_for_centered((int)strlen(seat_menu->footers[i]), seat_menu->footer_widths[i]));
        wprintw(seat_menu->menu.window, "%s", seat_menu->footers[i]);
        x += seat_menu->footer_widths[i];
    }

    move_to_x(seat_menu->menu.window, 0);
    wchgat(seat_menu->menu.window, seat_menu->menu.width, A_STANDOUT, color_pair, NULL);
}

void display_seat_menu(seat_menu_t* seat_menu, short color_pair) {
    print_seat_menu_header(seat_menu, color_pair);

    print_seat_menu_menu_header(seat_menu, color_pair);

    print_int_menu_options(&seat_menu->menu);

    print_seat_menu_footer(seat_menu, color_pair);

    select_int_menu_option(&seat_menu->menu, seat_menu->menu.selected_option);

    wrefresh(seat_menu->menu.window);
}

int get_current_selected_seat(seat_menu_t* seat_menu) {
    return seat_menu->menu.menu_options[seat_menu->menu.selected_option].label;
}

void free_seat_menu(seat_menu_t* seat_menu) {
    cleanup_int_menu(&seat_menu->menu);

    free(seat_menu->menu.menu_options);
    free(seat_menu->footers);
    free(seat_menu->footer_widths);
}