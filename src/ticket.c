#include <schedule.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <utils.h>
#include <tui/scheduling_menu.h>

int main_menu();
void displaySchedule(weekly_schedule_t* weekly_schedule);
void booking(weekly_schedule_t* weekly_schedule);
void cleanup(weekly_schedule_t* weekly_schedule);
void viewSchedule(weekly_schedule_t* weekly_schedule);

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

int main() {
    main_menu();
    return 0;
}

int main_menu() {
    weekly_schedule_t weekly_schedule;
    init_weekly_schedule(&weekly_schedule);
    booking(&weekly_schedule);
    displaySchedule(&weekly_schedule);
    cleanup(&weekly_schedule);
    char choice;

    do {
        printf("Main Menu\n");
        printf("1. Book Ticket\n");
        printf("2. View Schedule\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        switch (choice) {
            case '1':
                booking(&weekly_schedule);
                break;
            case '2':
                displaySchedule(&weekly_schedule);
                break;
            case '3':
                printf("Exiting the system.\n");
                cleanup(&weekly_schedule);
                return 0;
            default:
                printf("Invalid choice. Please try agian, Thank You.\n");
                break;
        }
    }while(1);

}

//1
void displaySchedule(weekly_schedule_t* weekly_schedule) {
    for(int i=0; i<7; i++) {
        printf("SUN ='0', MON ='1', TUE ='2', WED ='3', THU ='4', FRI ='5', SAT ='6'");
        printf("Choose the day of schedule : %d \n",i);

        for(int j=0;j<weekly_schedule->days[i].n_elements;j++) {
            schedule_t* schedule = weekly_schedule->days[i].array[j];
            printf("Train ID : %s \n",schedule->train_id);
            printf("From Station : %s \n",schedule->from_station_id);
            printf("To Station : %s \n",schedule->to_station_id);
            printf("Departure Time : %02d:%02d:%02d \n", schedule->departure_time.tm_hour, schedule->departure_time.tm_min, schedule->departure_time.tm_sec);
            printf("ETA : %02d:%02d:%02d \n", schedule->eta.tm_hour, schedule->eta.tm_min, schedule->eta.tm_sec);
            printf("Number of Seat Available : %d\n",schedule->n_seats);
            printf("\n\n");

        }
    }
}
//2
void booking(weekly_schedule_t* weekly_schedule) {
    char trainID[10];
    char seat[10];
    char choice;

    do {
        printf("Select Train ID : \n");
        scanf("%s",&trainID);
        printf("Select Your Seat : \n");
        scanf("%s",&seat);
        printf("Do you want to book another ticket? (Y/N) : ");
        scanf("%c",&choice);
        choice = toupper(choice);

    }while(choice == 'Y');

    do {
        printf("Booking successful! Your ticket details:\n");
        schedule_vector_t* last_day_schedule = &(weekly_schedule->days[6]);
        schedule_t* last_ticket = last_day_schedule->array[last_day_schedule->n_elements - 1];
        //maybe most buy ticket same destination
        printf("Train ID: %s\n", last_ticket->train_id);
        printf("From Station: %s\n", last_ticket->from_station_id);
        printf("To Station: %s\n", last_ticket->to_station_id);
        printf("Departure Time: %02d:%02d:%02d\n", last_ticket->departure_time.tm_hour,
               last_ticket->departure_time.tm_min, last_ticket->departure_time.tm_sec);
        printf("ETA: %02d:%02d:%02d\n", last_ticket->eta.tm_hour, last_ticket->eta.tm_min,
               last_ticket->eta.tm_sec);
        printf("Number of Seats Available: %d\n", last_ticket->n_seats);

        printf("Return to main menu? (Y/N) : ");
        scanf("%c",choice);
        choice=toupper(choice);
    }while(choice == 'Y');
}

void viewSchedule(weekly_schedule_t* weekly_schedule) {
    // Call the displaySchedule function to show the schedule
    displaySchedule(weekly_schedule);
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

int available_seat(char trainID, char departStation, int departTime, int MAX_TRAIN_SEAT) {
    FILE* ticketFP;
    Ticket temp;
    int seatCount;

    int seatAvailable[300];

    for (int i = 0; i < MAX_TRAIN_SEAT; i++) {
        seatAvailable[i] = i + 1;
    }

    ticketFP = fopen("MemberBooking.txt", "r");

    if (!ticketFP) {
        fprintf(stderr, "Error to open this file!\n");
        return EXIT_FAILURE;
    }

    while (ticketFP != EOF) {
        fscanf(ticketFP, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d:%d:%d|%d:%d:%d\n",
            temp.ticketID, temp.seatNum, temp.trainID, temp.username, temp.station,
            temp.departure_time.tm_hour, temp.departure_time.tm_min, temp.departure_time.tm_sec,
            temp.eta.tm_hour, temp.eta.tm_min, temp.eta.tm_sec);


    }

    fclose(ticketFP);
    return seatAvailable;
}

void cleanup(weekly_schedule_t* weekly_schedule) {
    // Iterate through each day of the week
    for (int i = 0; i < 7; i++) {
        schedule_vector_t* day_schedule = &(weekly_schedule->days[i]);

        // Iterate through the schedules for the current day
        for (int j = 0; j < day_schedule->n_elements; j++) {
            free(day_schedule->array[j]); // Free memory for individual schedule
        }

        // Free memory for the array of schedules
        free(day_schedule->array);
    }
}

//int main() {
//    // Sample usage
//    weekly_schedule_t weekly_schedule;
//    // Initialize weekly schedule
//    // (Assuming weekly_schedule is properly initialized)
//
//    // Example: Train ID, Time, Date
//    char train_id[] = "ABC123";
//    struct tm time = { .tm_hour = 10, .tm_min = 0, .tm_sec = 0 }; // 10:00:00
//    int tm_wday = 0; // Monday (0-indexed)
//
//    // Get the number of available seats
//    int available_seats = get_available_seats(&weekly_schedule, train_id, time, tm_wday);
//    printf("Available seats: %d\n", available_seats);
//
//    return 0;
//}