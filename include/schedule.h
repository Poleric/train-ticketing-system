#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_H

#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <utils.h>

typedef struct Schedule schedule_t;
typedef struct ScheduleVector schedule_vector_t;
typedef struct WeeklySchedule weekly_schedule_t;

struct Schedule {
    char train_id[5];
    char from_station_id[4], to_station_id[4];
    dt_time_t departure_time;
    dt_time_t eta;
    int n_seats;
};

struct ScheduleVector {
    int max_size;
    int n_elements;
    schedule_t **array;
};

struct WeeklySchedule {
    schedule_vector_t days[7];  // 0 - Sunday
};

schedule_t* create_schedule(char* train_id, char* from_station_id, char* to_station_id, dt_time_t time, dt_time_t eta, int n_seats);
int init_schedules(schedule_vector_t* schedules);
int init_weekly_schedule(weekly_schedule_t* weekly_schedule);

int add_schedule(schedule_vector_t* schedules, schedule_t* schedule);
int delete_schedule_by_info(schedule_vector_t* schedules, schedule_t* schedule);
int delete_schedule_by_train_time(schedule_vector_t* schedules, char* train_id, dt_time_t time);

bool is_schedule_same(schedule_t* schedule_1, schedule_t* schedule_2);

int save_daily_schedule(schedule_vector_t* schedules, FILE* fp);
int save_weekly_schedule(weekly_schedule_t* weekly_schedule, const char* filepath);

int load_weekly_schedule(weekly_schedule_t* weekly_schedule, const char* filepath);

void free_schedules(schedule_vector_t* schedules);
void free_weekly_schedules(weekly_schedule_t* weekly_schedule);

int weekly_add_schedule(
        weekly_schedule_t* weekly_schedule,
        char* train_id,
        char* from_station_id,
        char* to_station_id,
        dt_time_t time,
        dt_time_t eta,
        int n_seats,
        int tm_wday);

#endif //TRAINTICKETINGSYSTEM_SCHEDULE_H
