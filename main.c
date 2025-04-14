#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define MAX_NEIGHBORS 100
#define MAX_PATH_LEN 100
#define MAX_LINE_LEN 256

typedef struct Node {
    char* name;
    char* neighbors[MAX_NEIGHBORS];
    int neighbor_count;
    int active;  // for avoiding redundant traversals
} Node;

Node graph[MAX_NODES];
int graph_size = 0;

// Paths
char** all_paths[MAX_NODES * MAX_NODES];
int path_lengths[MAX_NODES * MAX_NODES];
int total_paths = 0;

// Utility: find or add a node
Node* get_node(const char* name) {
    for (int i = 0; i < graph_size; i++) {
        if (strcmp(graph[i].name, name) == 0)
            return &graph[i];
    }
    graph[graph_size].name = strdup(name);
    graph[graph_size].neighbor_count = 0;
    graph[graph_size].active = 1;
    return &graph[graph_size++];
}

// Add edge
void add_edge(const char* from, const char* to) {
    Node* src = get_node(from);
    src->neighbors[src->neighbor_count++] = strdup(to);
}

// Check if node is visited in path
int is_visited(char** path, int length, const char* name) {
    for (int i = 0; i < length; i++) {
        if (strcmp(path[i], name) == 0) return 1;
    }
    return 0;
}

// Recursive path finding
void find_paths(const char* current, char** path, int length) {
    Node* node = get_node(current);
    node->active = 0;

    path[length++] = strdup(current);

    if (is_visited(path, length - 1, current)) {
        // cyclic
        all_paths[total_paths] = malloc(length * sizeof(char*));
        for (int i = 0; i < length; i++) {
            all_paths[total_paths][i] = strdup(path[i]);
        }
        path_lengths[total_paths++] = length;
        return;
    }

    if (node->neighbor_count == 0) {
        all_paths[total_paths] = malloc(length * sizeof(char*));
        for (int i = 0; i < length; i++) {
            all_paths[total_paths][i] = strdup(path[i]);
        }
        path_lengths[total_paths++] = length;
        return;
    }

    for (int i = 0; i < node->neighbor_count; i++) {
        find_paths(node->neighbors[i], path, length);
    }
}

// Loop type detection
int contains_loop(char** path, int length) {
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            if (strcmp(path[i], path[j]) == 0) return 1;
        }
    }
    return 0;
}

int is_circular_loop(char** path, int length) {
    return length > 1 && strcmp(path[0], path[length - 1]) == 0;
}

// Main
int main() {
    FILE* file = fopen("dependencies.txt", "r");
    if (!file) {
        perror("Failed to open dependencies.txt");
        return 1;
    }

    char from[64], arrow[4], to[64];
    while (fscanf(file, "%s %s %s", from, arrow, to) == 3) {
        add_edge(from, to);
    }
    fclose(file);

    for (int i = 0; i < graph_size; i++) {
        if (graph[i].active) {
            char* path[MAX_PATH_LEN];
            find_paths(graph[i].name, path, 0);
        }
    }

    printf("Paths found: %d\n", total_paths);
    printf("No circular dependency:\n");

    for (int i = 0; i < total_paths; i++) {
        int len = path_lengths[i];
        int loop = contains_loop(all_paths[i], len);
        int circular = is_circular_loop(all_paths[i], len);

        if (!loop) {
            for (int j = 0; j < len; j++) {
                printf("%s%s", all_paths[i][j], (j == len - 1) ? "\n" : " -> ");
            }
        }
    }

    printf("Circular dependency detected:\n");
    for (int i = 0; i < total_paths; i++) {
        int len = path_lengths[i];
        int loop = contains_loop(all_paths[i], len);
        int circular = is_circular_loop(all_paths[i], len);

        if (loop) {
            for (int j = 0; j < len; j++) {
                printf("%s%s", all_paths[i][j], (j == len - 1) ? "\n" : " -> ");
            }
        }
    }

    // Clean up memory
    for (int i = 0; i < graph_size; i++) {
        free(graph[i].name);
        for (int j = 0; j < graph[i].neighbor_count; j++) {
            free(graph[i].neighbors[j]);
        }
    }

    for (int i = 0; i < total_paths; i++) {
        for (int j = 0; j < path_lengths[i]; j++) {
            free(all_paths[i][j]);
        }
        free(all_paths[i]);
    }

    return 0;
}
