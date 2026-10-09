#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "pathing.h"

// returns true if cell c has a wall on side d
static bool HasWall(Cell *c, int d) {
    switch (d) {
        case 0: return c->north;
        case 1: return c->east;
        case 2: return c->south;
        case 3: return c->west;
    }
    return true;
}

int GetNeighbors(Maze *m, int cell, int out[4]) {
    int x = cell % m->config.cols;
    int y = cell / m->config.cols;
    Cell *c = GetCell(m, x, y);
    int count = 0;

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (!HasWall(c, d) && InBounds(m, nx, ny)) {
            out[count] = ny * m->config.cols + nx;
            count++;
        }
    }
    return count;
}

bool IsEnd(Maze *m, int cell) {
    for (int i = 0; i <m->config.endCount; i++) {
        if (m->ends[i] == cell) return true;
    }
    return false;
}

int PathCost(Maze *m, int *parent, int foundCell) {
    int total = 0;
    int cell = foundCell;
    while (parent[cell] != -1) { // start cell = -1 cant give startcell a cost
        total += m->cost[cell];
        cell = parent[cell];
    }

    return total;

}

static size_t g_current = 0, g_peak = 0, g_total = 0;

void *TrackAlloc(size_t n) {
    if (n > SIZE_MAX - sizeof(size_t)) return NULL;

    size_t *base = malloc(n + sizeof(size_t));
    if (base == NULL) return NULL;

    if (n > SIZE_MAX - g_current ||
        n > SIZE_MAX - g_total) {
        free(base);
        return NULL;
    }

    *(size_t *)base = n;

    g_current += n;
    g_total += n;

    if (g_current > g_peak)
        g_peak = g_current;

    return (char *)base + sizeof(size_t);
}

void TrackFree(void *p) {
    if (p == NULL) {
        return;
    }

    size_t *base = (size_t *)((char *)p - sizeof(size_t));

    // makes current that much less that was freed
    if (g_current >= *base) g_current -= *base;
    else g_current = 0;
    free(base);
}

void *TrackCalloc(size_t count, size_t size) {
    if (size != 0 && count > SIZE_MAX / size) return NULL;

    size_t n = count * size;
    void *p = TrackAlloc(n);

    if (p != NULL) memset(p, 0, n);

    return p;
}

void   TrackReset(void)      { g_current = g_peak = g_total = 0; }
size_t TrackPeakBytes(void)  { return g_peak; }
size_t TrackTotalBytes(void) { return g_total; }

