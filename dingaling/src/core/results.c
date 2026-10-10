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
        fprintf(f, "seed,cols,rows,obstacleRate,endCount,maxCost,checksum,algorithm,found,pathLength,nodesExpanded,maxFrontier,timeMs,pathCost,totalBytes,peakBytes\n");
    }

    fprintf(f, "%u,%d,%d,%d,%d,%d,%u,%s,%d,%d,%d,%d,%.3f,%d,%zu,%zu\n", r->seed, r->cols, r->rows, r->obstacleRate, r->endCount, r->maxCost, r->checksum, r->algorithm, r->found, r->pathLength, r->nodesExpanded, r->maxFrontier, r->timeMs, r->pathCost, r->totalMemBytes, r->peakMemBytes);

    fclose(f);
}
