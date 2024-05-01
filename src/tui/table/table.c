#include <tui/table/table.h>
#include <tui/utils/tui_utils.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

void print_table_header(table_t* table, short color_pair) {
    int x = 0;
    for (int i = 0; i < table->number_of_columns; i++) {
        move_to_x(table->window, x);
        move_offset_x(table->window, get_offset_for_centered((int)strlen(table->headers[i]), table->column_widths[i]));
        wprintw(table->window, "%s", table->headers[i]);
        x += table->column_widths[i];
    }
    move_to_x(table->window, 0);
    wchgat(table->window, table->width, A_STANDOUT, color_pair, NULL);
}

void scale_table_to_window(table_t* table) {
    int max_x = getmaxx(table->window);

    // scale up / down all column widths
    const int header_max = sum_d(table->column_widths, table->number_of_columns);
    for (int i = 0; i < table->number_of_columns; i++)
        table->column_widths[i] = (int)((float)table->column_widths[i] / (float)header_max * (float)max_x);
    const int footer_max = sum_d(table->footer_widths, table->number_of_footers);
    for (int i = 0; i < table->number_of_footers; i++)
        table->footer_widths[i] = (int)((float)table->footer_widths[i] / (float)footer_max * (float)max_x);

    table->width = max_x;
}

void highlight_selected_row(table_t* table, int header_offset, short selected_color_pair) {
    wmove(table->window, table->selected_line + header_offset, 0);
    wchgat(table->window, table->width, A_STANDOUT, selected_color_pair, NULL);
}

void print_table_footer(table_t* table, short color_pair) {
    wmove(table->window, getmaxy(table->window) - 1, 0);

    int x = 0;
    for (int i = 0; i < table->number_of_footers; i++) {
        move_to_x(table->window, x);
        move_offset_x(table->window, get_offset_for_centered((int)strlen(table->footers[i]), table->footer_widths[i]));
        wprintw(table->window, "%s", table->footers[i]);
        x += table->footer_widths[i];
    }

    move_to_x(table->window, 0);
    wchgat(table->window, table->width, A_STANDOUT, color_pair, NULL);
}

void free_table(table_t* table) {
    free(table->headers);
    free(table->column_widths);
    free(table->footers);
    free(table->footer_widths);
    delwin(table->window);
}