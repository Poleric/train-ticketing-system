#include <stdlib.h>
#include <schedule/graph.h>
#include <string.h>


struct StationNode* init_node(int id, const char* station_id, const char* station_name) {
    struct StationNode* node = malloc(sizeof (struct StationNode));
    node->id = id;
    node->details = (struct StationDetails){
            station_id,
            station_name
    };

    return node;
}

struct StationNode* copy_node(struct StationNode *node) {
    return init_node(node->id, node->details.station_id, node->details.station_name);
}

struct StationNode* get_next_node(struct StationNode* node) {
    return node->edge.next;
}

int node_add_next_connection(struct StationNode* start_node, struct StationNode* next_node, int distance) {
    start_node->edge.next = copy_node(next_node);
    start_node->edge.distance = distance;
    return EXIT_SUCCESS;
}

int node_add_connection(struct StationNode* start_node, struct StationNode* next_node, int distance) {
    struct StationNode* node = start_node;
    while (get_next_node(node) != NULL) {
        node = get_next_node(node);
    }
    node_add_next_connection(node, next_node, distance);
    return EXIT_SUCCESS;
}

struct StationGraph* init_graph() {
    struct StationGraph* graph = malloc(sizeof (struct StationGraph));
    graph->max_size = 1;
    graph->number_of_nodes = 0;
    graph->stations = calloc(1, sizeof (struct StationNode*));

    return graph;
}

int resize_graph(struct StationGraph *graph) {
    struct StationNode** tmp = realloc(graph->stations, 2 * graph->number_of_nodes * sizeof (struct StationNode *));
    if (tmp == NULL)
        return EXIT_FAILURE;
    graph->stations = tmp;
    graph->max_size *= 2;
    return EXIT_SUCCESS;
}

struct StationNode* graph_get_node(struct StationGraph* graph, const char* station_id) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        if (strcmp(graph->stations[i]->details.station_id, station_id) == 0)
            return graph->stations[i];
    return NULL;
}

int graph_add_connection(struct StationGraph* graph, const char* station_id, const char* next_station_id, int distance) {
    struct StationNode* node = graph_get_node(graph, station_id);
    struct StationNode* next_node = graph_get_node(graph, next_station_id);

    node_add_connection(node, next_node, distance);
    node_add_connection(next_node, node, distance);

    return EXIT_SUCCESS;
}

int graph_add_node(struct StationGraph* graph, const char* station_id, const char* station_name) {
    static int index = 0;

    if (graph->max_size <= graph->number_of_nodes) {
       if (resize_graph(graph) == EXIT_FAILURE)
           return EXIT_FAILURE;
    }
    graph->stations[graph->number_of_nodes++] = init_node(index++, station_id, station_name);
    return EXIT_SUCCESS;
}

void free_node_list(struct StationNode* node) {
    while (node) {
        struct StationNode* next = get_next_node(node);
        free(node);
        node = next;
    }
}

void free_graph(struct StationGraph* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        free_node_list(graph->stations[i]);
    free(graph->stations);
    free(graph);
}
