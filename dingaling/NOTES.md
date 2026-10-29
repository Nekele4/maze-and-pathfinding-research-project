# MAZE PATHFINDING BENCHMARK: run notes

## Provenance
- Code: git tag final-run, commit: 26af658
- Repository: https://github.com/Nekele4/maze-and-pathfinding-research-project/commit/26af6583216675bbe64fa615a83249cc31c5932b
- Machine specifications:
    - CPU: AMD Ryzen 7 2700 8 Cores 3200 Mhz
    - GPU: Nvidia RTX 2060 Super
    - Ram: 16 GB DDR4 DIMM clockspeed: 2133 MHz
    - Motherboard: Gigabyte Technology B450M Gaming
    - Operating system: Windows 10 Home

- Date of runs: [13th October 2026, 14th October 2026]
- Reproducibility check: seed 41 at (100x100, rate: 50%, no cost) has checksum: 1710022561 
- (seed 41, 100x100, rate 50, maxCost 1, 1 end: checksum 1710022561 (verified identical on two machines))


## Data and files
| file | sizes | seeds per config | rows |
|---|---|---|---|
| data/resultsmain.csv | 10, 20, 50, 100, 200, 500, 1000 | 100 (50 for size 1000) | 85,800 |
| data/results10k.csv | 10000 | 10 | 1,320 |

Both: obstacle rates 0, 10, ..., 100; maxCost 1, 5, 10; endCount 1; 4 algorithms.

Joined: 87,120 rows, 1,056 groups of (algorithm, size, rate, maxCost).

Raw CSVs are backed up at [/datafilesbackup/data] and set read-only.

The data analyser script generates the following files from the data collected: summary.csv, summary.md.

From which it further generates graphs, to be found in [/graphs] and backed up at [/datafilesbackup/graphs]

## Maze generation

## Cost model

## Algorithm rules

## Measurements and results

## Checksum

## Checks done

## Limitations