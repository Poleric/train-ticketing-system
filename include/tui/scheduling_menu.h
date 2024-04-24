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

void schedule_menu();

#endif //TRAINTICKETINGSYSTEM_SCHEDULING_MENU_H
