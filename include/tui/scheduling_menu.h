#ifndef TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
#define TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H

#include "tui/utils/tui_utils.h"
#include "tui/utils/tui_common.h"
#include "stations.h"
#include "schedule.h"

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

void print_header(table_t* table);
void print_footer(table_t* table);
void print_schedule_row(table_t* table, schedule_t * schedule);
void display_schedules(table_t* table, schedule_vector_t* schedule_vector);
void init_schedule_table(table_t* table, int n_lines, int column_padding);
void free_schedule_table(table_t* table);

void schedule_menu();

#endif //TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
