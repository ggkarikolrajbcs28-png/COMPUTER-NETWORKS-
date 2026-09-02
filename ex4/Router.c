#include <stdio.h>
#define MAX 10
#define INF 999

void showMatrix(int n, int graph[MAX][MAX]) {
    int i, j;
    printf("\n========================================\n");
    printf("            NETWORK COST MATRIX\n");
    printf("========================================\n");
    printf("\n\t");
    for (i = 0; i < n; i++) printf("%c\t", 'A' + i);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c\t", 'A' + i);
        for (j = 0; j < n; j++) {
            if (graph[i][j] == INF) printf("INF\t");
            else printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
}

void showFinalDVTable(int n, int dist[MAX][MAX], int next[MAX][MAX]) {
    int i, j;
    printf("\n========================================\n");
    printf("      FINAL DISTANCE VECTOR TABLE\n");
    printf("========================================\n");

    printf("\n--- Final Shortest Path Cost Matrix ---\n");
    printf("\n\t");
    for (i = 0; i < n; i++) printf("%c\t", 'A' + i);
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%c\t", 'A' + i);
        for (j = 0; j < n; j++) {
            if (dist[i][j] == INF) printf("INF\t");
            else printf("%d\t", dist[i][j]);
        }
        printf("\n");
    }
}

/*
 * DISTANCE VECTOR ROUTING
 * -----------------------
 * This function implements the Distance Vector algorithm, which is based on
 * the Bellman-Ford equation. Each router maintains a vector of shortest
 * distances to every other router. The algorithm iteratively relaxes these
 * distances through intermediate nodes until the network converges.
 */
void distanceVector(int n, int cost[MAX][MAX]) {
    int dist[MAX][MAX];
    int next[MAX][MAX];
    int i, j, k;
    int change;
    int round = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dist[i][j] = cost[i][j];
            if (cost[i][j] != INF && i != j) next[i][j] = j;
            else next[i][j] = -1;
        }
    }
    do {
        change = 0;
        round++;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                            change = 1;
                        }
                    }
                }
            }
        }
    } while (change);
    printf("\n========================================\n");
    printf("        DISTANCE VECTOR ROUTING\n");
    printf("========================================\n");
    printf("\nNetwork converged after %d iteration(s).\n", round);
    for (i = 0; i < n; i++) {
        printf("\nRouting Table for Router %c\n", 'A' + i);
        printf("----------------------------------------\n");
        printf("Destination\tNext Hop\tCost\n");
        for (j = 0; j < n; j++) {
            printf("%c\t\t", 'A' + j);
            if (i == j) {
                printf("-\t\t0\n");
            } else if (dist[i][j] == INF) {
                printf("-\t\tINF\n");
            } else {
                printf("%c\t\t%d\n", 'A' + next[i][j], dist[i][j]);
            }
        }
    }
    showFinalDVTable(n, dist, next);
}

int findMin(int dist[MAX], int visited[MAX], int n) {
    int min = INF;
    int pos = -1;
    int i;
    for (i = 0; i < n; i++) {
        if (!visited[i] && dist[i] < min) {
            min = dist[i];
            pos = i;
        }
    }
    return pos;
}

void showPath(int parent[MAX], int node) {
    if (parent[node] == -1) {
        printf("%c", 'A' + node);
        return;
    }
    showPath(parent, parent[node]);
    printf(" -> %c", 'A' + node);
}

/*
 * LINK STATE ROUTING
 * ------------------
 * This function uses Dijkstra's algorithm. Starting from a source router,
 * it repeatedly selects the unvisited router with the smallest known distance,
 * then updates the shortest paths to its neighbors. This is the classic
 * greedy shortest-path algorithm used by Link State protocols.
 */
