#include <tui/form/schedule_form.h>
#include <string.h>

#define SCHEDULE_TRAIN_ID_LABEL         "Train ID                      :"
#define SCHEDULE_FROM_STATION_ID_LABEL  "From Station                  :"
#define SCHEDULE_TO_STATION_ID_LABEL    "To Station                    :"
#define SCHEDULE_WEEKDAY_LABEL          "Weekday           (0 - Sunday):"
#define SCHEDULE_DEPART_TIME_LABEL      "Departure Time         (HH:MM):"
#define SCHEDULE_ETA_LABEL              "Estimated Arrival Time (HH:MM):"
#define SCHEDULE_NUMBER_OF_SEATS_LABEL  "Number of seats               :"
#define SCHEDULE_PRICE_LABEL            "Price per seat                :"

#define SCHEDULE_LABEL_FIELD_LENGTH strlen(SCHEDULE_TRAIN_ID_LABEL)
#define SCHEDULE_FIELD_GAP 1

#define SCHEDULE_FORM_SIDE_MARGIN_PERCENTAGE 0.1
#define SCHEDULE_FORM_TOP_MARGIN_PERCENTAGE 0.2

void init_schedule_form(schedule_form_t* schedule_form, WINDOW* form_window, const char* header, const char* form_header) {
    const int FIELD_LABEL_WIDTH = SCHEDULE_LABEL_FIELD_LENGTH;

    init_form(
            &schedule_form->form,
            form_window,
            8,
            MAX(
                    getmaxx(form_window) - FIELD_LABEL_WIDTH - SCHEDULE_FIELD_GAP - (int)(getmaxx(form_window) * SCHEDULE_FORM_SIDE_MARGIN_PERCENTAGE * 2),
                    20
            )
    );

    schedule_form->form.field_label_width = FIELD_LABEL_WIDTH;

    schedule_form->header = header;
    schedule_form->header_y = 0;

    schedule_form->form_header = form_header;
    schedule_form->form_header_y = (int)(getmaxy(form_window) * SCHEDULE_FORM_TOP_MARGIN_PERCENTAGE);

    schedule_form->form.fields[0].label = SCHEDULE_TRAIN_ID_LABEL;
    schedule_form->form.fields[0].y = schedule_form->form_header_y + 2;
    schedule_form->form.fields[0].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[0].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[0].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[1].label = SCHEDULE_FROM_STATION_ID_LABEL;
    schedule_form->form.fields[1].y = schedule_form->form_header_y + 4;
    schedule_form->form.fields[1].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[1].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[2].label = SCHEDULE_TO_STATION_ID_LABEL;
    schedule_form->form.fields[2].y = schedule_form->form_header_y + 5;
    schedule_form->form.fields[2].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[2].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[3].label = SCHEDULE_WEEKDAY_LABEL;
    schedule_form->form.fields[3].y = schedule_form->form_header_y + 7;
    schedule_form->form.fields[3].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[3].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[4].label = SCHEDULE_DEPART_TIME_LABEL;
    schedule_form->form.fields[4].y = schedule_form->form_header_y + 9;
    schedule_form->form.fields[4].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[4].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[5].label = SCHEDULE_ETA_LABEL;
    schedule_form->form.fields[5].y = schedule_form->form_header_y + 10;
    schedule_form->form.fields[5].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[5].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[6].label = SCHEDULE_NUMBER_OF_SEATS_LABEL;
    schedule_form->form.fields[6].y = schedule_form->form_header_y + 12;
    schedule_form->form.fields[6].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[6].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);
    schedule_form->form.fields[7].label = SCHEDULE_PRICE_LABEL;
    schedule_form->form.fields[7].y = schedule_form->form_header_y + 13;
    schedule_form->form.fields[7].offset_x = FIELD_LABEL_WIDTH + SCHEDULE_FIELD_GAP;
    schedule_form->form.fields[7].x = get_centered_x_start(schedule_form->form.window, schedule_form->form.fields[1].offset_x + (int)schedule_form->form.buffer_length);

    schedule_form->number_of_footers = 2;
    schedule_form->footers = calloc(schedule_form->number_of_footers, sizeof(const char*));
    schedule_form->footers[0] = "[Ctrl + C] Quit";
    schedule_form->footers[1] = "";

    schedule_form->footer_widths = calloc(schedule_form->number_of_footers, sizeof(int));
    schedule_form->footer_widths[0] = 1;
    schedule_form->footer_widths[1] = 4;

    schedule_form->form_message_y = schedule_form->form_header_y + 15;

    schedule_form->footer_y = getmaxy(form_window) - 1;

    scale_schedule_form_to_window(schedule_form);
}

