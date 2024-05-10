#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_FORM_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_FORM_H

#include <tui/form/form.h>
#include <utils.h>

typedef struct ScheduleForm schedule_form_t;

struct ScheduleForm {
    form_t form;

    const char* header;
    int header_y;

    const char* form_header;
    int form_header_y;

    int form_message_y;

    const char **footers;
    int* footer_widths;
    int number_of_footers;
    int footer_y;
};

void init_schedule_form(schedule_form_t* schedule_form, WINDOW* form_window, const char* header, const char* form_header);

void scale_schedule_form_to_window(schedule_form_t* schedule_form);

void print_schedule_form_fields(schedule_form_t* schedule_form);
void print_schedule_form_header(schedule_form_t* schedule_form, short color_pair);
void print_schedule_form_form_header(schedule_form_t* schedule_form, short color_pair);
void print_schedule_form_footer(schedule_form_t* schedule_form, short color_pair);
void print_schedule_form_message(schedule_form_t* schedule_form, const char* message, short color_pair);

void display_schedule_form(schedule_form_t* schedule_form, short color_pair);

void free_schedule_form(schedule_form_t* schedule_form);

char* get_schedule_form_train_id(schedule_form_t* schedule_form);
char* get_schedule_form_from_station_id(schedule_form_t* schedule_form);
char* get_schedule_form_to_station_id(schedule_form_t* schedule_form);
tm_wday_t get_schedule_form_weekday(schedule_form_t* schedule_form);
dt_time_t get_schedule_form_departure_time(schedule_form_t* schedule_form);
dt_time_t get_schedule_form_eta(schedule_form_t* schedule_form);
int get_schedule_form_number_of_seats(schedule_form_t* schedule_form);
double get_schedule_form_price(schedule_form_t* schedule_form);

bool validate_schedule_form_train_id(schedule_form_t* schedule_form);
bool validate_schedule_form_from_station_id(schedule_form_t* schedule_form);
bool validate_schedule_form_to_station_id(schedule_form_t* schedule_form);
bool validate_schedule_form_weekday(schedule_form_t* schedule_form);
bool validate_schedule_form_departure_time(schedule_form_t* schedule_form);
bool validate_schedule_form_eta(schedule_form_t* schedule_form);
bool validate_schedule_form_number_of_seats(schedule_form_t* schedule_form);
bool validate_schedule_form_price(schedule_form_t* schedule_form);

#endif //TRAINTICKETINGSYSTEM_SCHEDULE_FORM_H
