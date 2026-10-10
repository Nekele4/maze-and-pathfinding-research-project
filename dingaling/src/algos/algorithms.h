#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "maze.h"
#include "results.h"

typedef struct {
    const char *name;
    void *(*create)(Maze *m);
    void  (*step)(void *state, Maze *m);
    bool  (*done)(void *state);
    void  (*fillResult)(void *state, Result *r);
    void  (*draw)(void *state, Maze *m, int cellSize);
    void  (*destroy)(void *state);
} Algo;

extern const Algo BfsAlgo;
extern const Algo DfsAlgo;

extern const Algo *algos[];
extern const int algoCount;

Result RunAlgo(const Algo *a, Maze *m);

#endif
