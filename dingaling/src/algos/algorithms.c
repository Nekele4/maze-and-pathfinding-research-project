#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "algorithms.h"
#include "pathing.h"


const Algo *algos[] = {&BfsAlgo, &DfsAlgo};
const int algoCount = sizeof(algos) / sizeof(algos[0]);

Result RunAlgo(const Algo *a, Maze *m) {
    TrackReset();
    void *s = a->create(m);
    if (s == NULL) {
        printf("out of memory in %s\n", a->name);
        exit(1);
    }
    clock_t t0 = clock();
    while (!a->done(s)) a->step(s, m);
    double ms = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;

    Result r = {0};
    r.seed = m->config.seed;
    r.cols = m->config.cols;
    r.rows = m->config.rows;
    r.obstacleRate = m->config.obstacleRate;
    r.endCount = m->config.endCount;
    r.maxCost = m->config.maxCost;
    r.checksum = MazeChecksum(m);
    r.algorithm = a->name;
    r.timeMs = ms;
    r.totalMemBytes = TrackTotalBytes();
    r.peakMemBytes = TrackPeakBytes();
    a->fillResult(s, &r);
    a->destroy(s);
    return r;
}
