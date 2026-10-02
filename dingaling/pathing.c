#include "pathing.h"

// returns true if cell c has a wall on side d
static bool HasWall(Cell *c, int d) {
    switch (d) {
        case 0: return c->north;
        case 1: return c->east;
        case 2: return c->south;
        case 3: return c->west;
    }
    return true;
}

int GetNeighbors(Maze *m, int cell, int out[4]) {
    int x = cell % m->config.cols;
    int y = cell / m->config.cols;
    Cell *c = GetCell(m, x, y);
    int count = 0;

    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];

        if (!HasWall(c, d) && InBounds(m, nx, ny)) {
            out[count] = ny * m->config.cols + nx;
            count++;
        }
    }
    return count;
}
