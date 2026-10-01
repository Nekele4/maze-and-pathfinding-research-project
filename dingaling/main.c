#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#define MAX_ENDS 16
#define MAX_STARTS 1
#include "raymath.h"

//Random number gen cuz why would GenerateRandomValue work
typedef struct {
    unsigned int state;
} Rng;

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

// Declare directions
int dx[4] = { 0, 1, 0, -1 };   // dx[0]=0, dx[1]=1, dx[2]=0, dx[3]=-1
int dy[4] = { -1, 0, 1, 0 };   // dy[0]=-1, dy[1]=0, dy[2]=1, dy[3]=0

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

//
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

// Illustration funky monkey
void DrawMaze(Maze *m, int cellSize) {
    int s = m->starts[0];
    DrawRectangle((s % m->config.cols) * cellSize, (s / m->config.cols) * cellSize, cellSize, cellSize, GREEN);
    // then a for loop over endCount doing the same with m->ends[i] and RED
    for (int i = 0; i < m->config.endCount; i++) {
        DrawRectangle((m->ends[i] % m->config.cols) * cellSize, (m->ends[i] / m->config.cols) * cellSize, cellSize, cellSize, RED);
    }
    for (int y=0; y < m->config.rows; y++) {
        for (int x = 0; x < m->config.cols; x++) {
            Cell *c = GetCell(m, x, y);
            int px = x*cellSize;
            int py = y*cellSize;
            if (c->north) DrawLine(px, py, px + cellSize, py, BLACK);
            if (c->south) DrawLine(px, py+cellSize, px+cellSize, py+cellSize, BLACK);
            if (c->west) DrawLine(px, py, px, py + cellSize, BLACK);
            if (c->east) DrawLine(px+cellSize, py, px+cellSize, py+cellSize, BLACK);
        }
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
        int temp = g->walls[i];        // 1. put walls[i] in the spare cup
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

// control functions, toggles etc.
typedef struct {
    bool paused;
    int btsFPS;
} Playback;

void Controls(Playback *pb, Generator *g, Maze *m) {
    if (IsKeyPressed(KEY_SPACE)) pb->paused = !pb->paused;
    if (IsKeyDown(KEY_UP)) pb->btsFPS++;
    if (IsKeyDown(KEY_DOWN)) pb->btsFPS--;
    if (pb->btsFPS < 1) pb->btsFPS = 1;
    if (pb->btsFPS >100) pb->btsFPS = 100;

    // Generator running
    if (!pb->paused) {
        for (int i = 0; i < pb->btsFPS; i++) {
            if (!g->done) GeneratorStep(g, m);
        }
    }

    // move single frame forward/next step of the operation
    if (pb->paused && IsKeyPressed(KEY_RIGHT)) {
        if (!g->done) GeneratorStep(g, m);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        while (!g->done) {GeneratorStep(g, m);}
    }
}

void DrawSettings(Playback *pb, Generator *g, Maze *m) {
    if (pb->paused) DrawText("PAUSED", 10, 10, 20, RED);
    else if (!pb->paused&&!g->done) DrawText("RUNNING", 10, 10, 20, GREEN);
    else if (g->done) DrawText(TextFormat("FINISHED"), 10, 10, 20, BLUE);
    DrawText(TextFormat("Speed: %d", pb->btsFPS), 10, 40, 20, BLACK);
    DrawText(TextFormat("Obstacle: %d", m->config.obstacleRate), 10, 70, 20, BLACK);
    if (g->phase == 0) DrawText("BUILDING", 10, 100, 20, BLACK);
    if (g->phase == 1) DrawText("DESTROYING", 10, 100, 20, BLACK);
}

int main()
{
    MazeConfig cfg = { .seed = 42, .cols = 10, .rows = 10, .obstacleRate = 50, .endCount = 1 };
    Maze maze = CreateMaze(cfg);

    PickStart(&maze);
    PickEnds(&maze);

    Generator gen = CreateGenerator(&maze);
    printf("stack top: %d, first item: %d, start: %d\n", gen.top, gen.stack[0], maze.starts[0]);



    int windowWidth = 1000;
    int windowHeight = 1000;
    int cellW = windowWidth/cfg.cols;
    int cellH = windowHeight/cfg.rows;
    int cellSize = (cellW < cellH) ? cellW : cellH;
    InitWindow(windowWidth, windowHeight, "MAZE");

    SetTargetFPS(60);
    Playback pb = { .paused = false, .btsFPS = 1 };

    while (!WindowShouldClose())
    {
        Controls(&pb, &gen, &maze);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawMaze(&maze, cellSize);
        DrawSettings(&pb, &gen, &maze);
        EndDrawing();

    }

    // Close
    CloseWindow();
    FreeMaze(&maze);
    FreeGenerator(&gen);

    return 0;
}
