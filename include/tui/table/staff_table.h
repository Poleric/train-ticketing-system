#ifndef TRAINTICKETINGSYSTEM_STAFF_TABLE_H
#define TRAINTICKETINGSYSTEM_STAFF_TABLE_H

#include <tui/table/table.h>
#include <staff.h>

typedef struct StaffsTable staff_table_t;

struct StaffsTable {
    table_t table;
    staff_vector_t* staffs;
    staff_t* current_staff;
};

void init_staff_table(WINDOW* window, staff_table_t* staff_table, staff_vector_t* staffs, staff_t* current_staff);

void print_staff_row(staff_table_t* staff_table, staff_t* staff);

void display_staffs(staff_table_t* staff_table);

void free_staff_table(staff_table_t* staff_table);

staff_t* get_selected_staff(staff_table_t* staff_table);

#endif //TRAINTICKETINGSYSTEM_STAFF_TABLE_H
