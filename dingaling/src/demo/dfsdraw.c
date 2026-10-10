#include "dfs.h"
#include "drawalgo.h"

void DfsDraw(void *state, Maze *m, int cellSize) {
    DfsState *s = (DfsState *)state;
    int cells = m->config.cols * m->config.rows;

    for (int i = 0; i < cells; i++) {
        if (s->seen[i] != 0) DrawCellFill(m, i, cellSize, COL_FINISHED);
    }
    for (int i = 0; i< (s->top); i++) {
        DrawCellFill(m, s->stack[i], cellSize, COL_FRONTIER);
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
