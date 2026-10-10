#include <stdlib.h>
#include "dfs.h"
#include "pathing.h"

static void *DfsCreate(Maze *m) {
    DfsState *s = TrackCalloc(1, sizeof(DfsState));
    if (s == NULL) return NULL;

    int cells = m->config.cols * m->config.rows;
    int start = m->starts[0];

    s->seen = TrackCalloc(cells, sizeof(int));
    s->stack = TrackAlloc(cells * sizeof(int));
    s->parent = TrackAlloc(cells * sizeof(int));


    if (s->seen == NULL || s->stack == NULL || s->parent == NULL) {
        TrackFree(s->seen);
        TrackFree(s->stack);
        TrackFree(s->parent);
        TrackFree(s);
        return NULL;
    }

    for (int i = 0; i < cells; i++) {
        s->parent[i] = -1;
    }
    s->seen[start] = 1;
    s->stack[0] = start;
    s->top = 1;
    s->current = start;
    s->nodesExpanded = 1;
    s->maxFrontier = 1;

    return s;

}

static void DfsStep(void *state, Maze *m) {
    DfsState *s = (DfsState *)state;
    if (s->done) return;
    if (s->top == 0) {
        s->done = true;
        return;
    }
    int cur = s->stack[s->top - 1];
    int nb[4];
    int n = GetNeighbors(m, cur, nb);

    int next = -1;
    for (int i = 0; i < n; i++) {
        if (s->seen[nb[i]] == 0) {
            next = nb[i];
            break;                 // take the first one only
        }
    }

    if (next != -1) {
        s->seen[next] = 1;
        s->parent[next] = cur;
        s->stack[s->top] = next;
        s->top++;
        s->nodesExpanded++;
        s->current = next;

        if (s->top > s->maxFrontier) s->maxFrontier = s->top;

        if (IsEnd(m, next)) {
            s->found = true;
            s->foundCell = next;
            s->pathLength = s->top - 1;
            s->done = true;
            s->pathCost = PathCost(m, s->parent, next);
        }


    } else {
        s->top--;                                   // pop: step back one cell
        if (s->top > 0) s->current = s->stack[s->top - 1];   // highlight where we are now
        else s->done = true;                        // stack empty: everything explored
    }

}

static bool DfsDone(void *state) { return ((DfsState *)state)->done; }

static void DfsResult(void *state, Result *r) {
    DfsState *s = (DfsState *)state;
    r->found = s->found ? 1 : 0;
    r->pathLength = s->found ? s->pathLength : 0;
    r->nodesExpanded = s->nodesExpanded;
    r->maxFrontier = s->maxFrontier;
    r->pathCost = s->pathCost;

}

static void DfsDestroy(void *state) {
    if (state == NULL) return;

    DfsState *s = (DfsState *)state;
    TrackFree(s->seen);
    TrackFree(s->stack);
    TrackFree(s->parent);
    TrackFree(s);
}


const Algo DfsAlgo = {
    .name = "DFS",
    .create = DfsCreate, .step = DfsStep, .done = DfsDone,
    .fillResult = DfsResult, .draw = DfsDraw, .destroy = DfsDestroy
};
