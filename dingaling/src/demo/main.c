#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "maze.h"
#include "pathing.h"
#include "algorithms.h"
#include "testbuilder.h"


typedef struct {
    const Algo *algo;   // which algorithm, e.g. &BfsAlgo
    void *state;        // its state, NULL until the maze is finished
} Runner;

// one unit of work for whatever is active
static void StepActive(Generator *g, Maze *m, Runner *r) {
    if (!g->done) { GeneratorStep(g, m); return; }
    if (!r->state) {
        r->state = r->algo->create(m);             // maze just finished
        if (!r->state) {
            printf("out of memory in %s\n", r->algo->name);
            exit(1);
        }
    }
    if (!r->algo->done(r->state)) r->algo->step(r->state, m);
}

static bool AllDone(Generator *g, Runner *r) {
    return g->done && r->state && r->algo->done(r->state);
}

static void ResetRunner(Runner *r) {
    if (r->state) r->algo->destroy(r->state);   // free the old arrays first
    r->state = NULL;                            // StepActive recreates it on the next step
}

static void SelectAlgo(Runner *r, int index) {
    ResetRunner(r);
    r->algo = algos[index];
}

// Illustration funky monkey
void DrawMaze(Maze *m, int cellSize, Runner *r) {
    // algoritmh goes first then the rest
    if (r->state && r->algo->draw) r->algo->draw(r->state, m, cellSize);

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

// control functions, toggles etc.
typedef struct {
    bool paused;
    int btsFPS;
} Playback;

void Controls(Playback *pb, Generator *g, Maze *m, Runner *r) {
    if (IsKeyPressed(KEY_SPACE)) pb->paused = !pb->paused;
    if (IsKeyPressed(KEY_UP)) pb->btsFPS *= 2;
    if (IsKeyPressed(KEY_DOWN)) pb->btsFPS /= 2;
    if (pb->btsFPS < 1) pb->btsFPS = 1;
    if (pb->btsFPS >4096) pb->btsFPS = 4096;

    for (int k = 0; k < algoCount; k++) {
        if (IsKeyPressed(KEY_ONE + k)) { SelectAlgo(r, k); pb->paused = true; }
    }
    if (IsKeyPressed(KEY_R)) { ResetRunner(r); pb->paused = true; }

    // Generator running
    if (!pb->paused) {
        for (int i = 0; i < pb->btsFPS; i++) {
            if (!AllDone(g, r)) StepActive(g, m, r);
        }
    }

    // move single frame forward/next step of the operation
    if (pb->paused && IsKeyPressed(KEY_RIGHT)) {
        if (!AllDone(g, r)) StepActive(g, m, r);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        while (!AllDone(g,r)) {StepActive(g, m, r);}
    }
}

void DrawSettings(Playback *pb, Generator *g, Maze *m, Runner *r) {
    if (pb->paused) DrawText("PAUSED", 10, 10, 20, RED);
    else if (!pb->paused&&!g->done) DrawText("RUNNING", 10, 10, 20, GREEN);
    else if (g->done) DrawText(TextFormat("FINISHED"), 10, 10, 20, BLUE);
    DrawText(TextFormat("Speed: %d", pb->btsFPS), 10, 40, 20, BLACK);
    DrawText(TextFormat("Obstacle: %d", m->config.obstacleRate), 10, 70, 20, BLACK);
    if (g->phase == 0) DrawText("BUILDING", 10, 100, 20, BLACK);
    if (g->phase == 1) DrawText("DESTROYING", 10, 100, 20, BLACK);
    DrawText(TextFormat("Algo: %s", r->algo->name), 10, 130, 20, BLACK);
}

int main()
{
    MazeConfig cfg = { .seed = 41, .cols = 10, .rows = 10, .obstacleRate = 100, .endCount = 1, .maxCost = 5 };
    Maze maze = CreateMaze(cfg);

    PickStart(&maze);
    PickEnds(&maze);

    Generator gen = CreateGenerator(&maze);

    int windowWidth = 1000;
    int windowHeight = 1000;
    int cellW = windowWidth/cfg.cols;
    int cellH = windowHeight/cfg.rows;
    int cellSize = (cellW < cellH) ? cellW : cellH;
    InitWindow(windowWidth, windowHeight, "MAZE");

    SetTargetFPS(60);
    Playback pb = { .paused = false, .btsFPS = 1 };

    bool checksumPrinted = false;
    bool testsRan = false;

    Runner runner = { .algo = &BfsAlgo, .state = NULL };


    while (!WindowShouldClose())
    {
        Controls(&pb, &gen, &maze, &runner);
        if (gen.done && !checksumPrinted) {
            printf("checksum: %u\n", MazeChecksum(&maze));
            checksumPrinted = true;
        }

        if (testsRan == false && IsKeyPressed(KEY_T)) {
            ResetRunner(&runner);

            RunTests("data/resultsnew.csv");
            printf("tests finished\n");
            testsRan = true;

        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawMaze(&maze, cellSize, &runner);
        DrawSettings(&pb, &gen, &maze, &runner);
        EndDrawing();

    }

    if (runner.state) runner.algo->destroy(runner.state);
    // Close
    CloseWindow();
    FreeMaze(&maze);
    FreeGenerator(&gen);


    return 0;
}
