#include <stdlib.h>
#include <schedule.h>
#include <stdio.h>
#include <string.h>

struct LocationNode* init_node(const char* location_name) {
    struct LocationNode* node = malloc(sizeof (struct LocationNode));
    node->name = location_name;
    node->con = (struct Connection){};

    return node;
}

struct LocationGraph* init_graph() {
    struct LocationGraph* graph = malloc(sizeof (struct LocationGraph));
    graph->max_size = 1;
    graph->number_of_nodes = 0;
    graph->locations = calloc(1, sizeof (struct LocationNode*));

    return graph;
}

struct LocationNode* get_next_node(struct LocationNode* node) {
    return node->con.next;
}

int add_next_connection(struct LocationNode* node, const char* location_name, int distance) {
    node->con.next = init_node(location_name);
    node->con.distance = distance;
    return EXIT_SUCCESS;
}

int add_connection(struct LocationNode* start_node, const char* location_name, int distance) {
    struct LocationNode* node = start_node;
    while (get_next_node(node) != NULL) {
        node = get_next_node(node);
    }
    add_next_connection(node, location_name, distance);
    return EXIT_SUCCESS;
}

int add_connection_to_connection(struct LocationNode* node_1, struct LocationNode* node_2, int distance) {
    add_connection(node_1, node_2->name, distance);
    add_connection(node_2, node_1->name, distance);
    return EXIT_SUCCESS;
}

int add_root_node(struct LocationGraph* graph, const char* location_name) {
    if (graph->max_size <= graph->number_of_nodes) {
        struct LocationNode** tmp = realloc(graph->locations, 2 * graph->number_of_nodes * sizeof (struct LocationNode *));
        if (tmp == NULL)
            return EXIT_FAILURE;
        graph->locations = tmp;
        graph->max_size *= 2;
    }
    graph->locations[graph->number_of_nodes++] = init_node(location_name);
    return EXIT_SUCCESS;
}


struct LocationNode* get_node(struct LocationGraph* graph, const char* location_name) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        if (strcmp(graph->locations[i]->name, location_name) == 0)
            return graph->locations[i];
    return NULL;
}

void print_node_link(struct LocationNode* node) {
    printf("%s", node->name);
    while (node) {
        if (node->con.distance != 0)
            printf(" -(%d)-> %s\n", node->con.distance, node->con.next->name);
        node = get_next_node(node);
    }
}

void print_graph(struct LocationGraph* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++) {
        print_node_link(graph->locations[i]);
        printf("\n");
    }
}

void free_node_list(struct LocationNode* node) {
    while (node) {
        struct LocationNode* next = get_next_node(node);
        free(node);
        node = next;
    }
}

void free_graph(struct LocationGraph* graph) {
    for (int i = 0; i < graph->number_of_nodes; i++)
        free_node_list(graph->locations[i]);
    free(graph->locations);
    free(graph);
}

int main() {
    struct LocationGraph* graph = init_graph();

    add_root_node(graph, "KL");
    add_root_node(graph, "TH");
    add_root_node(graph, "SG");
    add_root_node(graph, "JB");

    add_connection_to_connection(get_node(graph, "KL"), get_node(graph, "TH"), 120);
    add_connection_to_connection(get_node(graph, "KL"), get_node(graph, "SG"), 80);
    add_connection_to_connection(get_node(graph, "KL"), get_node(graph, "JB"), 60);
    add_connection_to_connection(get_node(graph, "JB"), get_node(graph, "SG"), 20);
    add_connection_to_connection(get_node(graph, "JB"), get_node(graph, "TH"), 140);
    add_connection_to_connection(get_node(graph, "SG"), get_node(graph, "TH"), 200);

    print_graph(graph);

    free_graph(graph);

    return EXIT_SUCCESS;
}


