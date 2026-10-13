#include "bestfirst.h"

static void *DijkstraCreate(Maze *m) { return BestFirstCreate(m, false); }

const Algo DijkstraAlgo = {
    .name = "Dijkstra",
    .create = DijkstraCreate, .step = BestFirstStep, .done = BestFirstDone,
    .fillResult = BestFirstFill, .draw = BestFirstDraw, .destroy = BestFirstDestroy
};
