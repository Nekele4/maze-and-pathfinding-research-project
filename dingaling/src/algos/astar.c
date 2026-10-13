#include "bestfirst.h"

static void *AStarCreate(Maze *m) { return BestFirstCreate(m, true); }

const Algo AStarAlgo = {
    .name = "AStar",
    .create = AStarCreate, .step = BestFirstStep, .done = BestFirstDone,
    .fillResult = BestFirstFill, .draw = BestFirstDraw, .destroy = BestFirstDestroy
};
