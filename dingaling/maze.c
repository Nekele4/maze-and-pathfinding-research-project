#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "maze.h"

int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };

unsigned int RngNext(Rng *r) {
    unsigned int x = r->state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    r->state = x;
    return x;
}

// random int from min to max, both included (like GetRandomValue)
int RngRange(Rng *r, int min, int max) {
    return min + (int)(RngNext(r) % (unsigned int)(max - min + 1));
}

Maze CreateMaze(MazeConfig cfg) {
    // clamps for grid enz
    if (cfg.obstacleRate < 0)   cfg.obstacleRate = 0;
    if (cfg.obstacleRate > 100) cfg.obstacleRate = 100;
    if (cfg.endCount < 1)         cfg.endCount = 1;
    if (cfg.endCount > MAX_ENDS)  cfg.endCount = MAX_ENDS;

    if (cfg.endCount > cfg.cols*cfg.rows-1) cfg.endCount = cfg.cols*cfg.rows - 1;

    if (cfg.seed == 0) cfg.seed = (unsigned int)time(NULL);

    printf("Seed: %u\n", cfg.seed);

    Maze m;

    m.rng.state = cfg.seed * 2654435761u;
    if (m.rng.state == 0) m.rng.state = 1;
    for (int i = 0; i < 10; i++) RngNext(&m.rng);

    m.config = cfg;
    m.cells = malloc(cfg.cols * cfg.rows * sizeof(Cell));

    //init for maze gen
    for (int i = 0; i < cfg.cols*cfg.rows; i++) {
        m.cells[i].north = true;
        m.cells[i].east = true;
        m.cells[i].south = true;
        m.cells[i].west = true;
        m.cells[i].visited = false;
    }
    return m;
}

// find cell type ding
Cell *GetCell(Maze *m, int x, int y) {
    return &m->cells[y * m->config.cols + x];
}

// name speaks for itself lowkey
void PickStart(Maze *m) {
    m->starts[0] = RngRange(&m->rng, 0, m->config.cols * m->config.rows - 1);
    printf("Start: %d\n", m->starts[0]);
}

void PickEnds(Maze *m) {
    for (int i = 0; i < m->config.endCount; i++) {   // once per endpoint
        int pick;
        bool duplicate;
        do {
            pick = RngRange(&m->rng, 0, m->config.cols * m->config.rows - 1);
            duplicate = false;

            if (pick==m->starts[0]) {
                duplicate = true;
            }

            for (int j = 0; j<i; j++) {
                if (m->ends[j] == pick) {
                    duplicate = true;
                }

            }
        } while (duplicate);
        m->ends[i] = pick;
        printf("Ends: %d\n", pick);

    }
}

// release memory
void FreeMaze(Maze *m) {
    free(m->cells);
    m->cells = NULL;
}

void ClearWall(Cell *c, int d) {
    switch (d) {
        case 0: c->north = false; break;
        case 1: c->east = false;  break;
        case 2: c->south = false; break;
        case 3: c->west = false; break;
    }
}

void Carve(Maze *m, int x, int y, int d) {
    int nx = x + dx[d];
    int ny = y + dy[d];

    Cell *a = GetCell(m, x, y);
    Cell *b = GetCell(m, nx, ny);

    ClearWall(a, d);          // wall to walk thru
    ClearWall(b, (d+2)%4) ;        // break it again from other room cuz we like that
}


Generator CreateGenerator(Maze *m) {
    Generator g;
    g.stack = malloc(m->config.cols * m->config.rows * sizeof(int));
    g.top = 0;
    g.done = false;

    // phases
    g.phase = 0;
    g.walls = NULL;
    g.wallCount = 0;
    g.wallIndex = 0;
    g.wallTarget = 0;

    int start = m->starts[0];
    int x = start % m->config.cols;
    int y = start / m->config.cols;
    Cell *startCell = GetCell(m, x, y);
    startCell->visited = true;

    g.stack[g.top] = start;            // Mundo mundo mundo mundo
    g.top++;

    return g;
}

// Vibe check to see if "neighbor" in bouns
bool InBounds(Maze *m, int x, int y) {
    return x >= 0 && x < m->config.cols && y >= 0 && y < m->config.rows;
}

void PrepareWalls(Generator *g, Maze *m) {
    int cols = m->config.cols;
    int rows = m->config.rows;
    g->walls = malloc(2 * cols * rows * sizeof(int));  // max walls
    g->wallCount = 0;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            Cell *c = GetCell(m, x, y);
            int idx = y * cols + x;

            if (c->east && x < cols - 1) {
                g->walls[g->wallCount] = idx * 4 + 1;
                g->wallCount++;
            }
            if (c->south && y < rows -1) {
                g->walls[g->wallCount] = idx * 4 + 2;
                g->wallCount++;
            }
        }
    }

    // shuffle: go from the end, swap each item with a random earlier one
    for (int i = g->wallCount - 1; i > 0; i--) {
        int j = RngRange(&m->rng, 0, i);
        int temp = g->walls[i];        // 1. put walls[i] in the spare
        g->walls[i] = g->walls[j];     // 2. overwrite it with walls[j]
        g->walls[j] = temp;            // 3. put the saved value into j
    }

    g->wallTarget = g->wallCount * (100 - m->config.obstacleRate) / 100;
    g->wallIndex = 0;
    printf("walls left: %d, to remove: %d\n", g->wallCount, g->wallTarget);
}

//Walkie talkie
void GeneratorStep(Generator *g, Maze *m) {
    if (g->phase == 0) {
        if (g->top == 0) {
            PrepareWalls(g, m);
            g->phase = 1;
            return;
        }                                          // no stacks = we good fam

        int cur = g->stack[g->top - 1];                // peep the cell we're standing in
        int x = cur % m->config.cols;
        int y = cur / m->config.cols;

        int options[4];
        int count = 0;

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (InBounds(m, nx, ny) && !GetCell(m, nx, ny)->visited) {
                options[count] = d;
                count++;
            }

        }

        if (count > 0) {
            int d = options[RngRange(&m->rng, 0, count - 1)];
            int nx = x + dx[d];
            int ny = y + dy[d];

            Carve(m, x, y, d);
            GetCell(m, nx, ny)->visited = true;

            g->stack[g->top] = ny * m->config.cols + nx;
            g->top++;
        } else {
            g->top--;     // stuck = step back go again
        }
    }
    else {
        if (g->wallIndex >= g->wallTarget) { g->done = true; return; }
        int entry = g->walls[g->wallIndex];
        int cell = entry / 4;
        int d    = entry % 4;
        int x = cell % m->config.cols;
        int y = cell / m->config.cols;
        Carve(m, x, y, d);
        g->wallIndex++;
    }
}

// release memory from malloc stack malloc walls
void FreeGenerator(Generator *g) {
    free(g->stack);
    g->stack = NULL;

    free(g->walls);
    g->walls = NULL;
}
// CALL IT!!!!

unsigned int MazeChecksum(Maze *m) {
    unsigned int h = 2166136261u;                  // any fixed starting number
    for (int i = 0; i < m->config.cols * m->config.rows; i++) {
        Cell *c = &m->cells[i];
        unsigned int bits = c->north | (c->east << 1) | (c->south << 2) | (c->west << 3);
        h = (h ^ bits) * 16777619u;                // mix this cell in
    }
    return h;
}


