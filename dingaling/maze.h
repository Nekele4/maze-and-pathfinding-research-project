#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

#define MAX_ENDS 16
#define MAX_STARTS 1

// typedefs here, in this order: Rng, Cell, MazeConfig, Maze, Generator
// (Maze contains Rng, MazeConfig and Cell, so those must come first)
typedef struct {
    unsigned int state;
} Rng;

// Cell properties
typedef struct {
    bool north, east, south, west;
    bool visited;
} Cell;

// Maze config for createmaze (Maze+Mazeconfig)
typedef struct {
    unsigned int seed;
    int cols, rows;
    int obstacleRate;   // 0 to 100 NEED TO IMPLEMENT!
    int endCount;
} MazeConfig;

typedef struct {
    Rng rng;
    MazeConfig config;
    Cell *cells;
    int ends[MAX_ENDS];    // cell positions of the endpoints in array
    int starts[MAX_STARTS];
} Maze;

typedef struct {
    int *stack;
    int top;
    bool done;

    int phase;        // 0 = building the maze, 1 = removing extra walls
    int *walls;       // the shuffled list
    int wallCount;    // how many candidates exist
    int wallIndex;    // how many removed so far
    int wallTarget;   // how many to remove in total

} Generator;

extern int dx[4];
extern int dy[4];

unsigned int RngNext(Rng *r);
int RngRange(Rng *r, int min, int max);
Maze CreateMaze(MazeConfig cfg);
Cell *GetCell(Maze *m, int x, int y);
void PickStart(Maze *m);
void PickEnds(Maze *m);
void FreeMaze(Maze *m);
void Carve(Maze *m, int x, int y, int d);
Generator CreateGenerator(Maze *m);
bool InBounds(Maze *m, int x, int y);
void GeneratorStep(Generator *g, Maze *m);
void FreeGenerator(Generator *g);
unsigned int MazeChecksum(Maze *m);

#endif
