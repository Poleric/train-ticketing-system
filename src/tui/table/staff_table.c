#include <tui/table/staff_table.h>
#include <tui/utils/tui_utils.h>
#include <tui/utils/menu_utils.h>
#include <utils.h>
#include <stdlib.h>
#include <string.h>

void init_staff_table(WINDOW* window, staff_table_t* staff_table, staff_vector_t* staffs, staff_t* current_staff) {
    staff_table->table.number_of_columns = 6;
    staff_table->table.headers = calloc(staff_table->table.number_of_columns, sizeof(const char*));
    staff_table->table.headers[0] = "Username";
    staff_table->table.headers[1] = "Email";
    staff_table->table.headers[2] = "Contact No";
    staff_table->table.headers[3] = "Position";
    staff_table->table.headers[4] = "Permissions";
    staff_table->table.headers[5] = "Salary";

    staff_table->table.column_widths = calloc(staff_table->table.number_of_columns, sizeof (int));
    staff_table->table.column_widths[0] = 1;
    staff_table->table.column_widths[1] = 3;
    staff_table->table.column_widths[2] = 2;
    staff_table->table.column_widths[3] = 1;
    staff_table->table.column_widths[4] = 3;
    staff_table->table.column_widths[5] = 2;

    staff_table->table.number_of_footers = 5;
    staff_table->table.footers = calloc(staff_table->table.number_of_footers, sizeof(const char*));
    staff_table->table.footers[0] = "[q] Quit";
    staff_table->table.footers[1] = "[↑↓] Scroll";
    if (current_staff->permissions & MANAGE_STAFF) {
        staff_table->table.footers[2] = "[R] Register";
        staff_table->table.footers[3] = "[D] Delete";
        staff_table->table.footers[4] = "[E] Edit";
    } else {
        staff_table->table.footers[2] = "";
        staff_table->table.footers[3] = "";
        staff_table->table.footers[4] = "";
    }

    staff_table->table.footer_widths = calloc(staff_table->table.number_of_footers, sizeof (int));
    staff_table->table.footer_widths[0] = 1;
    staff_table->table.footer_widths[1] = 1;
    staff_table->table.footer_widths[2] = 1;
    staff_table->table.footer_widths[3] = 1;
    staff_table->table.footer_widths[4] = 1;

    staff_table->table.window = window;

    staff_table->table.number_of_display_lines = getmaxy(window) - 3;

    staff_table->table.width = 0;
    for (int i = 0; i < staff_table->table.number_of_columns; i++) {
        staff_table->table.width += staff_table->table.column_widths[i];
    }

    staff_table->staffs = staffs;
    staff_table->current_staff = current_staff;

    staff_table->table.current_line = 0;
    staff_table->table.selected_line = 0;

    scale_table_to_window(&staff_table->table);

    keypad(staff_table->table.window, true);
    curs_set(0);
}

void print_staff_row(staff_table_t* staff_table, staff_t* staff) {
    int offset_x = 0;

    move_offset_x(staff_table->table.window, get_offset_for_centered((int)strlen(staff->username), staff_table->table.column_widths[0]));
    wprintw(staff_table->table.window, "%.*s", staff_table->table.column_widths[0] - 1, staff->username);
    offset_x += staff_table->table.column_widths[0];

    move_to_x(staff_table->table.window, offset_x);

    wprintw(staff_table->table.window, "%.*s", staff_table->table.column_widths[1] - 1, staff->email);
    offset_x += staff_table->table.column_widths[1];

    move_to_x(staff_table->table.window, offset_x);

    move_offset_x(staff_table->table.window, get_offset_for_centered((int)strlen(staff->contact_no), staff_table->table.column_widths[2]));
    wprintw(staff_table->table.window, "%.*s", staff_table->table.column_widths[2] - 1, staff->contact_no);
    offset_x += staff_table->table.column_widths[2];

    move_to_x(staff_table->table.window, offset_x);

    move_offset_x(staff_table->table.window, get_offset_for_centered((int)strlen(staff->position), staff_table->table.column_widths[3]));
    wprintw(staff_table->table.window, "%.*s", staff_table->table.column_widths[3] - 1, staff->position);
    offset_x += staff_table->table.column_widths[3];

    move_to_x(staff_table->table.window, offset_x);

    wprintw(staff_table->table.window, "%.*s", staff_table->table.column_widths[4] - 1, permission_to_string(staff->permissions));
    offset_x += staff_table->table.column_widths[4];

    move_to_x(staff_table->table.window, offset_x);

    move_offset_x(staff_table->table.window, get_offset_for_centered(get_number_of_digits_d((int)staff->salary) + 3, staff_table->table.column_widths[5]));
    wprintw(staff_table->table.window, "%.2lf", staff->salary);
}

void display_staffs(staff_table_t* staff_table) {
    wmove(staff_table->table.window, 0, 0);

    print_table_header(&staff_table->table, COLOR_2);

    move_to_next_line(staff_table->table.window, 0);

    for (
            int i = staff_table->table.current_line;
            i < staff_table->staffs->num_of_staff && i < staff_table->table.current_line + staff_table->table.number_of_display_lines;
            i++
            ) {
        print_staff_row(staff_table, staff_table->staffs->array[i]);
        move_to_next_line(staff_table->table.window, 0);
    }

    highlight_selected_row(&staff_table->table, 1, SELECTED);

    print_table_footer(&staff_table->table, COLOR_2);

    wrefresh(staff_table->table.window);
}

void free_staff_table(staff_table_t* staff_table) {
    keypad(staff_table->table.window, false);
    free_table(&staff_table->table);

    curs_set(1);
}

staff_t* get_selected_staff(staff_table_t* staff_table) {
    return staff_table->staffs->array[staff_table->table.selected_line];
}