#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "maze.h"
#include "results.h"

Result RunBFS(Maze *m);
Result RunDFS(Maze *m);
Result RunDijkstra(Maze *m);
Result RunAStar(Maze *m);

#endif
