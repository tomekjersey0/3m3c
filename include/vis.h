#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bfs.h"
#include "state.h"

// label helper: "L:(m,c)  R:(m,c)  Boat:L/R"
static void state_label(const State* s, char* buf, size_t n) {
    snprintf(buf, n, "L:(%d,%d)  R:(%d,%d)  Boat:%c",
             s->left.m_count, s->left.c_count,
             s->right.m_count, s->right.c_count,
             (s->boatSide == BOAT_SIDE_LEFT ? 'L' : 'R'));
}

// true if edge u->v is part of the ShortestPath (consecutive pair)
static int edge_on_path(Node* u, Node* v, const ShortestPath* p) {
    if (!p || !p->path || p->hops <= 0) return 0;
    for (int i = 0; i < p->hops; i++) {
        if (p->path[i] == u && p->path[i+1] == v) return 1;
    }
    return 0;
}

// Write whole graph in DOT format. If `sol` is non-NULL, highlight the path.
void write_dot(Graph* g, const char* filename, const ShortestPath* sol) {
    FILE* f = fopen(filename, "w");
    if (!f) { perror("fopen"); return; }

    fprintf(f, "digraph G {\n");
    fprintf(f, "  rankdir=TB;\n");
    fprintf(f, "  node [shape=oval,fontname=\"Courier\"];\n");

    // Emit all nodes with labels
    for (size_t i = 0; i < g->nodeCount; i++) {
        State* s = (State*)g->nodes[i]->data;
        char label[128];
        state_label(s, label, sizeof(label));
        // mark start/target visually if you want (optional)
        int isRoot = (g->nodes[i] == g->root);
        fprintf(f, "  n%zu [label=\"%s\"%s];\n",
                i, label, isRoot ? ",style=filled,fillcolor=lightgray" : "");
    }

    // Emit edges, highlight those on the shortest path
    for (size_t i = 0; i < g->nodeCount; i++) {
        Node* u = g->nodes[i];
        for (size_t j = 0; j < u->neighbourCount; j++) {
            Node* v = u->neighbours[j];

            // find indices (we know nodes are in g->nodes array)
            size_t ui = i, vi = 0;
            for (size_t k = 0; k < g->nodeCount; k++) {
                if (g->nodes[k] == v) { vi = k; break; }
            }

            int onPath = edge_on_path(u, v, sol);
            if (onPath) {
                fprintf(f, "  n%zu -> n%zu [color=red,penwidth=2.5];\n", ui, vi);
            } else {
                fprintf(f, "  n%zu -> n%zu;\n", ui, vi);
            }
        }
    }

    fprintf(f, "}\n");
    fclose(f);
}

