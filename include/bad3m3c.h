#include "bfs.h"
#include <stdio.h>
#include <assert.h>

typedef struct Side {
    int m_count;
    int c_count;
} Side;

typedef enum BoatSide {
    BOAT_SIDE_LEFT = -1,
    BOAT_SIDE_RIGHT = 1 
} BoatSide;

typedef struct State {
    Side left; 
    Side right; 
    BoatSide boatSide;
} State;

static inline State* newState(Side l, Side r, int b) {
    State* s = malloc(sizeof(State));
    assert(s != NULL && "malloc error.");

    s->left = l;
    s->right = r;
    s->boatSide = b;

    return s;
}

static inline void freeState(void* data) {
    State* s = (State*)data;
    free(s);
}

static inline int stateCompare(const void* a, const void* b) {
    const State* sa = (const State*)a;
    const State* sb = (const State*)b;

    return (sa->boatSide != sb->boatSide) || 
       (sa->left.m_count != sb->left.m_count) || 
       (sa->left.c_count != sb->left.c_count) || 
       (sa->right.m_count != sb->right.m_count) || 
       (sa->right.c_count != sb->right.c_count); 
}

static inline int validState(Graph* g, State* state) {
    if (state->left.c_count > state->left.m_count) return 0;
    if (state->right.c_count > state->right.m_count) return 0;
    if (    (state->left.c_count < 0) ||
            (state->left.m_count < 0) ||
            (state->right.c_count < 0) ||
            (state->right.m_count < 0)) return 0;
    if (isInGraph(g, state)) { return 0; }
    return 1;
}

static inline void resetState(State og, State* current) {
    current->boatSide = og.boatSide;
    current->left.m_count = og.left.m_count;
    current->left.c_count = og.left.c_count;
    current->right.m_count = og.right.m_count;
    current->right.c_count = og.right.c_count;
}

static inline void printState(State current) {
    printf("leftm: %d, leftc: %d, rightm: %d, rightc: %d, boatSide: %d\n", current.left.m_count, current.left.c_count, current.right.m_count, current.right.c_count, current.boatSide);
}

typedef enum Move {
    _1ctor,
    _1mtor,
    _2mtor,
    _2ctor,
    _1m1ctor,

    _1mtol,
    _1ctol,
    _2mtol,
    _2ctol,
    _1m1ctol,
    MOVE_COUNT
} Move;

static inline BoatSide moveDirection(Move m) {
    switch (m) {
        case _1ctor:
        case _1mtor:
        case _2mtor:
        case _2ctor:
        case _1m1ctor:
            return BOAT_SIDE_RIGHT;
        case _1mtol:
        case _1ctol:
        case _2mtol:
        case _2ctol:
        case _1m1ctol:
            return BOAT_SIDE_LEFT;
    }
}

static inline void makeMove(Graph* g, State current, State* target, int depth);

static inline void doMove(Graph* g, State* nextState, State* target, State og, Move move, Node* parent, int depth) {
    switch (move) {
        // to the right
        case _1mtor:
            // 1 maiden 
            nextState->left.m_count--;
            nextState->right.m_count++;
            break;
        case _2mtor:
            // 2 maidens
            nextState->left.m_count -= 2;
            nextState->right.m_count += 2;
            break;
        case _1ctor:
            // 1 cannibal
            nextState->left.c_count--;
            nextState->right.c_count++;
            break;
        case _2ctor:
            // 2 cannibals
            nextState->left.c_count -= 2;
            nextState->right.c_count += 2;
            break;
        case _1m1ctor:
            // 1 cannibal 1 maiden
            nextState->left.m_count--;
            nextState->right.m_count++;
            nextState->left.c_count--;
            nextState->right.c_count++;
            break;

        // to the left
        case _1mtol:
            // 1 maiden 
            nextState->left.m_count++;
            nextState->right.m_count--;
            break;
        case _2mtol:
            // 2 maidens
            nextState->left.m_count += 2;
            nextState->right.m_count -= 2;
            break;
        case _1ctol:
            // 1 cannibal
            nextState->left.c_count++;
            nextState->right.c_count--;
            break;
        case _2ctol:
            // 2 cannibals
            nextState->left.c_count += 2;
            nextState->right.c_count -= 2;
            break;
        case _1m1ctol:
            // 1 cannibal 1 maiden
            nextState->left.m_count++;
            nextState->right.m_count--;
            nextState->left.c_count++;
            nextState->right.c_count--;
            break;
    }

    BoatSide newBoatSide = moveDirection(move);

    printf("state depth[%d]:\n", depth);
    printState(*nextState);

    if (validState(g, nextState) && newBoatSide != nextState->boatSide) {
        nextState->boatSide = newBoatSide;
        printf("the following state is valid:\n");
        printState(*nextState);

        if (parent != NULL) {
            Node* childNode = g->addNode(g, nextState);
            childNode->visited = 1;
            g->addEdge(g, parent, childNode);
        }
        makeMove(g, *nextState, target, depth+1);
    }
    else {
        printf("nope!\n");
    }
    resetState(og, nextState);
}

static inline void makeMove(Graph* g, State current, State* target, int depth) {
    if (0 == stateCompare(&current, target)) { printf("found!\n\n"); return; } 

    Node* parent;
    if (g->root == NULL) {
        State* save = newState(current.left, current.right, current.boatSide);
        Node* savedNode = g->addNode(g, save);
        savedNode->visited = 1;
        parent = savedNode;
    } else {
        parent = findNode(g, &current);
    }

    State* nextState = newState(current.left, current.right, current.boatSide);
    State og = *nextState;

    for (size_t m = 0; m < MOVE_COUNT; m++) {
        Move move = (Move)m;

        doMove(g, nextState, target, og, move, parent, depth);
    }
}

static inline void bad3m3v() {
    Graph* graph = newGraph(stateCompare, freeState);

    State* start = newState((Side){.m_count = 3, .c_count = 3}, (Side){.m_count = 0, .c_count = 0}, BOAT_SIDE_LEFT);
    State* trg = newState((Side){.m_count = 0, .c_count = 0}, (Side){.m_count = 3, .c_count = 3}, BOAT_SIDE_RIGHT);

    makeMove(graph, *start, trg, 0);

    int is = isInGraph(graph, trg);
    printf("found: %d\n", is);

    ShortestPath path = shortestPath(graph, trg);
    if (path.hops == 0) printf("no path.");
    printf("hops: %d\n", path.hops);
    for (int i = 0; i < path.hops; i++) {
        printf("step [%d]: ", i+1);
        printState(*(State*)(path.path[i]->data)); printState(*(State*)(path.path[i+1]->data));
    }
    path.freePath(&path);

    graph->freeGraph(graph);
}
