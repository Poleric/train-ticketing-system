#include <openssl/evp.h>
#include <utils.h>
#include <string.h>
#include <math.h>

int digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len) {
    // https://wiki.openssl.org/index.php/EVP_Message_Digests
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == NULL)
        return EXIT_FAILURE;

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        return EXIT_FAILURE;

    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
        return EXIT_FAILURE;

    if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)
        return EXIT_FAILURE;

    if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
        return EXIT_FAILURE;

    EVP_MD_CTX_free(mdctx);
    return EXIT_SUCCESS;
}

int decode_digest(const unsigned char* digest, size_t digest_size, char* buff, size_t buff_size) {
    if (buff_size < digest_size * 2) {
        fprintf(stderr, "Buffer size `%zu` is not large enough to fit the digest of length `%zu`\n", buff_size, digest_size);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < digest_size; i++) {
        snprintf(buff, 3, "%02x", digest[i]);
        buff += 2;
    }
    return EXIT_SUCCESS;
}

int hash_message(const char* message, char buff[SHA256_HASH_LENGTH]) {
    unsigned char* digest;
    unsigned digest_size;

    digest_message((const unsigned char*)message, strlen(message), &digest, &digest_size);

    decode_digest(digest, (size_t) digest_size, buff, SHA256_HASH_LENGTH);
    free(digest);
    return EXIT_SUCCESS;
}

int compare_hash(const char* not_hashed, const char* hashed) {
    char to_hash[SHA256_HASH_LENGTH + 1];
    to_hash[SHA256_HASH_LENGTH] = 0;
    hash_message(not_hashed, to_hash);

    return strncmp(to_hash, hashed, 64);
}

int input(const char* prompt, char* buffer, size_t buffer_size) {
    printf("%s", prompt);

    if (fgets(buffer, (int) buffer_size, stdin) == NULL)
        return EXIT_FAILURE;

    if (buffer[strlen(buffer) - 1] != '\n')  // input longer than buffer can hold
        flush(stdin);

    // trim \n
    buffer[strcspn(buffer, "\n")] = 0;

    if (buffer[0] == 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void flush(FILE* stream) {
    int c;
    while ((c = getc(stream)) != '\n' && c != EOF);
}

void delete_char(char* buffer, int pos) {
    /* unsafe
     * helper function to remove character in middle and shift all char
     */
    buffer += pos;
    while (*(buffer++)) {
        *(buffer - 1) = *buffer;
    }
    *buffer = 0;
}

void add_char(char* buffer, char ch, int pos) {
    /* unsafe
     * helper function to add character in middle and shift all char
     */
    size_t i = strlen(buffer);
    buffer += i;
    *(buffer + 1) = 0;
    while (i-- != pos) {
        *buffer = *(buffer - 1);
        buffer--;
    }
    *buffer = ch;
}

const char* tm_mon_to_text(int tm_mon) {
    switch (tm_mon) {
        case 0: return "January";
        case 1: return "February";
        case 2: return "March";
        case 3: return "April";
        case 4: return "May";
        case 5: return "June";
        case 6: return "July";
        case 7: return "August";
        case 8: return "September";
        case 9: return "October";
        case 10: return "November";
        case 11: return "December";
        default: return NULL;
    }
}

const char* tm_wday_to_text(tm_wday_t tm_wday) {
    switch (tm_wday) {
        case 0: return "Sunday";
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        default: return NULL;
    }
}

char* to_time(dt_time_t time, char* buffer, int n) {
    snprintf(buffer, n, "%2d:%02d", time.tm_hour, time.tm_min);
    return buffer;
}

int sum_d(const int* nums, int number_of_num) {
    int sum = 0;
    for (int i = 0; i < number_of_num; i++)
        sum += nums[i];
    return sum;
}

int get_number_of_digits_d(int n) {
    n = abs(n);

    if (n == 0)
        return 1;

    return (int)ceil(log10(n + 1));
}

bool is_time_same(dt_time_t time_1, dt_time_t time_2) {
    return
            time_1.tm_hour == time_2.tm_hour &&
            time_1.tm_min == time_2.tm_min &&
            time_1.tm_sec == time_2.tm_sec;
}

time_t time_t_from_datetime(int year, int month, int day, int hour, int minute, int second) {
    struct tm tm;

    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    return mktime(&tm);
}

time_t time_t_from_dt(dt_date_t date, dt_time_t time) {
    return time_t_from_datetime(
            date.tm_year,
            date.tm_mon,
            date.tm_mday,
            time.tm_hour,
            time.tm_min,
            time.tm_sec
            );
}

void split_tm(struct tm tm, dt_date_t* date, dt_time_t* time, int *tm_wday) {
    if (date != NULL) {
        date->tm_year = tm.tm_year + 1900;
        date->tm_mon = tm.tm_mon + 1;
        date->tm_mday = tm.tm_mday;
    }
    if (time != NULL) {
        time->tm_hour = tm.tm_hour;
        time->tm_min = tm.tm_min;
        time->tm_sec = tm.tm_sec;
    }
    if (tm_wday != NULL)
        *tm_wday = tm.tm_wday;
}

struct tm tm_now() {
    time_t t = time(NULL);
    return *localtime(&t);
}

struct tm tm_add_days(struct tm tm, const int days) {
    const time_t SECONDS_PER_DAY = 24 * 60 * 60;

    // Seconds since start of epoch --> 01/01/1970 at 00:00hrs
    time_t date_seconds = mktime(&tm) + (days * SECONDS_PER_DAY);

    return *localtime(&date_seconds);
}

dt_date_t date_add_days(dt_date_t date, const int days) {
    const time_t SECONDS_PER_DAY = 24 * 60 * 60;

    // Seconds since start of epoch --> 01/01/1970 at 00:00hrs
    time_t date_seconds = time_t_from_dt(date, (dt_time_t){0, 0, 0}) + (days * SECONDS_PER_DAY);

    split_tm(*localtime(&date_seconds), &date, NULL, NULL);
    return date;
}
