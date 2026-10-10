#ifndef BFS_H
#define BFS_H
#include "algorithms.h"

typedef struct {
    int *dist, *queue, *parent;
    int head, tail;
    int current;            // cell expanded last step (for highlighting)
    bool done, found;
    int foundCell;
    int nodesExpanded, maxFrontier;
    int pathCost;
} BfsState;

void BfsDraw(void *state, Maze *m, int cellSize);

#endif
