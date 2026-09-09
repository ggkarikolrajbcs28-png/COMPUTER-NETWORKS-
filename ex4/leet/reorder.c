#include <stdlib.h>

typedef struct {
    int to;
    int cost;
    int next;
} Edge;

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    // There are (n-1) undirected connections, so we store 2*(n-1) directed edges
    Edge* edges = (Edge*)malloc(2 * connectionsSize * sizeof(Edge));
    int* head   = (int*)malloc(n * sizeof(int));
    int* visited = (int*)calloc(n, sizeof(int));
    int* queue  = (int*)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        head[i] = -1;
    }

    int edgeCnt = 0;

    // Add a directed edge u -> v with the given cost
    #define ADD_EDGE(u, v, c) \
        do { \
            edges[edgeCnt].to   = (v); \
            edges[edgeCnt].cost = (c); \
            edges[edgeCnt].next = head[(u)]; \
            head[(u)] = edgeCnt++; \
        } while (0)

    for (int i = 0; i < connectionsSize; i++) {
        int a = connections[i][0];
        int b = connections[i][1];

        // Original road a -> b: traversing this way goes AWAY from city 0, so it needs 1 reversal
        ADD_EDGE(a, b, 1);
        // Reverse direction b -> a: this already points TOWARD city 0, so cost is 0
        ADD_EDGE(b, a, 0);
    }

    #undef ADD_EDGE

    // BFS starting from the capital (city 0)
    int front = 0, rear = 0;
    queue[rear++] = 0;
    visited[0] = 1;
    int changes = 0;

    while (front < rear) {
        int u = queue[front++];

        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (!visited[v]) {
                visited[v] = 1;
                changes += edges[e].cost;   // accumulate cost if we crossed a wrongly oriented road
                queue[rear++] = v;
            }
        }
    }

    free(edges);
    free(head);
    free(visited);
    free(queue);

    return changes;
}
