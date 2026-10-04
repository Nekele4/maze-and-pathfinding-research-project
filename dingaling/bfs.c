#include <stdlib.h>
#include <time.h>
#include "algorithms.h"
#include "pathing.h"
#include "bfs.h"

#define UNREACHED (-1)

static void *BfsCreate(Maze *m) {
    BfsState *s = calloc(1, sizeof(BfsState));
    int cells = m->config.cols * m->config.rows;
    s->dist = malloc(cells * sizeof(int));
    s->queue = malloc(cells * sizeof(int));
    s->parent = malloc(cells * sizeof(int));

    for (int i=0; i<cells; i++) {
        s->dist[i] = UNREACHED;
        s->parent[i] = -1;
    }

    int start = m->starts[0];
    s->dist[start] = 0;
    s->queue[s->tail] = start;
    s->tail++;
    s->current = start;
    return s;
}

static void BfsStep(void *state, Maze *m) {
    BfsState *s = (BfsState *)state;
    if (s->done) return;
    if (s->head >= s->tail) {
        s->done = true;
        return;
    }

    int cur = s->queue[s->head];
    s->head++;
    s->current = cur;
    s->nodesExpanded++;

    if(IsEnd(m, cur)) {
        s->found = true;
        s->foundCell = cur;
        s->done = true;
        return;
    }

    int nb[4];
    int n = GetNeighbors(m, cur, nb);

    for (int i = 0; i < n; i++) {
        if (s->dist[nb[i]] == UNREACHED) {
            s->dist[nb[i]] = s->dist[cur] + 1;
            s->queue[s->tail] = nb[i];
            s->parent[nb[i]] = cur;
            s->tail++;
        }
    }
    if (s->tail - s->head > s->maxFrontier) {
        s->maxFrontier = s->tail - s->head;
    }

    if (s->head >= s->tail) s->done = true;

}

static bool BfsDone(void *state) { return ((BfsState *)state)->done; }

static void BfsResult(void *state, Result *r) {
    BfsState *s = (BfsState *)state;
    r->found = s->found ? 1 : 0;
    r->pathLength = s->found ? s->dist[s->foundCell] : 0;
    r->nodesExpanded = s->nodesExpanded;
    r->maxFrontier = s->maxFrontier;

}

static void BfsDestroy(void *state) {
    BfsState *s = (BfsState *)state;
    free(s->dist); free(s->queue); free(s->parent);
    free(s);
}

const Algo BfsAlgo = {
    .name = "BFS",
    .create = BfsCreate, .step = BfsStep, .done = BfsDone,
    .fillResult = BfsResult, .draw = BfsDraw, .destroy = BfsDestroy
};