void linkState(int n, int graph[MAX][MAX]) {
    int dist[MAX];
    int visited[MAX];
    int parent[MAX];
    int source;
    int i, count, current;
    printf("\nEnter the source router (A to %c): ", 'A' + n - 1);
    char srcChar;
    scanf(" %c", &srcChar);
    source = srcChar - 'A';
    if (source < 0 || source >= n) {
        printf("\nInvalid source router selected.\n");
        return;
    }
    for (i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[source] = 0;
    for (count = 0; count < n - 1; count++) {
        current = findMin(dist, visited, n);
        if (current == -1) break;
        visited[current] = 1;
        for (i = 0; i < n; i++) {
            if (!visited[i] && graph[current][i] != INF) {
                int newCost = dist[current] + graph[current][i];
                if (newCost < dist[i]) {
                    dist[i] = newCost;
                    parent[i] = current;
                }
            }
        }
    }
    printf("\n========================================\n");
    printf("          LINK STATE ROUTING\n");
    printf("========================================\n");
    printf("\nSource Router: %c\n", 'A' + source);
    printf("\nDestination\tCost\t\tShortest Path\n");
    printf("----------------------------------------\n");
    for (i = 0; i < n; i++) {
        printf("%c\t\t", 'A' + i);
        if (dist[i] == INF) {
            printf("INF\t\tNo path available\n");
        } else {
            printf("%d\t\t", dist[i]);
            showPath(parent, i);
            printf("\n");
        }
    }
}

void updateSpecificCost(int n, int graph[MAX][MAX]) {
    char r1Char, r2Char;
    int r1, r2, newCost;
    printf("\nEnter the source router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &r1Char);
    printf("Enter the destination router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &r2Char);
    r1 = r1Char - 'A';
    r2 = r2Char - 'A';
    if (r1 < 0 || r1 >= n || r2 < 0 || r2 >= n) {
        printf("\nInvalid router identifiers entered.\n");
        return;
    }
    if (r1 == r2) {
        printf("\nThe cost from a router to itself is always 0.\n");
        return;
    }

    if (graph[r1][r2] == INF)
        printf("\nPrevious cost between %c and %c: INF (no direct connection)\n", r1Char, r2Char);
    else
        printf("\nPrevious cost between %c and %c: %d\n", r1Char, r2Char, graph[r1][r2]);

    printf("Enter the new cost (use %d for no direct connection): ", INF);
    scanf("%d", &newCost);
    graph[r1][r2] = newCost;
    graph[r2][r1] = newCost;
    printf("\nCost matrix updated successfully for the link between %c and %c.\n", r1Char, r2Char);
}

/*
 * SPECIFIC SHORTEST PATH
 * ----------------------
 * This also uses Dijkstra's algorithm (same greedy logic as linkState)
 * but stops after computing the path for one specific destination.
 */
void findSpecificShortestPath(int n, int graph[MAX][MAX]) {
    char srcChar, destChar;
    int source, dest;
    printf("\nEnter the source router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &srcChar);
    printf("Enter the destination router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &destChar);
    source = srcChar - 'A';
    dest = destChar - 'A';
    if (source < 0 || source >= n || dest < 0 || dest >= n) {
        printf("\nInvalid router identifiers entered.\n");
        return;
    }
    int dist[MAX], visited[MAX], parent[MAX];
    int i, count, current;
    for (i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }
    dist[source] = 0;
    for (count = 0; count < n - 1; count++) {
        current = findMin(dist, visited, n);
        if (current == -1) break;
        visited[current] = 1;
        for (i = 0; i < n; i++) {
            if (!visited[i] && graph[current][i] != INF) {
                int newCost = dist[current] + graph[current][i];
                if (newCost < dist[i]) {
                    dist[i] = newCost;
                    parent[i] = current;
                }
            }
        }
    }
    printf("\n========================================\n");
    printf("     SHORTEST PATH: %c -> %c\n", 'A' + source, 'A' + dest);
    printf("========================================\n");
    if (dist[dest] == INF) {
        printf("No path exists between routers %c and %c.\n", 'A' + source, 'A' + dest);
    } else {
        printf("Shortest path cost: %d\n", dist[dest]);
        printf("Path: ");
        showPath(parent, dest);
        printf("\n");
    }
}

/*
 * DROP EDGE & RECALCULATE
 * -----------------------
 * After removing a link, this function reruns the Distance Vector
 * (Bellman-Ford-based) algorithm to show how the network reconverges.
 */
void dropEdgeRecalculateDV(int n, int graph[MAX][MAX]) {
    char r1Char, r2Char;
    int r1, r2;
    int dist[MAX][MAX];
    int next[MAX][MAX];
    int i, j, k;
    int change;

    printf("\nEnter the first router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &r1Char);
    printf("Enter the second router (A to %c): ", 'A' + n - 1);
    scanf(" %c", &r2Char);

    r1 = r1Char - 'A';
    r2 = r2Char - 'A';

    if (r1 < 0 || r1 >= n || r2 < 0 || r2 >= n) {
        printf("\nInvalid router identifiers entered.\n");
        return;
    }

    if (r1 == r2) {
        printf("\nCannot remove a link from a router to itself.\n");
        return;
    }

    graph[r1][r2] = INF;
    graph[r2][r1] = INF;

    printf("\nLink between %c and %c has been removed (cost set to %d).\n", r1Char, r2Char, INF);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
            if (graph[i][j] != INF && i != j) next[i][j] = j;
            else next[i][j] = -1;
        }
    }

    do {
        change = 0;
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (dist[i][k] != INF && dist[k][j] != INF) {
                        if (dist[i][k] + dist[k][j] < dist[i][j]) {
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                            change = 1;
                        }
                    }
                }
            }
        }
    } while (change);

    printf("\n========================================\n");
    printf("   DISTANCE VECTOR AFTER EDGE REMOVAL\n");
    printf("========================================\n");

    showFinalDVTable(n, dist, next);
}

int main() {
    int n = 0;
    int graph[MAX][MAX];
    int i, j;
    int choice;
    int initialized = 0;
    int hasLink;
    int cost;
    do {
        printf("\n========================================\n");
        printf("      ROUTING ALGORITHM SIMULATOR\n");
        printf("========================================\n");
        printf("1. Enter or Update Network Cost Matrix\n");
        printf("2. Display Current Cost Matrix\n");
        printf("3. Run Distance Vector Routing\n");
        printf("4. Run Link State Routing\n");
        printf("5. Run Both Algorithms\n");
        printf("6. Update Cost for a Specific Router Pair\n");
        printf("7. Find Shortest Path Between Specific Routers\n");
        printf("8. Remove Link and Recalculate Distance Vector\n");
        printf("9. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\nEnter the number of routers (maximum %d): ", MAX);
                scanf("%d", &n);
                if (n <= 0 || n > MAX) {
                    printf("\nInvalid number of routers. Please enter a value between 1 and %d.\n", MAX);
                    n = 0;
                    break;
                }

                /* Initialize the entire matrix to INF and diagonal to 0 */
                for (i = 0; i < n; i++) {
                    for (j = 0; j < n; j++) {
                        if (i == j) graph[i][j] = 0;
                        else graph[i][j] = INF;
                    }
                }

                printf("\nEnter the network connections.\n");
                printf("For each pair of routers, indicate whether a direct link exists.\n\n");

                for (i = 0; i < n; i++) {
                    for (j = i + 1; j < n; j++) {
                        printf("Link between %c and %c? (1-Yes, 0-No): ", 'A' + i, 'A' + j);
                        scanf("%d", &hasLink);
                        if (hasLink) {
                            printf("Enter cost: ");
                            scanf("%d", &cost);
                            graph[i][j] = cost;
                            graph[j][i] = cost;
                        }
                    }
                }
                initialized = 1;
                break;
            case 2:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else showMatrix(n, graph);
                break;
            case 3:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else distanceVector(n, graph);
                break;
            case 4:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else linkState(n, graph);
                break;
            case 5:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else {
                    printf("\nExecuting Distance Vector Routing Algorithm...\n");
                    distanceVector(n, graph);
                    printf("\nExecuting Link State Routing Algorithm...\n");
                    linkState(n, graph);
                }
                break;
            case 6:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else updateSpecificCost(n, graph);
                break;
            case 7:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else findSpecificShortestPath(n, graph);
                break;
            case 8:
                if (!initialized) printf("\nPlease initialize the network by entering the cost matrix first (Option 1).\n");
                else dropEdgeRecalculateDV(n, graph);
                break;
            case 9:
                printf("\nProgram terminated successfully.\n");
                break;
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 9.\n");
        }
    } while (choice != 9);
    return 0;
}
