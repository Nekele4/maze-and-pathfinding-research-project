#ifndef PATHING_H
#define PATHING_H

#include <stddef.h>
#include <string.h>
#include "maze.h"

int GetNeighbors(Maze *m, int cell, int out[4]);

// Returns if reached end cell
bool IsEnd(Maze *m, int cell);

// Cell costs
int PathCost(Maze *m, int *parent, int foundCell);

// tracker
void *TrackAlloc(size_t n);
void TrackFree(void *p);
void *TrackCalloc(size_t count, size_t size);
void TrackReset(void);
size_t TrackPeakBytes(void);
size_t TrackTotalBytes(void);


#endif



