#include "schedule.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MIN_DAY_SCHEDULES 4

schedule_t* create_schedule(char* train_id, char* from_station_id, char* to_station_id, dt_time_t time, dt_time_t eta, int n_seats) {
    schedule_t* schedule = malloc(sizeof (schedule_t));

    strncpy(schedule->train_id, train_id, 5);
    strncpy(schedule->from_station_id, from_station_id, 4);
    strncpy(schedule->to_station_id, to_station_id, 4);
    schedule->departure_time = time;
    schedule->eta = eta;
    schedule->n_seats = n_seats;

    return schedule;
}

int init_schedules(schedule_vector_t* schedules) {
    schedules->max_size = MIN_DAY_SCHEDULES;
    schedules->n_elements = 0;
    schedules->array = calloc(sizeof(schedule_t*), schedules->max_size);

    if (schedules->array == NULL)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int init_weekly_schedule(weekly_schedule_t* weekly_schedule) {
    for (int i = 0; i < 7; i++)
        init_schedules(weekly_schedule->days + i);
    return EXIT_SUCCESS;
}

int add_schedule(schedule_vector_t* schedules, schedule_t* schedule) {
    if (schedules->n_elements + 1 > schedules->max_size) {
        schedules->max_size *= 2;
        schedule_t **tmp = realloc(schedules->array, sizeof (schedule_t*) * schedules->max_size);

        if (tmp == NULL)
            return EXIT_FAILURE;

        schedules->array = tmp;
    }

    schedules->array[schedules->n_elements++] = schedule;
    return EXIT_SUCCESS;
}

int delete_schedule_by_info(schedule_vector_t* schedules, schedule_t* schedule) {
    for (int i = 0; i < schedules->n_elements; i++) {
        if (is_schedule_same(schedules->array[i], schedule)) {
            free(schedules->array[i]);
            schedules->array[i] = NULL;

            // shift into empty slot
            for (int j = i + 1; j < schedules->n_elements; j++)
                schedules->array[j - 1] = schedules->array[j];
            schedules->array[--schedules->n_elements] = NULL;

            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

int delete_schedule_by_train_time(schedule_vector_t* schedules, char* train_id, dt_time_t time) {
    for (int i = 0; i < schedules->n_elements; i++) {
        if (strcmp(schedules->array[i]->train_id, train_id) == 0 &&
            is_time_same(schedules->array[i]->departure_time, time)) {
            free(schedules->array[i]);
            schedules->array[i] = NULL;

            // shift into empty slot
            for (int j = i + 1; j < schedules->n_elements; j++)
                schedules->array[j - 1] = schedules->array[j];
            schedules->array[schedules->n_elements--] = NULL;

            return EXIT_SUCCESS;
        }
    }
    return EXIT_FAILURE;
}

bool is_time_same(dt_time_t time_1, dt_time_t time_2) {
    return
            time_1.tm_hour == time_2.tm_hour &&
            time_1.tm_min == time_2.tm_min &&
            time_1.tm_sec == time_2.tm_sec;
}

bool is_schedule_same(schedule_t* schedule_1, schedule_t* schedule_2) {
    if (schedule_1 == schedule_2)
        return true;

    if (strcmp(schedule_1->train_id, schedule_2->train_id) != 0)
        return false;

    if (strcmp(schedule_1->from_station_id, schedule_2->from_station_id) != 0)
        return false;

    if (strcmp(schedule_1->to_station_id, schedule_2->to_station_id) != 0)
        return false;

    if (!is_time_same(schedule_1->departure_time, schedule_2->departure_time))
        return false;

    if (!is_time_same(schedule_1->eta, schedule_2->eta))
        return false;

    return true;
}

int save_daily_schedule(schedule_vector_t* schedules, FILE* fp) {
    for (int i = 0; i < schedules->n_elements; i++)
        fprintf(fp, "%4s, %3s, %3s, %02d:%02d:%02d, %02d:%02d:%02d, %d\n",
                schedules->array[i]->train_id,
                schedules->array[i]->from_station_id,
                schedules->array[i]->to_station_id,
                schedules->array[i]->departure_time.tm_hour, schedules->array[i]->departure_time.tm_min, schedules->array[i]->departure_time.tm_sec,
                schedules->array[i]->eta.tm_hour, schedules->array[i]->eta.tm_min, schedules->array[i]->eta.tm_sec,
                schedules->array[i]->n_seats
        );
    return EXIT_SUCCESS;
}

int save_weekly_schedule(weekly_schedule_t* weekly_schedule, const char* filepath) {
    FILE* fp = fopen(filepath, "w");

    if (fp == NULL)
        return EXIT_FAILURE;

    for (int i = 0; i < 7; i++) {
        save_daily_schedule(weekly_schedule->days + i, fp);
        fputc('\n', fp);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int load_weekly_schedule(weekly_schedule_t* weekly_schedule, const char* filepath) {
    FILE* fp = fopen(filepath, "r");

    if (fp == NULL)
        return EXIT_FAILURE;

    char line_buff[50];
    char train_id[5], from_station_id[4], to_station_id[4];
    dt_time_t time, eta;
    int n_seats;
    for (int i = 0; i < 7; i++)
        while (1) {
            fgets(line_buff, 50, fp);
            if (strcmp(line_buff, "\n") == 0)
                break;

            if (sscanf(line_buff, "%4s, %3s, %3s, %d:%d:%d, %d:%d:%d, %d",
                   train_id,
                   from_station_id,
                   to_station_id,
                   &time.tm_hour, &time.tm_min, &time.tm_sec,
                   &eta.tm_hour, &eta.tm_min, &eta.tm_sec,
                   &n_seats) != 10)
                break;

            weekly_add_schedule(weekly_schedule, train_id, from_station_id, to_station_id, time, eta, n_seats, i);
        }

    fclose(fp);
    return EXIT_SUCCESS;
}

void free_schedules(schedule_vector_t* schedules) {
    for (int i = 0; i < schedules->n_elements; i++)
        free(schedules->array[i]);
    free(schedules->array);
}

void free_weekly_schedules(weekly_schedule_t* weekly_schedule) {
    for (int i = 0; i < 7; i++)
        free_schedules(weekly_schedule->days + i);
}

int weekly_add_schedule(weekly_schedule_t* weekly_schedule, char* train_id, char* from_station_id, char* to_station_id, dt_time_t time, dt_time_t eta, int n_seats, int tm_wday) {
    schedule_t* schedule = create_schedule(train_id, from_station_id, to_station_id, time, eta, n_seats);

    add_schedule(weekly_schedule->days + tm_wday, schedule);

    return EXIT_SUCCESS;
}


