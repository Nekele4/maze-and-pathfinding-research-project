#ifndef PATHING_H
#define PATHING_H

#include "maze.h"

int GetNeighbors(Maze *m, int cell, int out[4]);
bool IsEnd(Maze *m, int cell);


#endif
