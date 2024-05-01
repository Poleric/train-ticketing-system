#include <tui/table/table.h>
#include <tui/utils/tui_utils.h>
#include <stdlib.h>
#include <string.h>

void print_table_header(table_t* table, short color_pair) {
    int x = 0;
    for (int i = 0; i < table->number_of_columns; i++) {
        move_to_x(table->pad, x);
        move_offset_x(table->pad, get_offset_for_centered((int)strlen(table->headers[i]), table->column_widths[i]));
        wprintw(table->pad,  "%s", table->headers[i]);
        x += table->column_widths[i];
    }
    move_to_x(table->pad, 0);
    wchgat(table->pad, table->max_cols, A_STANDOUT, color_pair, NULL);
}

void scale_to_screen_size(table_t* table) {
    int max_x = getmaxx(table->pad);

    // scale up / down all column widths
    for (int i = 0; i < table->number_of_columns; i++)
        table->column_widths[i] *= max_x / table->max_cols;
    table->max_cols = max_x;
}

void free_table(table_t* table) {
    free(table->headers);
    free(table->column_widths);
    delwin(table->pad);
}