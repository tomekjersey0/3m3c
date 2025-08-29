#include "graph.h"
#include <assert.h>
#include <stdio.h>

void addEdge(Graph* self, Node* from, Node* to) {
    Node** tmp = realloc(from->neighbours, (from->neighbourCount + 1) * sizeof(Node*));
    assert(tmp != NULL && "realloc failed");
    from->neighbours = tmp;
    from->neighbours[from->neighbourCount] = to;
    from->neighbourCount++;
}

void freeGraph(Graph* g) {
    if (g == NULL) return;

    for (size_t i = 0; i < g->nodeCount; i++) {
        g->freeData(g->nodes[i]);
    }
    
    free(g->nodes);

    free(g);
}

Node* addNode(Graph* self, void* data) {
    Node* n = newNode(data);
    
    if (self->nodeCount >= self->nodeCap) {
        self->nodeCap *= 2;
        self->nodes = realloc(self->nodes, self->nodeCap * sizeof(Node*));
        assert(self->nodes != NULL && "realloc error.");
    }

    self->nodes[self->nodeCount] = n;
    if (self->nodeCount == 0) self->root = n;
    self->nodeCount++;
    return n;
}

Graph* newGraph(CompareFunc cmp, FreeDataFunc freeData)  {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    assert(g != NULL && "malloc error.");

    g->root = NULL;
    g->nodeCap = 4;
    g->nodes = malloc(sizeof(Node*) * g->nodeCap);
    assert(g->nodes != NULL && "malloc error.");
    g->nodeCount = 0;

    g->cmp = cmp;
    g->freeData = freeData;

    g->freeGraph = freeGraph;
    g->addEdge = addEdge;
    g->addNode = addNode;

    return g;
}

