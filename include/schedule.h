#ifndef TRAINTICKETINGSYSTEM_SCHEDULE_H
#define TRAINTICKETINGSYSTEM_SCHEDULE_H


struct LocNode {
    const char* loc_id;
    struct NextNode* next;
};

struct NextNode {
    struct LocNode* next;
    int distance;
};

struct Graph {
    int num_vertices;
    struct LocNode** adj_lists;
};



#endif //TRAINTICKETINGSYSTEM_SCHEDULE_H
