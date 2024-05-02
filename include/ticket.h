#ifndef TRAINTICKETINGSYSTEM_TICKET_H
#define TRAINTICKETINGSYSTEM_TICKET_H

#include <schedule.h>


typedef struct TrainTicket train_ticket_t;
typedef struct TrainTicketVector train_ticket_vector_t;

struct TrainTicket{
    unsigned long ticket_id;

    char train_id[5];
    time_t timestamp;

    char username[255];
    int seat;
};

struct TrainTicketVector {
    int max_size;
    train_ticket_t* array;
    int num_of_tickets;
};


unsigned long generate_ticket_id(char* train_id, char* username, time_t timestamp, int seat);
int create_ticket(train_ticket_t* ticket, char* train_id, char* username, time_t timestamp, int seat);

train_ticket_vector_t * init_train_ticket_vector();
int resize_train_ticket_vector(train_ticket_vector_t* train_tickets, int size);
int add_train_ticket(train_ticket_vector_t* train_tickets, train_ticket_t* train_ticket);

int get_ticket_history(train_ticket_vector_t* train_tickets, const char* filepath, const char* username);
int get_tickets_from_details(train_ticket_vector_t* train_tickets, const char* filepath, const char* train_id, time_t timestamp);
int save_train_tickets(train_ticket_vector_t* train_tickets, const char* filepath);

int get_number_of_booked_seats(const char* filepath, const char* train_id, time_t timestamp);

void free_train_ticket_vector(train_ticket_vector_t * members);



#endif //TRAINTICKETINGSYSTEM_TICKET_H
