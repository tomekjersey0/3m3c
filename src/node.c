#include "node.h"
#include <assert.h>

void freeSelf(Node* self) {
    if (self == NULL) return;
    if (self->neighbours) {
        free(self->neighbours);
    }
    free(self);
}

Node* newNode(void* data) {
    Node* n = (Node*)malloc(sizeof(Node));
    assert(n != NULL && "malloc error.");
    n->data = data;
    n->neighbours = NULL;
    n->neighbourCount = 0;

    n->visited = 0;
    n->parent = NULL;

    n->freeSelf = freeSelf;

    return n;
}
