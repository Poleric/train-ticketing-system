#ifndef TRAINTICKETINGSYSTEM_TABLE_H
#define TRAINTICKETINGSYSTEM_TABLE_H

#include <panel.h>

typedef struct Table table_t;

struct Table {
    const char **headers;
    int* column_widths;
    int number_of_columns;

    int max_lines, max_cols;
    int current_line, current_col;

    WINDOW* pad;
};

void print_table_header(table_t* table, short color_pair);
void scale_to_screen_size(table_t* table);
void highlight_selected_row(table_t* table, int header_offset, short selected_color_pair);
void free_table(table_t* table);

#endif //TRAINTICKETINGSYSTEM_TABLE_H
