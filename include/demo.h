#ifndef DEMO_H
#define DEMO_H
#include "graph.h"
#include "bfs.h"
#include <stdio.h>

static inline void demoProgram() {
    int vals[] = {1, 2, 3, 4, 5};  

    Graph* graph = newGraph(intCompare, noopFree);

    Node* a = graph->addNode(graph, (void*)&vals[0]);
    Node* b = graph->addNode(graph, (void*)&vals[1]);
    Node* c = graph->addNode(graph, (void*)&vals[2]);
    Node* d = graph->addNode(graph, (void*)&vals[3]);
    Node* e = graph->addNode(graph, (void*)&vals[4]);

    graph->addEdge(graph, a, b);
    graph->addEdge(graph, a, c);
    graph->addEdge(graph, b, d);
    graph->addEdge(graph, c, d);
    graph->addEdge(graph, d, e);

    int target = 5;

    ShortestPath path = shortestPath(graph, (void*)&target);

    printf("hops to %d: %d\n", target, path.hops);
    for (size_t i = 0; i < path.hops; i++) {
        printf("path to %d, step [%ld]: %d -> %d\n", target, i+1, *(int*)path.path[i]->data, *(int*)path.path[i+1]->data);
    }
    path.freePath(&path);

    graph->freeGraph(graph);
}

#endif
