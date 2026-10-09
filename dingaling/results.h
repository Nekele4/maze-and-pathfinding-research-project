#ifndef RESULTS_H
#define RESULTS_H

#include <stddef.h>

typedef struct {
    unsigned int seed;
    int cols, rows, obstacleRate, endCount, maxCost;
    unsigned int checksum;
    const char *algorithm;
    int found, pathLength, nodesExpanded, maxFrontier;
    double timeMs;
    int pathCost;
    size_t totalMemBytes, peakMemBytes;
} Result;

void AppendResult(const char *filename, Result *r);

#endif
