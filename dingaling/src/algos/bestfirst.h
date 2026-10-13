#ifndef BESTFIRST_H
#define BESTFIRST_H

#include "algorithms.h"

// Heap functions declaration
typedef struct {
    int cell;
    int prio;
} HeapEntry;

typedef struct {
    HeapEntry *data;
    int size;        // how many entries are in use
    int capacity;    // how many fit before we must grow
} Heap;

void HeapSelfTest(void);   // temporary, removed later

// For dijkstra and astar
typedef struct {
    int *dist;
    int *parent;
    unsigned char *closed;      // 1 = finished
    Heap heap;
    bool useHeuristic;          // false = Dijkstra, true = A*
    int current;
    bool done, found;
    int foundCell;
    int nodesExpanded, maxFrontier;
    int pathLength, pathCost;
} BestFirstState;

void *BestFirstCreate(Maze *m, bool useHeuristic);
void BestFirstStep(void *state, Maze *m);
bool BestFirstDone(void *state);
void BestFirstFill(void *state, Result *r);
void BestFirstDestroy(void *state);
void BestFirstDraw(void *state, Maze *m, int cellSize);


#endif
