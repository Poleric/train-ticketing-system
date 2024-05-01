#ifndef TRAINTICKETINGSYSTEM_TABLE_H
#define TRAINTICKETINGSYSTEM_TABLE_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#include <curses.h>
#else
#include <ncursesw/ncurses.h>
#endif

typedef struct Table table_t;

struct Table {
    const char **headers;
    int* column_widths;
    int number_of_columns;

    const char **footers;
    int* footer_widths;
    int number_of_footers;

    int width;
    int number_of_display_lines;
    int current_line, selected_line;

    WINDOW* window;
};

void print_table_header(table_t* table, short color_pair);
void scale_table_to_window(table_t* table);
void highlight_selected_row(table_t* table, int header_offset, short selected_color_pair);
void print_table_footer(table_t* table, short color_pair);
void free_table(table_t* table);

#endif //TRAINTICKETINGSYSTEM_TABLE_H
