#include "queue.h" 
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// pretty useless yeah, but it is the idiomatic C++ way, even if this is C
size_t size(Queue* self) {
    return self->length;
}

void clear(Queue* self) {
    self->start = 0;
    self->end = 0;
    self->length = 0;
}

void freeQ(Queue* self) {
    self->clear(self);
    free(self->array);
    free(self);
}

int isFull(Queue* self) {
    return self->length == self->cap;
}

int isEmpty(Queue* self) {
   return self->length == 0; 
}

void* peek(Queue* self) {
    if (!self->isEmpty(self)) {
        return self->array[self->start];
    } else {
        assert(0 && "queue is empty.");
        // below to quiet warnings
        return NULL;
    }
}

void grow(Queue* self, size_t req_cap) {
    if (req_cap <= self->cap) 
        return; 

    void** temp = malloc(sizeof(void*) * req_cap);
    assert(temp != NULL && "malloc error.");

    // Copy elements in correct circular order
    for (size_t i = 0; i < self->length; i++) {
        temp[i] = self->array[(self->start + i) % self->cap];
    }

    free(self->array);
    self->array = temp;
    self->cap = req_cap;

    // Normalize indices
    self->start = 0;
    self->end = self->length;
}


void enqueue(Queue* self, void* data) {
    if (self->isFull(self)) {
        size_t newCap = (self->cap == 0 ? 1 : self->cap * 2);
       grow(self, newCap);         
    }
    assert(!self->isFull(self) && "queue already full."); 
    self->array[self->end] = data;

    self->end++;
    if (self->end >= self->cap) {
        self->end = 0;
    }

    self->length++;
}

void* dequeue(Queue* self) {
    assert(!self->isEmpty(self) && "queue already empty.");
    void* toDelete = self->array[self->start];

    self->start++;
    if (self->start >= self->cap) {
        self->start = 0;
    }

    self->length--;
    return toDelete;
}

Queue* newQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    assert(q != NULL && "malloc error.");

    q->start = 0;
    q->end = 0;
    q->length = 0;
    q->cap = 4;

    q->array = malloc(sizeof(void*) * q->cap);
    assert(q->array != NULL && "malloc error.");
    
    q->enqueue = enqueue;
    q->dequeue = dequeue;
    q->peek = peek;
    q->isEmpty = isEmpty;
    q->isFull = isFull;
    q->clear = clear;

    q->grow = grow;
    q->freeSelf = freeQ;

    return q;
}


