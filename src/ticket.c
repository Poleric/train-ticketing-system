#include <ticket.h>
#include <stdio.h>
#include <string.h>
#include <utils.h>
#include <assert.h>

#define BASE_TRAIN_TICKETS_LENGTH 8

unsigned long generate_ticket_id(char* train_id, char* username, time_t timestamp, int seat) {
    unsigned long hash = 5381;
    int c;

    while ((c = (unsigned char)*train_id++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    while ((c = (unsigned char)*username++))
        hash = ((hash << 5) + hash) + c;

    hash = ((hash << 5) + hash) + timestamp;
    hash = ((hash << 5) + hash) + seat;

    return hash;
}

int create_ticket(train_ticket_t* ticket, char* train_id, char* username, time_t timestamp,time_t order_timestamp,  int seat) {
    ticket->ticket_id = generate_ticket_id(train_id, username, timestamp, seat);
    strncpy(ticket->train_id, train_id, 5);
    strncpy(ticket->username, username, 255);
    ticket->timestamp = timestamp;
    ticket->order_timestamp = order_timestamp;
    ticket->seat = seat;
    return EXIT_SUCCESS;
}

train_ticket_vector_t * init_train_ticket_vector() {
    train_ticket_vector_t * train_tickets = malloc(sizeof(train_ticket_vector_t));
    if (train_tickets == NULL)
        return NULL;

    train_tickets->max_size = BASE_TRAIN_TICKETS_LENGTH;
    train_tickets->array = calloc(sizeof(train_ticket_t *), train_tickets->max_size);
    train_tickets->num_of_tickets = 0;

    return train_tickets;
}

int resize_train_ticket_vector(train_ticket_vector_t* train_tickets, int size) {
    assert(size > train_tickets->num_of_tickets);

    train_ticket_t *tmp = realloc(train_tickets->array, sizeof (train_ticket_t) * size);

    if (tmp == NULL)
        return EXIT_FAILURE;

    train_tickets->max_size = size;
    train_tickets->array = tmp;
    return EXIT_SUCCESS;
}

int get_ticket_history(train_ticket_vector_t* train_tickets, const char* filepath, const char* username) {
    FILE* fptr;
    fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    train_ticket_t buffer;
    while (fread(&buffer, sizeof(train_ticket_t), 1, fptr) != 0)
        if (strcmp(buffer.username, username) == 0)
            add_train_ticket(train_tickets, &buffer);

    fclose(fptr);
    return EXIT_SUCCESS;
}

int get_tickets_from_details(train_ticket_vector_t* train_tickets, const char* filepath, const char* train_id, time_t timestamp) {
    FILE* fptr;
    fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    train_ticket_t buffer;
    while (fread(&buffer, sizeof(train_ticket_t), 1, fptr) != 0)
        if (strcmp(buffer.train_id, train_id) == 0 && buffer.timestamp == timestamp)
            add_train_ticket(train_tickets, &buffer);

    fclose(fptr);
    return EXIT_SUCCESS;
}

int save_train_tickets(train_ticket_vector_t* train_tickets, const char* filepath) {
    /*
     * Save and update ticket details based on the array of tickets given. Matches row by ticket_id
     */
    FILE* fptr;
    fptr = fopen(filepath, "rb+");

    if (fptr == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    int number_of_tickets = train_tickets->num_of_tickets;
    train_ticket_t** tmp_array = calloc(number_of_tickets, sizeof(train_ticket_t*));

    train_ticket_t buffer;
    // gg O(n^2)
    while (fread(&buffer, sizeof(train_ticket_t), 1, fptr) != 0)
        for (int i = 0; i < number_of_tickets; i++)
            if (buffer.train_id == tmp_array[i]->train_id) {
                fseek(fptr, (long) -sizeof(train_ticket_t), SEEK_CUR);
                fwrite(tmp_array[i], sizeof(train_ticket_t), 1, fptr);

                // delete from tmp
                for (int j = i + 1; j < number_of_tickets; j++)
                    tmp_array[j - 1] = tmp_array[j];
                tmp_array[--number_of_tickets] = NULL;
            }
    for (int i = 0; i < number_of_tickets; i++)
        fwrite(tmp_array[i], sizeof(train_ticket_t), 1, fptr);


    free(tmp_array);
    fclose(fptr);

    assert(number_of_tickets == 0);
    return EXIT_SUCCESS;
}

int add_train_ticket(train_ticket_vector_t* train_tickets, train_ticket_t* train_ticket) {
    if (train_tickets->num_of_tickets + 1 > train_tickets->max_size)
        if (resize_train_ticket_vector(train_tickets, train_tickets->max_size * 2) == EXIT_FAILURE)
            return EXIT_FAILURE;

    train_tickets->array[train_tickets->num_of_tickets++] = *train_ticket;
    return EXIT_SUCCESS;
}

int get_number_of_booked_seats(const char* filepath, dt_date_t date, schedule_t* schedule) {
    int n = 0;

    FILE* fptr;
    fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return 0;
    }


    time_t timestamp = time_t_from_dt(date, schedule->departure_time);

    train_ticket_t buffer;
    fseek(fptr, sizeof(int), SEEK_SET);
    while (fread(&buffer, sizeof(train_ticket_t), 1, fptr) != 0)
        if (strcmp(buffer.train_id, schedule->train_id) == 0 && buffer.timestamp == timestamp)
            n++;

    fclose(fptr);

    return n;
}

int book_ticket(const char* filepath, dt_date_t date, schedule_t* schedule, char* username, time_t order_timestamp, int seat) {
    train_ticket_t ticket;

    time_t timestamp = time_t_from_dt(date, schedule->departure_time);
    create_ticket(&ticket, schedule->train_id, username, timestamp, order_timestamp, seat);

    FILE* fptr;
    fptr = fopen(filepath, "ab");

    if (fptr == NULL) {
//        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    fwrite(&ticket, sizeof(train_ticket_t), 1, fptr);

    fclose(fptr);

    return EXIT_SUCCESS;
}

void free_train_ticket_vector(train_ticket_vector_t * members) {
    free(members->array);
    free(members);
}
