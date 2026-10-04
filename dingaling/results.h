#ifndef RESULTS_H
#define RESULTS_H

typedef struct {
    unsigned int seed;
    int cols, rows, obstacleRate, endCount;
    unsigned int checksum;
    const char *algorithm;
    int found, pathLength, nodesExpanded, maxFrontier;
    double timeMs;
} Result;

void AppendResult(const char *filename, Result *r);

#endif
