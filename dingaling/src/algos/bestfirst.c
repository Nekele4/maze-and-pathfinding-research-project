#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bestfirst.h"
#include "pathing.h"

#define UNREACHED (-1)

// Heuristics for Astar
static int Heuristic(Maze *m, int cell) {
    int x = cell % m->config.cols;
    int y = cell / m->config.cols;
    int best = 1 << 30;

    for (int i = 0; i < m->config.endCount; i++) {
        int ex = m->ends[i] % m->config.cols;
        int ey = m->ends[i] / m->config.cols;
        int d = abs(x - ex) + abs(y - ey);
        if (d < best) best = d;                  // keep the smallest
    }
    return best;
}

// true if a should come out of the heap before b
static bool Less(HeapEntry a, HeapEntry b) {
    if (a.prio != b.prio) return a.prio < b.prio;
    return a.cell < b.cell;       // tie-break: lower cell number first
}

static bool HeapPush(Heap *h, int cell, int prio) {
    // If full, double capacity
    if (h->size == h->capacity) {
        int newCap = h->capacity * 2;

        HeapEntry *bigger = TrackAlloc((size_t)newCap * sizeof(HeapEntry));

        if (bigger == NULL) return false;

        memcpy(bigger, h->data, (size_t)h->size * sizeof(HeapEntry));
        TrackFree(h->data);
        h->data = bigger;
        h->capacity = newCap;
    }

    // 2. sift up
    HeapEntry e = { cell, prio };
    int i = h->size;
    h->size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!Less(e, h->data[parent])) break;   // parent is smaller: stop
        h->data[i] = h->data[parent];           // pull the parent down
        i = parent;                             // and move up one level
    }
    h->data[i] = e;                             // drop the new item in its final slot
    return true;
}

// caller must check h->size > 0 first
static HeapEntry HeapPop(Heap *h) {
    HeapEntry top = h->data[0];                 // the answer
    h->size--;
    HeapEntry last = h->data[h->size];          // the item that fills the hole
    int i = 0;

    while(1) {
        int child = 2*i + 1;

        if (child >= h->size) break;

        if (child + 1 < h->size && Less(h->data[child + 1], h->data[child])) {
            child++;                              // right child is smaller: use it
        }

        if (!Less(h->data[child], last)) break;

        h->data[i] = h->data[child];
        i = child;
    }

    if (h->size > 0) h->data[i] = last;         // drop last into its final slot
    return top;
}

void *BestFirstCreate(Maze *m, bool useHeuristic) {
    BestFirstState *s = TrackCalloc(1, sizeof(BestFirstState));
    if (s == NULL) return NULL;

    int cells = m->config.cols * m->config.rows;

    s->dist   = TrackAlloc((size_t)cells * sizeof(int));
    s->parent = TrackAlloc((size_t)cells * sizeof(int));
    s->closed = TrackCalloc((size_t)cells, 1);
    s->heap.capacity = 1024;
    s->heap.size = 0;
    s->heap.data = TrackAlloc(1024 * sizeof(HeapEntry));

    // No leakage errors
    if (s->dist == NULL || s->parent == NULL || s->closed == NULL || s->heap.data == NULL) {
        TrackFree(s->dist);
        TrackFree(s->parent);
        TrackFree(s->closed);
        TrackFree(s->heap.data);
        TrackFree(s);
        return NULL;
    }

    for (int i = 0; i < cells; i++) {
        s->dist[i] = UNREACHED;
        s->parent[i] = -1;
    }

    s->useHeuristic = useHeuristic;
    int start = m->starts[0];
    s->dist[start] = 0;
    s->current = start;

    int prio = useHeuristic ? Heuristic(m, start) : 0;
    HeapPush(&s->heap, start, prio);
    s->maxFrontier = 1;

    return s;
}

void BestFirstStep(void *state, Maze *m) {
    BestFirstState *s = (BestFirstState *)state;
    if (s->done) return;

    HeapEntry e;
    do {
        if (s->heap.size == 0) { s->done = true; return; }   // nothing left: no route
        e = HeapPop(&s->heap);
    } while (s->closed[e.cell]);                              // stale? pop again

    int cur = e.cell;
    s->closed[cur] = 1;
    s->current = cur;
    s->nodesExpanded++;

    if (IsEnd(m, cur)) {
        s->found = true;
        s->foundCell = cur;
        s->pathCost = s->dist[cur];
        s->pathLength = PathSteps(s->parent, cur);
        s->done = true;
        return;
    }

    int nb[4];
    int n = GetNeighbors(m, cur, nb);
    for (int i = 0; i < n; i++) {
        int v = nb[i];
        if (s->closed[v]) continue;                 // already final, skip

        int nd = s->dist[cur] + m->cost[v];     // (cost so far + cost of entering v)
        if (s->dist[v] == UNREACHED || nd < s->dist[v]) {
            s->dist[v] = nd;
            s->parent[v] = cur;
            int prio = nd + (s->useHeuristic ? Heuristic(m, v) : 0);
            if (!HeapPush(&s->heap, v, prio)) {
                printf("out of memory");
                exit(1);
            }
        }
    }

    if (s->heap.size > s->maxFrontier) s->maxFrontier = s->heap.size;
}

bool BestFirstDone(void *state) { return ((BestFirstState *)state)->done; }

void BestFirstFill(void *state, Result *r) {
    BestFirstState *s = (BestFirstState *)state;
    r->found         = s->found ? 1 : 0;
    r->pathLength    = s->found ? s->pathLength : 0;
    r->pathCost      = s->found ? s->pathCost : 0;
    r->nodesExpanded = s->nodesExpanded;
    r->maxFrontier   = s->maxFrontier;
}

void BestFirstDestroy(void *state) {
    if (state == NULL) return;
    BestFirstState *s = (BestFirstState *)state;
    TrackFree(s->heap.data);
    TrackFree(s->dist);
    TrackFree(s->parent);
    TrackFree(s->closed);
    TrackFree(s);
}
