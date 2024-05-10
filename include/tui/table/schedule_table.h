#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_TABLE_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_TABLE_H

#include <tui/table/table.h>
#include <schedule.h>

#define FIRST_DAY_OF_WEEK SUNDAY
#define SELECTED_DAY_PADDING 5

typedef struct ScheduleTable schedule_table_t;

struct ScheduleTable {
    table_t table;
    weekly_schedule_t *weekly_schedule;
    tm_wday_t selected_wday;
    dt_date_t selected_date;
    dt_date_t today_date;
};

void init_schedule_table(WINDOW* window, schedule_table_t* schedule_table, weekly_schedule_t* weekly_schedules);

void print_schedule_table_day_header(schedule_table_t* schedule_table, short color_pair, short selected_color_pair);
void print_schedule_table_date_header(schedule_table_t* schedule_table);
void print_schedule_row(schedule_table_t* table, schedule_t* schedule);

void display_schedules(schedule_table_t* schedule_table);

void free_schedule_table(schedule_table_t* schedule_table);

schedule_t* get_selected_schedule(schedule_table_t* schedule_table);

#endif //TRAINTICKETINGSYSTEM_SCHEDULE_TABLE_H
