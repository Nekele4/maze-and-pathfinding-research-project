#include "testbuilder.h"
#include <stdio.h>
#include "maze.h"
#include "algorithms.h"
#include "results.h"

void RunTests(const char *filename) {
    FILE *f = fopen(filename, "w");     // "w" empties the file
    if (f) fclose(f);

    //              1  2   3    4    5    6    7      8
    int sizes[] = {10, 20, 50, 100, 200, 500, 1000, 10000};
    int rates[] = {0, 25, 50, 75, 100};
    int cost[] = {1, 5, 10};

    //                   1    2    3    4    5    6    7    8
    int seedCounts[] = {100, 100, 100, 100, 100, 100, 100, 10};
    int endCount = 1;


    for (size_t s = 0; s<sizeof(sizes)/sizeof(sizes[0]); s++) {
        for (size_t r=0; r < sizeof(rates)/sizeof(rates[0]); r++) {
            for (size_t c=0; c < sizeof(cost)/sizeof(cost[0]); c++) {
                for (unsigned int seed = 1; seed <= (unsigned int)seedCounts[s]; seed++) {

                    MazeConfig cfg = { .seed = seed, .cols = sizes[s], .rows = sizes[s], .obstacleRate = rates[r], .endCount = endCount, .maxCost = cost[c] };
                    Maze maze = BuildMaze(cfg);

                    for (int a = 0; a<algoCount;a++) {
                        Result res = RunAlgo(algos[a], &maze);
                        AppendResult(filename, &res);
                    }
                    FreeMaze(&maze);
                    printf("size %d rate %d cost %d done\n", sizes[s], rates[r], cost[c]);
                }
            }

        }
    }
}
