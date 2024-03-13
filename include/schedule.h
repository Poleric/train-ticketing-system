#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_H

struct Connection {
    struct LocationNode* next;
    int distance;
};

struct LocationNode {
    const char* name;
    struct Connection con;
};

struct LocationGraph {
    int max_size;
    int number_of_nodes;
    struct LocationNode** locations;
};


#endif //TRAINTICKETINGSYSTEM_SCHEDULE_H
