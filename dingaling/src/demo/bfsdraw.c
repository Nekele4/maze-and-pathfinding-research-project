#include "bfs.h"
#include "drawalgo.h"

void BfsDraw(void *state, Maze *m, int cellSize) {
    BfsState *s = (BfsState *)state;
    int cells = m->config.cols * m->config.rows;

    for (int i = 0; i < cells; i++) {
        if (s->dist[i] != -1) DrawCellFill(m, i, cellSize, COL_FINISHED);
    }
    for (int i = s->head; i< (s->tail); i++) {
        DrawCellFill(m, s->queue[i], cellSize, COL_FRONTIER);
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
