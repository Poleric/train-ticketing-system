#ifndef TRAINTICKETINGSYSTEM_STAFF_H
#define TRAINTICKETINGSYSTEM_STAFF_H

enum Permissions {
    MANAGE_MEMBER = 0b1,
    MANAGE_STAFF = 0b10,
};

typedef struct Staff {
    char* id;
    char* name;
    char* email;
    char* contact_no;
    char* position;
    enum Permissions permissions;
    double salary;
} staff_t;

#endif //TRAINTICKETINGSYSTEM_STAFF_H
