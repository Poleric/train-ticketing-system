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

struct StationGraph {
    int max_size;
    int number_of_nodes;
    struct StationNode** stations;
};

struct StationNode* init_node(int id, const char* station_id, const char* station_name);
struct StationGraph* init_graph();

struct StationNode* copy_node(struct StationNode *node);
struct StationNode* get_next_node(struct StationNode* node);

int node_add_next_connection(struct StationNode* start_node, struct StationNode* next_node, int distance);
int node_add_connection(struct StationNode* start_node, struct StationNode* next_node, int distance);

struct StationNode* graph_get_node(struct StationGraph* graph, const char* station_id);
int resize_graph(struct StationGraph *graph);
int graph_add_node(struct StationGraph* graph, const char* station_id, const char* station_name);
int graph_add_connection(struct StationGraph* graph, const char* station_id, const char* next_station_id, int distance);

void free_node_list(struct StationNode* node);
void free_graph(struct StationGraph* graph);

#endif //TRAINTICKETINGSYSTEM_GRAPH_H
