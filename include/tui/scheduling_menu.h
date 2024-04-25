#ifndef TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
#define TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H

#include <tui/tui_utils.h>
#include <tui/tui_common.h>
#include <schedule/stations.h>
#include <schedule/schedule.h>

typedef struct Table table_t;

struct Table {
    const char **headers;
    int* column_widths;
    int number_of_columns;
    int column_padding;

    int max_lines, max_cols;
    int current_line, current_col;

    WINDOW* pad;
};

void move_to_next_line(WINDOW* window, int x);
void move_to_x(WINDOW* window, int x);
void move_offset_x(WINDOW* window, int offset_x);
void print_header(table_t* table);
void print_footer(table_t* table);
char* to_time(dt_time_t time, char* buffer, int n);
void print_schedule_row(table_t* table, schedule_t * schedule);
void display_schedules(table_t* table, schedule_vector_t* schedule_vector);
void init_schedule_table(table_t* table, int n_lines, int column_padding);
void free_schedule_table(table_t* table);

void schedule_menu();

#endif //TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
