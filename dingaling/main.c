#include "raylib.h"
#include "stdio.h"
#include "stdbool.h"
#include "maze.h"
#include "pathing.h"

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
    MazeConfig cfg = { .seed = 41, .cols = 10, .rows = 10, .obstacleRate = 100, .endCount = 1 };
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

    bool checksumPrinted = false;


    while (!WindowShouldClose())
    {
        Controls(&pb, &gen, &maze);
        if (gen.done && !checksumPrinted) {
            printf("checksum: %u\n", MazeChecksum(&maze));
            checksumPrinted = true;
            int nb[4];
            int n = GetNeighbors(&maze, maze.starts[0], nb);
            printf("start %d has %d neighbors:", maze.starts[0], n);
            for (int i = 0; i < n; i++) printf(" %d", nb[i]);
            printf("\n");
        }

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
