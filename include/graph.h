#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "queue.h"

typedef int (*CompareFunc)(const void* a, const void* b);
typedef void (*FreeDataFunc)(void* data);
static inline void noopFree(void* data) { (void)data; }

typedef struct Graph {
    Node* root;
    Node** nodes;
    size_t nodeCount;
    size_t nodeCap;

    // for bfs
    CompareFunc cmp;
    FreeDataFunc freeData;

    void(*addEdge)(struct Graph* self, Node* from, Node* to);
    Node*(*addNode)(struct Graph* self, void* data);
    void(*freeGraph)(struct Graph* self);
} Graph;

Graph* newGraph(CompareFunc cmp, FreeDataFunc freeData);

#endif
