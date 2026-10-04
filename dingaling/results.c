#include <stdio.h>
#include "results.h"

void AppendResult(const char *filename, Result *r) {
    FILE *f = fopen(filename, "a+");
    if (!f) {
        printf("could not open %s\n", filename);
        return;
    }

    fseek(f, 0, SEEK_END);
    if (ftell(f) == 0) {
        fprintf(f, "seed,cols,rows,obstacleRate,endCount,checksum,algorithm,found,pathLength,nodesExpanded,maxFrontier,timeMs\n");
    }

    fprintf(f, "%u,%d,%d,%d,%d,%u,%s,%d,%d,%d,%d,%.3f\n", r->seed, r->cols, r->rows, r->obstacleRate, r->endCount, r->checksum, r->algorithm, r->found, r->pathLength, r->nodesExpanded, r->maxFrontier, r->timeMs);

    fclose(f);
}
