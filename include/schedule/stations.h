#ifndef TRAINTICKETINGSYSTEM_STATIONS_H
#define TRAINTICKETINGSYSTEM_STATIONS_H

#include <stdio.h>

typedef struct StationNode station_node_t;
typedef struct Edge edge_t;
typedef struct StationDetails station_details_t;
typedef struct StationGraph station_graph_t;

typedef struct SerializedEdge s_serialized_edge;

struct Edge {
    station_node_t* next;
    int distance;
};

struct StationDetails {
    char* station_id;
    char* station_name;
};

struct StationNode {
    int id;
    station_details_t details;
    edge_t edge;
};

struct StationGraph {
    int max_size;
    int number_of_nodes;
    station_node_t** nodes;
};

station_node_t* init_node(int id, const char* station_id, const char* station_name);
station_graph_t* init_graph();

station_node_t* copy_node(station_node_t *node);
station_node_t* get_next_node(station_node_t* node);

int node_add_next_connection(station_node_t* start_node, station_node_t* next_node, int distance);
int node_add_connection(station_node_t* start_node, station_node_t* next_node, int distance);

station_node_t* graph_get_node(station_graph_t* graph, const char* station_id);
int resize_graph(station_graph_t *graph);
int graph_add_node(station_graph_t* graph, const char* station_id, const char* station_name);
int graph_add_connection_bidirectional(station_graph_t* graph, const char* station_id, const char* next_station_id, int distance);
int graph_add_connection_directional(station_graph_t* graph, const char* station_id, const char* next_station_id, int distance);

void free_node_list(station_node_t* node);
void free_graph(station_graph_t* graph);

struct SerializedEdge {
    int distance;
    char from_station_id[4], to_station_id[4];
};

int save_stations(station_graph_t* graph, FILE* fp);
int load_stations(station_graph_t* graph, FILE* fp);
int save_connections(station_graph_t* graph, FILE* fp);
int load_connections(station_graph_t* graph, FILE* fp);
int save_graph(station_graph_t* graph, const char* filepath);
int load_graph(station_graph_t* graph, const char* filepath);

#endif //TRAINTICKETINGSYSTEM_STATIONS_H
