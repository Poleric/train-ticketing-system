#ifndef TRAINTICKETINGSYSTEM_MEMBER_H
#define TRAINTICKETINGSYSTEM_MEMBER_H

enum Membership {
    SILVER = 0x1,
    GOLD = 0x10,
    PLATINUM = 0x100
};

struct Member {
    char *id;
    char *name;
    char gender;
    char *email;
    char *contact_no;
    enum Membership membership;
};

#endif //TRAINTICKETINGSYSTEM_MEMBER_H
