#ifndef NODE_H
#define NODE_H

#include <stdlib.h>

typedef struct Node {
    void* data;
    struct Node** neighbours;
    size_t neighbourCount;

    // for bfs
    int visited;
    struct Node* parent;

    void(*freeSelf)(struct Node* self);
} Node;

Node* newNode(void* data);

#endif
