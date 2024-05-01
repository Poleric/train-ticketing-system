#include <schedule.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <utils.h>

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

    //show schedule

    //user see schedule and choose train

    //show schedule details

    //user choose seat

    do {
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

// Function to get the number of available seats
int get_available_seats(weekly_schedule_t* weekly_schedule, char* train_id, struct tm time, int tm_wday) {
    int available_seats = 0;

    // Iterate through the schedules for the given day of the week
    for (int i = 0; i < weekly_schedule->days[tm_wday].n_elements; i++) {
        schedule_t* schedule = weekly_schedule->days[tm_wday].array[i];

        // Check if the train ID and departure time match
        if (strcmp(schedule->train_id, train_id) == 0 && is_time_same(schedule->departure_time, (dt_time_t){time.tm_hour, time.tm_min, time.tm_sec})) {
            // Calculate available seats
            available_seats += schedule->n_seats;
        }
    }

    return available_seats;
}

int main() {
    // Sample usage
    weekly_schedule_t weekly_schedule;
    // Initialize weekly schedule
    // (Assuming weekly_schedule is properly initialized)

    // Example: Train ID, Time, Date
    char train_id[] = "ABC123";
    struct tm time = { .tm_hour = 10, .tm_min = 0, .tm_sec = 0 }; // 10:00:00
    int tm_wday = 0; // Monday (0-indexed)

    // Get the number of available seats
    int available_seats = get_available_seats(&weekly_schedule, train_id, time, tm_wday);
    printf("Available seats: %d\n", available_seats);

    return 0;
}