#ifndef TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
#define TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H

#include <tui/table/table.h>
#include <stations.h>
#include <schedule.h>
#include <utils.h>

typedef struct ScheduleTable schedule_table_t;


struct ScheduleTable {
    table_t table;
    weekly_schedule_t *weekly_schedule;
    tm_wday_t selected_wday;
};

void init_schedule_table(WINDOW* window, schedule_table_t* schedule_table, weekly_schedule_t* weekly_schedules);
void print_day_header(schedule_table_t* schedule_table, short color_pair, short selected_color_pair);
void print_schedule_row(schedule_table_t* table, schedule_t* schedule);
void display_schedules(schedule_table_t* schedule_table);
void free_schedule_table(schedule_table_t* schedule_table);

void schedule_menu();

#endif //TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
