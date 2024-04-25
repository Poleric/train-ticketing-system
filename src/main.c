/*#include <tui/main_menu.h>

int main() {
    start_menu();
    return 0;
}*/

#include "schedule.h"
#include <stdio.h>
#include <utils.h>
#include <tui/scheduling_menu.h>


void print_schedule(schedule_vector_t* schedules) {
    printf("+--------------+--------------+----------------+----------------+----------+\n");
    printf("| %-12s | %-12s | %-14s | %-14s | %-8s | \n",
        "From Station",
        "To Station",
        "Departure Time",
        "Arrival Time",
        "Train ID");
    printf("+--------------+--------------+----------------+----------------+----------+\n");

    char time_buffer[7], eta_buffer[7];
    for (int i = 0; i < schedules->n_elements; i++) {

        printf("| %11s -> %-12s | %-14s | %-14s | %-8s |\n",
            schedules->array[i]->from_station_id,
            schedules->array[i]->to_station_id,
            to_time(schedules->array[i]->departure_time, time_buffer, 7),
            to_time(schedules->array[i]->eta, eta_buffer, 7),
            schedules->array[i]->train_id
        );
    }
    printf("+-----------------------------+----------------+----------------+----------+\n");
}

void print_schedule_day(weekly_schedule_t* weekly_schedule, int tm_wday) {
    print_schedule(weekly_schedule->days + tm_wday);
}

void print_all_schedule(weekly_schedule_t* weekly_schedule) {
    for (int i = 0; i < 7; i++) {
        printf("%s Schedule\n", tm_wday_to_text(i));
        print_schedule_day(weekly_schedule, i);
        printf("\n");
    }
}

int main() {
    weekly_schedule_t weekly_schedule;

    init_weekly_schedule(&weekly_schedule);

    //    load_weekly_schedule(&weekly_schedule, "schedule.txt");

    //    From Station, To Station, Departure Time, Arrival Time, Train ID

    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 10 }, (dt_time_t) { 11, 30 }, 140, 0);
    weekly_add_schedule(&weekly_schedule, "T001", "MON", "COF", (dt_time_t) { 8, 10 }, (dt_time_t) { 10, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 0);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 0);

    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 10 }, (dt_time_t) { 10, 30 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 1);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 1);

    weekly_add_schedule(&weekly_schedule, "T001", "MON", "COF", (dt_time_t) { 8, 10 }, (dt_time_t) { 10, 30 }, 120, 2);
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 10 }, (dt_time_t) { 10, 30 }, 140, 2);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 2);

    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 3);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 3);

    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 4);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 4);

    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 5);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 5);

    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 6);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 6);

    //    schedule_t schedule = {
    //            "T003",
    //            "COF",
    //            "RMH",
    //            {13, 30},
    //            110
    //    };
    //    delete_schedule_by_info(weekly_schedule.days, &schedule);

    //    delete_schedule_by_info(weekly_schedule.days, weekly_schedule.days[0].array[2]);

    //    delete_schedule_by_train_time(weekly_schedule.days, "T003", (dt_time_t) {13, 30});

    save_weekly_schedule(&weekly_schedule, "schedule.txt");

    print_all_schedule(&weekly_schedule);

    free_weekly_schedules(&weekly_schedule);

    return 0;
}
