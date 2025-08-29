#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "bfs.h"

int intCompare(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia > ib) - (ia < ib); // 1, 0, or -1
}

int stringCompare(const void* a, const void* b) {
    return strcmp((const char*)a, (const char*)b);
}

void resetVisited(Graph* self) {
    if (self == NULL) return;
    if (self->nodes == NULL) return;

    for (size_t i = 0; i < self->nodeCount; i++) {
        self->nodes[i]->visited = 0;
        self->nodes[i]->parent = NULL;
    }
}

PathData shortestPathData(Graph* graph, void* target) {
    assert(graph != NULL && "graph cannot be NULL.");
    Node* root = graph->root;

    assert(root != NULL && "root cannot be NULL.");
    resetVisited(graph);

    Queue* toVisit = newQueue();
    toVisit->clear(toVisit);

    toVisit->enqueue(toVisit, root);

    while (toVisit->length > 0) {
        Node* current = toVisit->dequeue(toVisit);

        current->visited = 1;
        if (0 == graph->cmp(current->data, target)) {
            PathData pd = {current, 0, NULL};

            int len = 0;
            for (Node* n = current; n != NULL; n = n->parent) {
               len++; 
            }
            pd.hops = len - 1;

            pd.path = (Node**)malloc(len * sizeof(Node*));
            assert(pd.path != NULL && "malloc error.");

            int i = len - 1;
            for (Node* n = current; n != NULL; n = n->parent) {
                pd.path[i--] = n; 
            }
            
            toVisit->freeSelf(toVisit);
            return pd;
        }

        for (size_t i = 0; i < current->neighbourCount; i++) {
            Node* neigh = current->neighbours[i];
            if (!neigh->visited && neigh->parent == NULL) {
                neigh->parent = current;
                toVisit->enqueue(toVisit, neigh);  
            }
        }
    }

    toVisit->freeSelf(toVisit);
    return (PathData){NULL, -1, NULL};
}

Node* findNode(Graph* graph, void* target) {
    return shortestPathData(graph, target).node;    
}

int shortestDistance(Graph* graph, void* target) {
    return shortestPathData(graph, target).hops;
}

int isInGraph(Graph* graph, void* target) {
    if (findNode(graph, target) != NULL)
        return 1;
    else
        return 0;
}

void freePath(ShortestPath* self) {
    if (self->path == NULL) return;
    free(self->path);
}

ShortestPath shortestPath(Graph* graph, void* target) {
    PathData pd = shortestPathData(graph, target);
    return (ShortestPath){pd.path, pd.hops, freePath};
}

