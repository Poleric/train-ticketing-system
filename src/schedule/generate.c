#include <stdlib.h>
#include <schedule.h>

struct LocNode* create_node(const char* loc_id) {
    struct LocNode* node = malloc(sizeof (struct LocNode));
    if (node == NULL)
        return NULL;

    node->loc_id = loc_id;
    node->next = NULL;
    return node;
}

struct Graph* create_graph(int vertices) {
    struct Graph* graph = malloc(sizeof (struct Graph));
    if (graph == NULL)
        return NULL;

    graph->num_vertices = vertices;
    graph->adj_lists = calloc(vertices, sizeof (struct LocNode*));
    return graph;
}

int add_edge(struct Graph* graph, const char* ref_loc, const char* new_loc) {
    struct LocNode* node = create_node(new_loc);
    node->next = 

}

int free_node(struct LocNode* node) {
    free(node);
    return EXIT_SUCCESS;
}

int free_graph(struct Graph* graph) {
    for (int i = 0; i < graph->num_vertices; i++)
        free_node(graph->adj_lists[i]);
    free(graph->adj_lists);
    free(graph);
    return EXIT_SUCCESS;
}