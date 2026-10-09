#ifndef DRAWALGO_H
#define DRAWALGO_H
#include "raylib.h"
#include "maze.h"

#define COL_FINISHED  (Color){0, 102, 0, 255}   // dark green
#define COL_FRONTIER  (Color){255, 230, 50, 255}   // yellow
#define COL_CURRENT   (Color){0, 0, 153, 255}   // blue
#define COL_PATH      (Color){164, 0, 213, 255}   // purple

void DrawCellFill(Maze *m, int cell, int cellSize, Color c);

#endif
