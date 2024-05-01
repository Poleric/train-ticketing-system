#include <schedule.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <member.h>
#include <tui/scheduling_menu.h>

//ticket
typedef struct {
    char ticketID[10];  //TK001
    char seatNum[10];   //A001, B001
    char trainID[10];   //T001
    char username[10];  //Abu
    char station[10];   //Romah
    dt_time_t departure_time;
    dt_time_t eta;
}Ticket;

//train
typedef struct {
    char trainID[6];  // Unique identifier for the train (e.g., T001)
    char departureTime[10];  // Departure time of the train
    char arrivalTime[10];  // Arrival time of the train
    int numSeatsAvailable;  // Number of seats available on the train
    // Other relevant details about the train...
} Train;

void displaySchedule(Train *trains) {

}

void booking(Ticket *tickets, int *numTickets, Train *trains, struct datetime_time) {
    char trainID[10];
    char seat[10];
    char choice;

    do {
        printf("Booking Ticket\n");
        printf("==============\n");
        printf("Select Train ID : \n");
        scanf("%s",&trainID);
        printf("Select Your Time : \n");
        scanf("%d",&time);
        printf("Select Your Seat : \n");
        scanf("%s",&seat);

    }while(toupper(choice=='Y'));
}




bool valTicket(const char* ticketID) {
    FILE* fptr;
    fptr= fopen("valid_tickets.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file");
        return false;
    }

    char line[100];
    while (fgets(line, sizeof(line), fptr) != NULL) {
        // Remove newline character from the end of the line
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, ticketID) == 0) {
            fclose(fptr);
            return true; // Ticket ID is valid
        }
    }
    fclose(fptr);
    return false;
}

//check ticket
//book ticket
//ticket record
//get ticket from userid members