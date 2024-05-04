#ifndef TRAINTICKETINGSYSTEM_MEMBER_TICKET_TABLE_H
#define TRAINTICKETINGSYSTEM_MEMBER_TICKET_TABLE_H

#include <tui/table/table.h>
#include <member.h>
#include <ticket.h>

typedef struct MemberTicketTable member_ticket_table_t;

struct MemberTicketTable {
    table_t table;

    member_t* member;
    train_ticket_vector_t* tickets;
};

void init_member_ticket_table(WINDOW* window, member_ticket_table_t* member_ticket_table, member_t* member);
void print_member_ticket_table_day_header(member_ticket_table_t* schedule_table, short color_pair, short selected_color_pair);
void print_member_ticket_table_date_header(member_ticket_table_t* schedule_table);
void print_ticket_row(member_ticket_table_t* table, schedule_t* schedule);
void display_tickets(member_ticket_table_t* member_ticket_table);
void free_member_ticket_table(member_ticket_table_t* member_ticket_table);

#endif //TRAINTICKETINGSYSTEM_MEMBER_TICKET_TABLE_H
