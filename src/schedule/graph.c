#include <stdlib.h>
#include <schedule/graph.h>
#include <stdio.h>
#include <string.h>

struct StationNode* init_node(const char* location_name) {
    struct StationNode* node = malloc(sizeof (struct StationNode));
    node->name = location_name;
    node->edge = (struct Edge){};

    return node;
}

struct Stations* init_graph() {
    struct Stations* graph = malloc(sizeof (struct Stations));
    graph->max_size = 1;
    graph->number_of_nodes = 0;
    graph->locations = calloc(1, sizeof (struct StationNode*));

    return graph;
}

struct StationNode* get_next_node(struct StationNode* node) {
    return node->edge.next;
}

int add_next_connection(struct StationNode* node, const char* location_name, int distance) {
    node->edge.next = init_node(location_name);
    node->edge.distance = distance;
    return EXIT_SUCCESS;
}

int add_connection(struct StationNode* start_node, const char* location_name, int distance) {
    struct StationNode* node = start_node;
    while (get_next_node(node) != NULL) {
        node = get_next_node(node);
    }
    add_next_connection(node, location_name, distance);
    return EXIT_SUCCESS;
}

int add_connection_to_connection(struct StationNode* node_1, struct StationNode* node_2, int distance) {
    add_connection(node_1, node_2->name, distance);
    add_connection(node_2, node_1->name, distance);
    return EXIT_SUCCESS;
}

int add_root_node(struct Stations* graph, const char* location_name) {
    if (graph->max_size <= graph->number_of_nodes) {
        struct StationNode** tmp = realloc(graph->locations, 2 * graph->number_of_nodes * sizeof (struct StationNode *));
        if (tmp == NULL)
            return EXIT_FAILURE;
        graph->locations = tmp;
        graph->max_size *= 2;
    }
    graph->locations[graph->number_of_nodes++] = init_node(location_name);
    return EXIT_SUCCESS;
}


struct StationNode* get_node(struct Stations* graph, const char* location_name) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        if (strcmp(graph->locations[i]->name, location_name) == 0)
            return graph->locations[i];
    return NULL;
}

void print_node_link(struct StationNode* node) {
    printf("%s", node->name);
    while (node) {
        if (node->edge.distance != 0)
            printf(" -(%d)-> %s\n", node->edge.distance, node->edge.next->name);
        node = get_next_node(node);
    }
}

void print_graph(struct Stations* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++) {
        print_node_link(graph->locations[i]);
        printf("\n");
    }
}

void free_node_list(struct StationNode* node) {
    while (node) {
        struct StationNode* next = get_next_node(node);
        free(node);
        node = next;
    }
}

void free_graph(struct Stations* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        free_node_list(graph->locations[i]);
    free(graph->locations);
    free(graph);
}

int main() {
    struct Stations* graph = init_graph();

    add_root_node(graph, "KL");
    add_root_node(graph, "TH");
    add_root_node(graph, "SG");
    add_root_node(graph, "JB");

    add_connection_to_connection(get_node(graph, "KL"), get_node(graph, "TH"), 120);
    add_connection_to_connection(get_node(graph, "KL"), get_node(graph, "SG"), 80);
    add_connection_to_connection(get_node(graph, "JB"), get_node(graph, "SG"), 20);
    add_connection_to_connection(get_node(graph, "SG"), get_node(graph, "TH"), 200);

    print_graph(graph);

    free_graph(graph);

    return EXIT_SUCCESS;
}


