#include "bestfirst.h"
#include "drawalgo.h"

void BestFirstDraw(void *state, Maze *m, int cellSize) {
    BestFirstState *s = (BestFirstState *)state;
    int cells = m->config.cols * m->config.rows;

    for (int i = 0; i < cells; i++) {
        if (s->closed[i] != 0) {
            DrawCellFill(m, i, cellSize, COL_FINISHED);
        }
        else if (s->dist[i] != -1) {
            DrawCellFill(m, i, cellSize, COL_FRONTIER);
        }
    }

    DrawCellFill(m, s->current, cellSize, COL_CURRENT);

    if (s->found) {
        int cell = s->foundCell;
        while (cell !=-1) {
            DrawCellFill(m, cell, cellSize, COL_PATH);
            cell = s->parent[cell];
        }
    }
}
