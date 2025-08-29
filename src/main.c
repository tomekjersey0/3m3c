#include <stdio.h>
#include <assert.h>
#include "state.h"
#include "vis.h"
#include "bfs.h"
#include "graph.h"


int isValid(State s) {
    // no negatives
    if (s.left.m_count < 0 || s.left.c_count < 0 ||
        s.right.m_count < 0 || s.right.c_count < 0) return 0;

    // missionaries eaten on left
    if (s.left.m_count > 0 && s.left.c_count > s.left.m_count) return 0;

    // missionaries eaten on right
    if (s.right.m_count > 0 && s.right.c_count > s.right.m_count) return 0;

    return 1;
}

State applyMove(State s, Move m) {
    if (s.boatSide == BOAT_SIDE_LEFT) {
        s.left.m_count  -= m.m_count;
        s.left.c_count  -= m.c_count;
        s.right.m_count += m.m_count;
        s.right.c_count += m.c_count;
    } else { // boat on RIGHT
        s.left.m_count  += m.m_count;
        s.left.c_count  += m.c_count;
        s.right.m_count -= m.m_count;
        s.right.c_count -= m.c_count;
    }

    // flip boat
    s.boatSide = (s.boatSide == BOAT_SIDE_LEFT) ? BOAT_SIDE_RIGHT : BOAT_SIDE_LEFT;

    return s;
}

// all possible "move shapes"
Move moves[] = {
    {1, 0}, // 1 missionary
    {2, 0}, // 2 missionaries
    {0, 1}, // 1 cannibal
    {0, 2}, // 2 cannibals
    {1, 1}  // 1 missionary + 1 cannibal
};
size_t moveCount = sizeof(moves)/sizeof(moves[0]);

int stateCompare(const void* a, const void* b) {
    const State* sa = (const State*)a;
    const State* sb = (const State*)b;
    return (sa->boatSide != sb->boatSide ||
            sa->left.c_count != sb->left.c_count ||
            sa->left.m_count != sb->left.m_count ||
            sa->right.c_count != sb->right.c_count ||
            sa->right.m_count != sb->right.m_count);
}

void stateFree(void* data) {
    State* d = (State*)data;
    free(d);
}

State* newState(int mleft, int cleft, BoatSide boat) {
    State* s = malloc(sizeof(State));
    assert(s != NULL && "malloc error.");

    s->left.m_count = mleft;
    s->left.c_count = cleft;
    s->right.m_count = 3-mleft;
    s->right.c_count = 3-cleft;
    s->boatSide = boat;

    return s;
}

int main() {
    // setup graph
    Graph* graph = newGraph(stateCompare, stateFree);

    // enumerate all states
    Node* states[4][4][2] = { NULL };
    

    for (int M = 0; M <= 3; M++) {
        for (int C = 0; C <= 3; C++) {
            for (int boat = 0; boat <= 1; boat++) {
                State* s = newState(M, C, (BoatSide)boat);
                if (isValid(*s)) {
                    Node* n = graph->addNode(graph, s);
                    states[M][C][boat] = n;
                } else {
                    free(s); // discard invalid state
                }
            }
        }
    }

    for (int M = 0; M <= 3; M++) {
        for (int C = 0; C <= 3; C++) {
            for (int boat = 0; boat <= 1; boat++) {
                Node* node = states[M][C][boat];
                if (!node) continue;

                State curr = *(State*)node->data;

                
                for (size_t i = 0; i < moveCount; i++) {
                    State next = applyMove(curr, moves[i]);
                    if (isValid(next)) {
                        Node* nextNode = states[next.left.m_count][next.left.c_count][next.boatSide];
                        graph->addEdge(graph, node, nextNode);
                    }
                }
            }
        }
    }

    graph->root = states[3][3][0];

    State* target = newState(0, 0, BOAT_SIDE_RIGHT);

    int found = isInGraph(graph, target);
    printf("found: %d\n", found);

    ShortestPath p = shortestPath(graph, target);
    stateFree(target);
    for (size_t i = 0; i < p.hops; i++) {
        State* s1 = p.path[i]->data;
        State* s2 = p.path[i+1]->data;
        const char* ltor = "left to right";
        const char* rtol = "right to left";
        const char* movement;
        movement = (s1->boatSide == BOAT_SIDE_LEFT) ? ltor : rtol; 
        
/*       This is just to print the state of each side of the river at each point. commented out for cluttering!
 *       printf("step [%ld]: (%d,%dL:%d,%dR) -> (%d,%dL:%d,%dR)\n", i+1, s1->left.m_count, s1->left.c_count, s1->right.m_count, s1->right.c_count,
                                                                        s2->left.m_count, s2->left.c_count, s2->right.m_count, s2->right.c_count);
*/
        int movedM, movedC;
        if (s1->boatSide == BOAT_SIDE_LEFT) {
            movedM = s1->left.m_count - s2->left.m_count;
            movedC = s1->left.c_count - s2->left.c_count;
        } else {
            movedM = s1->right.m_count - s2->right.m_count;
            movedC = s1->right.c_count - s2->right.c_count;
        }
        if (movedM == 0)
            printf("step [%ld]: boat moved %d canniba%s from %s\n", i+1, movedC, (movedC > 1) ? "ls" : "l", movement);
        else if (movedC == 0)
            printf("step [%ld]: boat moved %d missionar%s from %s\n", i+1, movedM, (movedM > 1) ? "ies" : "y", movement);
        else
            printf("step [%ld]: boat moved %d canniba%s and %d missionar%s from %s\n", i+1, movedC, (movedC > 1) ? "ls" : "l", movedM, (movedM > 1) ? "ies" : "y", movement);
    }
    printf("and you're done!");

    write_dot(graph, "mc_graph.dot", &p);

    p.freePath(&p);
    return 0;
}
