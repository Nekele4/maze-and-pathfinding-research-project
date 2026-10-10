#include "drawalgo.h"

void DrawCellFill(Maze *m, int cell, int cellSize, Color c) {

    int x = cell % m->config.cols;
    int y = cell / m->config.cols;

    DrawRectangle(x*cellSize, y * cellSize, cellSize, cellSize, c);
}
