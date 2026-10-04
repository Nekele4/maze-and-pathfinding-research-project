#ifndef DFS_H
#define DFS_H
#include "algorithms.h"

typedef struct {
    int *seen, *stack, *parent;
    int top;
    int current;
    bool done, found;
    int foundCell, pathLength;
    int nodesExpanded, maxFrontier;
} DfsState;

void DfsDraw(void *state, Maze *m, int cellSize);

#endif
