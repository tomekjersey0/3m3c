#ifndef BFS_H
#define BFS_H
#include "graph.h"

typedef struct ShortestPath {
    Node** path;
    int hops;

    void(*freePath)(struct ShortestPath* self);
} ShortestPath;

typedef struct PathData {
    Node* node;
    int hops;
    Node** path;
} PathData;

int intCompare(const void* a, const void* b);
int stringCompare(const void* a, const void* b);
void resetVisited(Graph* self);
PathData shortestPathData(Graph* graph, void* target);
Node* findNode(Graph* graph, void* target);
int shortestDistance(Graph* graph, void* target);
int isInGraph(Graph* graph, void* target);
void freePath(ShortestPath* self);
ShortestPath shortestPath(Graph* graph, void* target);

#endif
