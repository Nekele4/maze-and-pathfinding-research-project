#include <time.h>
#include "algorithms.h"

Result RunAlgo(const Algo *a, Maze *m) {
    void *s = a->create(m);
    clock_t t0 = clock();
    while (!a->done(s)) a->step(s, m);
    double ms = (double)(clock() - t0) * 1000.0 / CLOCKS_PER_SEC;

    Result r = {0};
    r.algorithm = a->name;
    r.timeMs = ms;
    a->fillResult(s, &r);
    a->destroy(s);
    return r;
}
