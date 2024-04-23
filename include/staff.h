#ifndef TRAINTICKETINGSYSTEM_STAFF_H
#define TRAINTICKETINGSYSTEM_STAFF_H

enum Permissions {
    MANAGE_MEMBER = 0b1,
    MANAGE_STAFF = 0b10,
};

typedef struct Staff {
    char* username;
    char* hashed_password;
    char* email;
    char* contact_no;
    char* position;
    enum Permissions permissions;
    double salary;
} staff_t;


void free_staff(staff_t* staff);

#endif //TRAINTICKETINGSYSTEM_STAFF_H
