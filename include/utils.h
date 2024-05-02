#ifndef TRAINTICKETINGSYSTEM_UTILS_H
#define TRAINTICKETINGSYSTEM_UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define MAX(a,b) __max(a,b)
#define MIN(a,b) __min(a,b)
#else
#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
#endif

#define SHA256_HASH_LENGTH 64

typedef struct datetime_date dt_date_t;
typedef struct datetime_time dt_time_t;
typedef enum tm_wday tm_wday_t;

enum tm_wday {
    SUNDAY = 0,
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6
};

struct datetime_date {
    int tm_year;
    int tm_mon;
    int tm_mday;
};

struct datetime_time {
    int tm_hour;
    int tm_min;
    int tm_sec;
};

int digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
int decode_digest(const unsigned char* digest, size_t digest_size, char* buff, size_t buff_size);
int hash_message(const char* message, char buff[SHA256_HASH_LENGTH]);
int compare_hash(const char* not_hashed, const char* hashed);

int input(const char* prompt, char* buffer, size_t buffer_size);
void flush(FILE* stream);

void delete_char(char* buffer, int pos);
void add_char(char* buffer, char ch, int pos);

int sum_d(const int* nums, int number_of_num);

int get_number_of_digits_d(int n);

const char* tm_mon_to_text(int tm_mon);
const char* tm_wday_to_text(tm_wday_t tm_wday);
char* to_time(dt_time_t time, char* buffer, int n);
bool is_time_same(dt_time_t time_1, dt_time_t time_2);
time_t time_t_from_datetime(int year, int month, int day, int hour, int minute, int second);
time_t time_t_from_dt(dt_date_t date, dt_time_t time);
void split_tm(struct tm tm, dt_date_t* date, dt_time_t* time, int *tm_wday);
struct tm tm_now();

#endif //TRAINTICKETINGSYSTEM_UTILS_H
