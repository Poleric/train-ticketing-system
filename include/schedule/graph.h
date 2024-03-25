#ifndef TRAINTICKETINGSYSTEM_GRAPH_H
#define TRAINTICKETINGSYSTEM_GRAPH_H

struct Edge {
    struct StationNode* next;
    int distance;
};

struct StationDetails {
    const char* station_id;
    const char* station_name;
};

struct StationNode {
    int id;
    struct StationDetails details;
    struct Edge edge;
};

struct Stations {
    int max_size;
    int number_of_nodes;
    struct StationNode** locations;
};

#endif //TRAINTICKETINGSYSTEM_GRAPH_H
