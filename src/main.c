/*#include <tui/main_menu.h>

int main() {
    start_menu();
    return 0;
}*/

#include <schedule/schedule.h>
#include <stdio.h>
#include <utils.h>
#include <tui/scheduling_menu.h>

//    schedule_t schedule = {
//            "T003",   train id
//            "COF",    from
//            "RMH",    to
//            {13, 30}, dp,ar
//            110
//    };

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

    //SUNDAY
    weekly_add_schedule(&weekly_schedule, "T001", "MON", "COF", (dt_time_t) { 8, 10 }, (dt_time_t) { 10, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 10 }, (dt_time_t) { 11, 30 }, 140, 0);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 0);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "ROM", "MIL", (dt_time_t) { 12 }, (dt_time_t) { 12, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MIL", "NAP", (dt_time_t) { 13 }, (dt_time_t) { 14, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "NAP", "PAL", (dt_time_t) { 13, 30 }, (dt_time_t) { 14 }, 150, 0);
    weekly_add_schedule(&weekly_schedule, "T008", "PAL", "FIR", (dt_time_t) { 14, 15 }, (dt_time_t) { 15 }, 140, 0);
    weekly_add_schedule(&weekly_schedule, "T009", "FIR", "VEN", (dt_time_t) { 16 }, (dt_time_t) { 16, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "VEN", "SIE", (dt_time_t) { 17 }, (dt_time_t) { 17, 15 }, 130, 0);
    weekly_add_schedule(&weekly_schedule, "T011", "SIE", "URN", (dt_time_t) { 18, 10 }, (dt_time_t) { 18, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T012", "LPO", "COF", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T013", "NAP", "ROM", (dt_time_t) { 21 }, (dt_time_t) { 22 }, 140, 0);

    //MONDAY
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 8, 45 }, (dt_time_t) { 9, 30 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 10, 10 }, (dt_time_t) { 10, 30 }, 110, 1);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 11 }, (dt_time_t) { 12, 30 }, 120, 1);
    weekly_add_schedule(&weekly_schedule, "T001", "ROM", "MIL", (dt_time_t) { 11, 30 }, (dt_time_t) { 13, 30 }, 120, 1);
    weekly_add_schedule(&weekly_schedule, "T005", "MIB", "LOL", (dt_time_t) { 12, 45 }, (dt_time_t) { 13, 30 }, 100, 1);
    weekly_add_schedule(&weekly_schedule, "T006", "WIL", "ROC", (dt_time_t) { 13 }, (dt_time_t) { 14, 30 }, 120, 1);
    weekly_add_schedule(&weekly_schedule, "T007", "SAD", "WAT", (dt_time_t) { 14, 25 }, (dt_time_t) { 15, 50 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "GYU", "KAT", (dt_time_t) { 15 }, (dt_time_t) { 16, 30 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T009", "SUS", "PUR", (dt_time_t) { 16 }, (dt_time_t) { 16, 50 }, 100, 1);
    weekly_add_schedule(&weekly_schedule, "T010", "LIY", "MLS", (dt_time_t) { 17 }, (dt_time_t) { 18 }, 130, 1);
    weekly_add_schedule(&weekly_schedule, "T011", "QOP", "YOI", (dt_time_t) { 18 }, (dt_time_t) { 19, 55 }, 120, 1);
    weekly_add_schedule(&weekly_schedule, "T012", "SIE", "URN", (dt_time_t) { 18, 10 }, (dt_time_t) { 18, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T013", "LPO", "COF", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 140, 0);

    //TUESDAY
    weekly_add_schedule(&weekly_schedule, "T001", "MON", "COF", (dt_time_t) { 8, 10 }, (dt_time_t) { 10, 30 }, 120, 2);
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 10 }, (dt_time_t) { 10, 30 }, 140, 2);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 110, 2);
    weekly_add_schedule(&weekly_schedule, "T004", "LIY", "RMH", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 15, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MON", "LIY", (dt_time_t) { 14 }, (dt_time_t) { 17, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "LIY", "MLS", (dt_time_t) { 15 }, (dt_time_t) { 18 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "QOP", "YOI", (dt_time_t) { 14, 10 }, (dt_time_t) { 18, 55 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T009", "SIE", "URN", (dt_time_t) { 16 }, (dt_time_t) { 18, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "LPO", "COF", (dt_time_t) { 17 }, (dt_time_t) { 19, 30 }, 130, 0);
    weekly_add_schedule(&weekly_schedule, "T012", "SAD", "WAT", (dt_time_t) { 18,45 }, (dt_time_t) { 20, 35 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T011", "ROM", "FOC", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T013", "NAP", "ROM", (dt_time_t) { 21 }, (dt_time_t) { 22 }, 140, 0);

    //WEDNESDAY
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 3);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 3);
    weekly_add_schedule(&weekly_schedule, "T001", "COF", "RMH", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 120, 2);
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 140, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 15, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MON", "LIY", (dt_time_t) { 14 }, (dt_time_t) { 17, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "LIY", "MLS", (dt_time_t) { 15 }, (dt_time_t) { 18 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "QOP", "YOI", (dt_time_t) { 14, 10 }, (dt_time_t) { 18, 55 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T009", "SIE", "URN", (dt_time_t) { 16 }, (dt_time_t) { 18, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "LPO", "COF", (dt_time_t) { 17 }, (dt_time_t) { 19, 30 }, 130, 0);
    weekly_add_schedule(&weekly_schedule, "T011", "SAD", "WAT", (dt_time_t) { 18, 45 }, (dt_time_t) { 20, 35 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T012", "ROM", "FOC", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T013", "NAP", "ROM", (dt_time_t) { 21 }, (dt_time_t) { 22 }, 140, 0);

    //THURSDAY
    weekly_add_schedule(&weekly_schedule, "T002", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 140, 4);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 4);
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 110, 2);
    weekly_add_schedule(&weekly_schedule, "T001", "LIY", "RMH", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 15, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MON", "LIY", (dt_time_t) { 14 }, (dt_time_t) { 17, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "LIY", "MLS", (dt_time_t) { 15 }, (dt_time_t) { 18 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "QOP", "YOI", (dt_time_t) { 14, 10 }, (dt_time_t) { 18, 55 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T009", "SIE", "URN", (dt_time_t) { 16 }, (dt_time_t) { 18, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "LPO", "COF", (dt_time_t) { 17 }, (dt_time_t) { 19, 30 }, 130, 0);
    weekly_add_schedule(&weekly_schedule, "T012", "SAD", "WAT", (dt_time_t) { 18, 45 }, (dt_time_t) { 20, 35 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T011", "ROM", "FOC", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T013", "NAP", "ROM", (dt_time_t) { 21 }, (dt_time_t) { 22 }, 140, 0);

    //FRIDAY
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 5);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 5);
    weekly_add_schedule(&weekly_schedule, "T002", "QOP", "YOI", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 140, 2);
    weekly_add_schedule(&weekly_schedule, "T001", "LIY", "RMH", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 15, 30 }, 110, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MON", "LIY", (dt_time_t) { 14 }, (dt_time_t) { 17, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "LIY", "MLS", (dt_time_t) { 15 }, (dt_time_t) { 18 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "QOP", "YOI", (dt_time_t) { 14, 10 }, (dt_time_t) { 18, 55 }, 140, 1);
    weekly_add_schedule(&weekly_schedule, "T009", "SIE", "URN", (dt_time_t) { 16 }, (dt_time_t) { 18, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "LPO", "COF", (dt_time_t) { 17 }, (dt_time_t) { 19, 30 }, 130, 0);
    weekly_add_schedule(&weekly_schedule, "T011", "SAD", "WAT", (dt_time_t) { 18, 45 }, (dt_time_t) { 20, 35 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T012", "ROM", "FOC", (dt_time_t) { 20 }, (dt_time_t) { 22, 30 }, 120, 0);

    //SATURDAY
    weekly_add_schedule(&weekly_schedule, "T003", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 10, 30 }, 110, 6);
    weekly_add_schedule(&weekly_schedule, "T004", "MON", "LIY", (dt_time_t) { 16 }, (dt_time_t) { 10, 30 }, 120, 6);
    weekly_add_schedule(&weekly_schedule, "T001", "COF", "RMH", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 120, 2);
    weekly_add_schedule(&weekly_schedule, "T002", "LIY", "RMH", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 140, 0);
    weekly_add_schedule(&weekly_schedule, "T005", "COF", "RMH", (dt_time_t) { 13, 30 }, (dt_time_t) { 15, 30 }, 110, 0);
    weekly_add_schedule(&weekly_schedule, "T006", "MON", "LIY", (dt_time_t) { 14 }, (dt_time_t) { 17, 30 }, 120, 0);
    weekly_add_schedule(&weekly_schedule, "T007", "LIY", "MLS", (dt_time_t) { 15 }, (dt_time_t) { 18 }, 150, 1);
    weekly_add_schedule(&weekly_schedule, "T008", "EVE", "DCS", (dt_time_t) { 11, 30 }, (dt_time_t) { 12, 30 }, 140, 2);
    weekly_add_schedule(&weekly_schedule, "T009", "CAO", "MHY", (dt_time_t) { 12 }, (dt_time_t) { 13, 30 }, 100, 0);
    weekly_add_schedule(&weekly_schedule, "T010", "LPO", "COF", (dt_time_t) { 15 }, (dt_time_t) { 16, 30 }, 130, 0);

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
