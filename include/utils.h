#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef TRAINTICKETINGSYSTEM_UTILS_H
#define TRAINTICKETINGSYSTEM_UTILS_H

#define SHA256_HASH_LENGTH 64

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
const char* tm_mon_to_text(int tm_mon);
const char* tm_wday_to_text(tm_wday_t tm_wday);
char* to_time(dt_time_t time, char* buffer, int n);

#endif //TRAINTICKETINGSYSTEM_UTILS_H
