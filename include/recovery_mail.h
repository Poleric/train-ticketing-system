#ifdef EMAIL_RECOVERY_FEATURE

#ifndef TRAINTICKETINGSYSTEM_RECOVERY_MAIL_H
#define TRAINTICKETINGSYSTEM_RECOVERY_MAIL_H

#include <stdlib.h>

struct upload_status {
    char content[1024];
    size_t bytes_read;
};

int generate_random_code(int lower, int upper);
int send_recovery(const char* to_mail, int code);

#endif //TRAINTICKETINGSYSTEM_RECOVERY_MAIL_H

#endif