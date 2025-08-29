#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

typedef struct Queue {
   void** array;
    size_t start;
    size_t end;
    size_t length;
    size_t cap;

    void(*enqueue)(struct Queue* self, void* data);
    void*(*dequeue)(struct Queue* self);
    void*(*peek)(struct Queue* self);
    int(*isEmpty)(struct Queue* self);
    int(*isFull)(struct Queue* self);
    void(*clear)(struct Queue* self);

    void(*grow)(struct Queue* self, size_t req_cap);
    void(*freeSelf)(struct Queue* self);
} Queue;

Queue* newQueue();

#endif
