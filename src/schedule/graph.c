#include <stdlib.h>
#include <schedule/graph.h>
#include <string.h>


struct StationNode* init_node(int id, const char* station_id, const char* station_name) {
    struct StationNode* node = malloc(sizeof (struct StationNode));
    node->id = id;
    node->details = (struct StationDetails){
            strdup(station_id),
            strdup(station_name)
    };

    return node;
}

struct StationGraph* init_graph() {
    struct StationGraph* graph = malloc(sizeof (struct StationGraph));
    graph->max_size = 1;
    graph->number_of_nodes = 0;
    graph->stations = calloc(1, sizeof (struct StationNode*));

    return graph;
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

struct StationNode* graph_get_node(struct StationGraph* graph, const char* station_id) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        if (strcmp(graph->stations[i]->details.station_id, station_id) == 0)
            return graph->stations[i];
    return NULL;
}

int resize_graph(struct StationGraph *graph) {
    struct StationNode** tmp = realloc(graph->stations, 2 * graph->number_of_nodes * sizeof (struct StationNode *));
    if (tmp == NULL)
        return EXIT_FAILURE;
    graph->stations = tmp;
    graph->max_size *= 2;
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

int graph_add_connection_bidirectional(struct StationGraph* graph, const char* station_id, const char* next_station_id, int distance) {
    struct StationNode* node = graph_get_node(graph, station_id);
    struct StationNode* next_node = graph_get_node(graph, next_station_id);

    node_add_connection(node, next_node, distance);
    node_add_connection(next_node, node, distance);

    return EXIT_SUCCESS;
}

int graph_add_connection_directional(struct StationGraph* graph, const char* station_id, const char* next_station_id, int distance) {
    struct StationNode* node = graph_get_node(graph, station_id);
    struct StationNode* next_node = graph_get_node(graph, next_station_id);

    node_add_connection(node, next_node, distance);

    return EXIT_SUCCESS;
}

void free_node_list(struct StationNode* node) {
    while (node) {
        struct StationNode* next = get_next_node(node);
        free(node->details.station_id);
        free(node->details.station_name);
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

int save_locations(struct StationGraph* graph, FILE* fp) {
    fwrite(&graph->number_of_nodes, sizeof(int), 1, fp);
    for (int i = 0; i < graph->number_of_nodes; i++) {
        fwrite(graph->stations[i]->details.station_id, sizeof(char), 4, fp);
        int station_name_len = (int) strlen(graph->stations[i]->details.station_name) + 1;
        fwrite(&station_name_len, sizeof(int), 1, fp);
        fwrite(graph->stations[i]->details.station_name, sizeof (char), station_name_len, fp);
    }
    return EXIT_SUCCESS;
}

int load_locations(struct StationGraph* graph, FILE* fp) {
    int n_of_locations;
    fread(&n_of_locations, sizeof(int), 1, fp);

    char station_id[4], *station_name;
    int station_name_len;
    for (int i = 0; i < n_of_locations; i++) {
        fread(station_id, sizeof (char), 4, fp);
        fread(&station_name_len, sizeof (int), 1, fp);
        station_name = malloc(station_name_len);
        fread(station_name, sizeof (char), station_name_len, fp);
        graph_add_node(graph, station_id, station_name);
        free(station_name);
    }
    return EXIT_SUCCESS;
}

int save_connections(struct StationGraph* graph, FILE* fp) {
    // fp open in binary
    int n_of_connections = 0;
    long start_pos = ftell(fp);
    fseek(fp, sizeof(int), SEEK_CUR);  // leave space for size in the beginning
    for (int i = 0; i < graph->number_of_nodes; i++) {
        struct StationNode* node = graph->stations[i];
        struct StationNode* next_node = get_next_node(node);
        while (next_node) {
            struct s_SerializedEdge edge;
            edge.distance = node->edge.distance;
            strncpy(edge.from_station_id, node->details.station_id, 4);
            strncpy(edge.to_station_id, next_node->details.station_id, 4);

            fwrite(&edge, sizeof(struct s_SerializedEdge), 1, fp);
            n_of_connections++;

            next_node = get_next_node(next_node);
        }
    }
    fseek(fp, start_pos, SEEK_SET);
    fwrite(&n_of_connections, sizeof(int), 1, fp);
    return EXIT_SUCCESS;
}

int load_connections(struct StationGraph* graph, FILE* fp) {
    int n_of_connections;
    fread(&n_of_connections, sizeof(int), 1, fp);
    for (int i = 0; i < n_of_connections; i++) {
        struct s_SerializedEdge edge;
        fread(&edge, sizeof(struct s_SerializedEdge), 1, fp);
        graph_add_connection_directional(graph, edge.from_station_id, edge.to_station_id, edge.distance);
    }
    return EXIT_SUCCESS;
}

int save_graph(struct StationGraph* graph, const char* filepath) {
    FILE* fp = fopen(filepath, "wb");

    if (fp == NULL)
        return EXIT_FAILURE;

    save_locations(graph, fp);
    save_connections(graph, fp);
    return EXIT_SUCCESS;
}

int load_graph(struct StationGraph* graph, const char* filepath) {
    FILE* fp = fopen(filepath, "rb");

    if (fp == NULL)
        return EXIT_FAILURE;

    load_locations(graph, fp);
    load_connections(graph, fp);
    return EXIT_SUCCESS;
}
