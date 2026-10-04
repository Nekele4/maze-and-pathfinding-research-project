#include "testbuilder.h"
#include <stdio.h>
#include "maze.h"
#include "algorithms.h"
#include "results.h"

void RunTests(const char *filename) {
    int sizes[] = {10, 20, 50, 100, 200, 500};
    int rates[] = {0, 25, 50, 75, 90, 100};
    int seedCount = 100;
    int endCount = 1;


    for (int s = 0; s<sizeof(sizes)/sizeof(rates[0]); s++) {
        for (int r=0; r < sizeof(rates)/sizeof(rates[0]); r++) {
            for (unsigned int seed = 1; seed <= seedCount; seed++) {
                MazeConfig cfg = { .seed = seed, .cols = sizes[s], .rows = sizes[s], .obstacleRate = rates[r], .endCount = endCount};
                Maze maze = BuildMaze(cfg);

                for (int a = 0; a<algoCount;a++) {
                    Result res = RunAlgo(algos[a], &maze);
                    AppendResult(filename, &res);
                }
                FreeMaze(&maze);
            }
        }
    }
}
