#include <stdlib.h>
#include <schedule/stations.h>
#include <string.h>


station_node_t* init_node(char* station_id, char* station_name) {
    station_node_t* node = calloc(1, sizeof (station_node_t));
    node->station_id = station_id;
    node->station_name = station_name;

    return node;
}

station_graph_t* init_graph() {
    station_graph_t* graph = malloc(sizeof (station_graph_t));
    graph->max_size = 1;
    graph->number_of_nodes = 0;
    graph->nodes = calloc(1, sizeof (station_node_t*));

    return graph;
}

station_node_t* copy_node(station_node_t *node) {
    return init_node(node->station_id, node->station_name);
}

station_node_t* get_next_node(station_node_t* node) {
    return node->edge.next;
}

int node_add_next_connection(station_node_t* start_node, station_node_t* next_node, int distance) {
    start_node->edge.next = copy_node(next_node);
    start_node->edge.distance = distance;
    return EXIT_SUCCESS;
}

int node_add_connection(station_node_t* start_node, station_node_t* next_node, int distance) {
    station_node_t* node = start_node;
    while (get_next_node(node) != NULL) {
        node = get_next_node(node);
    }
    node_add_next_connection(node, next_node, distance);
    return EXIT_SUCCESS;
}

station_node_t* graph_get_node(station_graph_t* graph, const char* station_id) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        if (strcmp(graph->nodes[i]->station_id, station_id) == 0)
            return graph->nodes[i];
    return NULL;
}

int resize_graph(station_graph_t *graph) {
    station_node_t** tmp = realloc(graph->nodes, 2 * graph->number_of_nodes * sizeof (station_node_t *));
    if (tmp == NULL)
        return EXIT_FAILURE;
    graph->nodes = tmp;
    graph->max_size *= 2;
    return EXIT_SUCCESS;
}

int graph_add_node(station_graph_t* graph, const char* station_id, const char* station_name) {
    static int index = 0;

    if (graph->max_size <= graph->number_of_nodes) {
        if (resize_graph(graph) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    graph->nodes[graph->number_of_nodes++] = init_node(strdup(station_id), strdup(station_name));
    return EXIT_SUCCESS;
}

int graph_add_connection_bidirectional(station_graph_t* graph, char* station_id, char* next_station_id, int distance) {
    station_node_t* node = graph_get_node(graph, station_id);
    station_node_t* next_node = graph_get_node(graph, next_station_id);

    node_add_connection(node, next_node, distance);
    node_add_connection(next_node, node, distance);

    return EXIT_SUCCESS;
}

int graph_add_connection_directional(station_graph_t* graph, char* station_id, char* next_station_id, int distance) {
    station_node_t* node = graph_get_node(graph, station_id);
    station_node_t* next_node = graph_get_node(graph, next_station_id);

    node_add_connection(node, next_node, distance);

    return EXIT_SUCCESS;
}

void free_node_list(station_node_t* node) {
    printf("Freeing %s\n", node->station_id);
    free(node->station_id);
    free(node->station_name);
    while (node) {
        station_node_t* next = get_next_node(node);
        printf("%p\n", node);
        free(node);
        node = next;
    }
}

void free_graph(station_graph_t* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        free_node_list(graph->nodes[i]);
    free(graph->nodes);
    free(graph);
}

int save_stations(station_graph_t* graph, FILE* fp) {
    fwrite(&graph->number_of_nodes, sizeof(int), 1, fp);
    for (int i = 0; i < graph->number_of_nodes; i++) {
        fwrite(graph->nodes[i]->station_id, sizeof(char), 4, fp);
        int station_name_len = (int) strlen(graph->nodes[i]->station_name) + 1;
        fwrite(&station_name_len, sizeof(int), 1, fp);
        fwrite(graph->nodes[i]->station_name, sizeof (char), station_name_len, fp);
    }
    return EXIT_SUCCESS;
}

int load_stations(station_graph_t* graph, FILE* fp) {
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

int save_connections(station_graph_t* graph, FILE* fp) {
    // fp open in binary
    int n_of_connections = 0;
    long start_pos = ftell(fp);
    fseek(fp, sizeof(int), SEEK_CUR);  // leave space for size in the beginning
    for (int i = 0; i < graph->number_of_nodes; i++) {
        station_node_t* node = graph->nodes[i];
        station_node_t* next_node = get_next_node(node);
        while (next_node) {
            s_serialized_edge edge;
            edge.distance = node->edge.distance;
            strncpy(edge.from_station_id, node->station_id, 4);
            strncpy(edge.to_station_id, next_node->station_id, 4);

            fwrite(&edge, sizeof(s_serialized_edge), 1, fp);
            n_of_connections++;

            next_node = get_next_node(next_node);
        }
    }
    fseek(fp, start_pos, SEEK_SET);
    fwrite(&n_of_connections, sizeof(int), 1, fp);
    return EXIT_SUCCESS;
}

int load_connections(station_graph_t* graph, FILE* fp) {
    int n_of_connections;
    fread(&n_of_connections, sizeof(int), 1, fp);
    for (int i = 0; i < n_of_connections; i++) {
        s_serialized_edge edge;
        fread(&edge, sizeof(s_serialized_edge), 1, fp);
        graph_add_connection_directional(graph, edge.from_station_id, edge.to_station_id, edge.distance);
    }
    return EXIT_SUCCESS;
}

int save_graph(station_graph_t* graph, const char* filepath) {
    FILE* fp = fopen(filepath, "wb");

    if (fp == NULL)
        return EXIT_FAILURE;

    save_stations(graph, fp);
    save_connections(graph, fp);
    return EXIT_SUCCESS;
}

int load_graph(station_graph_t* graph, const char* filepath) {
    FILE* fp = fopen(filepath, "rb");

    if (fp == NULL)
        return EXIT_FAILURE;

    load_stations(graph, fp);
    load_connections(graph, fp);
    return EXIT_SUCCESS;
}