void scale_schedule_form_to_window(schedule_form_t* schedule_form) {
    int max_x = getmaxx(schedule_form->form.window);

    // scale up / down all column widths
    const int footer_max = sum_d(schedule_form->footer_widths, schedule_form->number_of_footers);
    for (int i = 0; i < schedule_form->number_of_footers; i++)
        schedule_form->footer_widths[i] = (int)((float)schedule_form->footer_widths[i] / (float)footer_max * (float)max_x);
}

void print_schedule_form_fields(schedule_form_t* schedule_form) {
    int old_row = schedule_form->form.selection_row;
    schedule_form->form.selection_row = 0;
    for (int i = 0; i < schedule_form->form.number_of_fields; i++) {
        print_current_field_label(&schedule_form->form);
        print_current_field_buffer(&schedule_form->form);
        schedule_form->form.selection_row++;
    }
    schedule_form->form.selection_row = old_row;
}

void print_schedule_form_header(schedule_form_t* schedule_form, short color_pair) {
    wmove(schedule_form->form.window, schedule_form->header_y, get_centered_x_start(schedule_form->form.window, (int)strlen(schedule_form->header)));
    wprintw(schedule_form->form.window, "%s", schedule_form->header);

    move_to_x(schedule_form->form.window, 0);
    wchgat(schedule_form->form.window, schedule_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_schedule_form_form_header(schedule_form_t* schedule_form, short color_pair) {
    wmove(schedule_form->form.window, schedule_form->form_header_y, get_centered_x_start(schedule_form->form.window, (int)strlen(schedule_form->form_header)));
    wprintw(schedule_form->form.window, "%s", schedule_form->form_header);

    const int width = schedule_form->form.fields[0].offset_x + (int)schedule_form->form.buffer_length;
    move_to_x(schedule_form->form.window, schedule_form->form.fields[0].x);
    wchgat(schedule_form->form.window, width, A_STANDOUT, color_pair, NULL);
}

void print_schedule_form_footer(schedule_form_t* schedule_form, short color_pair) {
    wmove(schedule_form->form.window, schedule_form->footer_y, 0);

    int x = 0;
    for (int i = 0; i < schedule_form->number_of_footers; i++) {
        move_to_x(schedule_form->form.window, x);
        move_offset_x(schedule_form->form.window, get_offset_for_centered((int)strlen(schedule_form->footers[i]), schedule_form->footer_widths[i]));
        wprintw(schedule_form->form.window, "%s", schedule_form->footers[i]);
        x += schedule_form->footer_widths[i];
    }

    move_to_x(schedule_form->form.window, 0);
    wchgat(schedule_form->form.window, schedule_form->form.width, A_STANDOUT, color_pair, NULL);
}

void print_schedule_form_message(schedule_form_t* schedule_form, const char* message, short color_pair) {
    wmove(schedule_form->form.window, schedule_form->form_message_y, 0);
    wclrtoeol(schedule_form->form.window);

    move_to_x(schedule_form->form.window, get_centered_x_start(schedule_form->form.window, (int)strlen(message)));
    wattron(schedule_form->form.window, COLOR_PAIR(color_pair));
    wprintw(schedule_form->form.window, "%s", message);
    wattroff(schedule_form->form.window, COLOR_PAIR(color_pair));
}

void display_schedule_form(schedule_form_t* schedule_form, short color_pair) {
    print_schedule_form_header(schedule_form, color_pair);

    print_schedule_form_form_header(schedule_form, color_pair);

    print_schedule_form_fields(schedule_form);

    print_schedule_form_footer(schedule_form, color_pair);

    move_cursor_to_input_field(&schedule_form->form);

    wrefresh(schedule_form->form.window);
}

void free_schedule_form(schedule_form_t* schedule_form) {
    cleanup_form(&schedule_form->form);
    free(schedule_form->footers);
    free(schedule_form->footer_widths);
}

char* get_schedule_form_train_id(schedule_form_t* schedule_form) {
    return schedule_form->form.fields[0].buffer;
}

char* get_schedule_form_from_station_id(schedule_form_t* schedule_form) {
    return schedule_form->form.fields[1].buffer;
}

char* get_schedule_form_to_station_id(schedule_form_t* schedule_form) {
    return schedule_form->form.fields[2].buffer;
}

tm_wday_t get_schedule_form_weekday(schedule_form_t* schedule_form) {
    return (int)strtol(schedule_form->form.fields[3].buffer, NULL, 10);
}

dt_time_t get_schedule_form_departure_time(schedule_form_t* schedule_form) {
    dt_time_t time = {-1, -1, 0};
    sscanf(schedule_form->form.fields[4].buffer, "%d:%d", &time.tm_hour, &time.tm_min);
    return time;
}

dt_time_t get_schedule_form_eta(schedule_form_t* schedule_form) {
    dt_time_t time = {0};
    sscanf(schedule_form->form.fields[5].buffer, "%d:%d", &time.tm_hour, &time.tm_min);
    return time;
}

int get_schedule_form_number_of_seats(schedule_form_t* schedule_form) {
    return (int)strtol(schedule_form->form.fields[6].buffer, NULL, 10);
}

double get_schedule_form_price(schedule_form_t* schedule_form) {
    return strtod(schedule_form->form.fields[7].buffer, NULL);
}

bool validate_schedule_form_train_id(schedule_form_t* schedule_form) {
    return strlen(get_schedule_form_train_id(schedule_form)) > 0 && strlen(get_schedule_form_train_id(schedule_form)) <= 4;
}

bool validate_schedule_form_from_station_id(schedule_form_t* schedule_form) {
    return strlen(get_schedule_form_from_station_id(schedule_form)) > 0 && strlen(get_schedule_form_from_station_id(schedule_form)) <= 3;
}

bool validate_schedule_form_to_station_id(schedule_form_t* schedule_form) {
    return strlen(get_schedule_form_to_station_id(schedule_form)) > 0 && strlen(get_schedule_form_to_station_id(schedule_form)) <= 3;
}

bool validate_schedule_form_weekday(schedule_form_t* schedule_form) {
    return 0 <= get_schedule_form_weekday(schedule_form) && get_schedule_form_weekday(schedule_form) < 7;
}

bool validate_schedule_form_departure_time(schedule_form_t* schedule_form) {
    dt_time_t time = get_schedule_form_departure_time(schedule_form);
    if (time.tm_hour < 0 || time.tm_min < 0)
        return false;
    if (time.tm_hour > 23 || time.tm_min > 59)
        return false;
    return true;
}

bool validate_schedule_form_eta(schedule_form_t* schedule_form) {
    dt_time_t time = get_schedule_form_eta(schedule_form);
    if (time.tm_hour < 0 || time.tm_min < 0)
        return false;
    if (time.tm_hour > 23 || time.tm_min > 59)
        return false;
    return true;
}

bool validate_schedule_form_number_of_seats(schedule_form_t* schedule_form) {
    return get_schedule_form_number_of_seats(schedule_form) > 0;
}

bool validate_schedule_form_price(schedule_form_t* schedule_form) {
    return get_schedule_form_price(schedule_form) > 0;
}


