import csv
from collections import *
import matplotlib.pyplot as plot
import numpy as np
import os

GROUPS = ["algorithm", "cols", "obstacleRate", "maxCost"]
METRICS = ["pathLength", "nodesExpanded", "maxFrontier", "timeMs", "pathCost"]
LOG_METRICS = ["nodesExpanded", "maxFrontier", "pathLength", "pathCost"]
COLORS = {
    "BFS": "#5B009C", #Purple
    "DFS": "#0A9C00", #Green 
    "Dijkstra": "#FF8400", #orange
    "AStar": "#0055FF", #Blue
}

def Convert(values):
    try:
        return int(values) #give int if possible 
    except ValueError:
        try:
            return float(values) # give float if no int
        except ValueError:
            return values #if not able to convert to int or float just give string

def LoadFile(filename):
    rows = []
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            converted = {}
            for column, value in row.items():
                converted[column] = Convert(value)
            rows.append(converted)
    return rows

def Group(rows, columns):
    groups = defaultdict(list)
    for row in rows:
        key = tuple(row[c] for c in columns)
        groups[key].append(row)
    return groups

def Summarize(groups, metrics):
    summary = {}
    for key, rows in groups.items():
        summary[key] = {"n": len(rows)}
        for metric in metrics:
            values = []

            for row in rows:
                values.append(row[metric])
            
            summary[key][metric] = sum(values)/len(values)
    return summary

def WriteSummary(summary, filename):
    fieldnames = GROUPS + ["n"] + METRICS
    with open(filename, "w+", newline="") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()

        for key, values in summary.items():
            row = {}

            for i, column in enumerate(GROUPS):
                row[column] = key[i]

            row["n"] = values["n"]

            for metric in METRICS:
                row[metric] = values[metric]

            writer.writerow(row)

# xaxis needs to be a variable (GROUPS), yaxis needs to be the resulting value (METRICS) 
# size is rows and cols count, rows only for program
def PlotGraph(rows, size, xaxis, yaxis):
    algorithms = sorted(set(row["algorithm"] for row in rows))

    for algorithm in algorithms:
        points = []

        for row in rows:
            if row["algorithm"] == algorithm and row["cols"] == size:
                points.append((row[xaxis], row[yaxis]))

        points.sort()
        x = [p[0] for p in points]
        y = [p[1] for p in points]

        plot.plot(x, y, marker="o", label=algorithm, color=COLORS[algorithm])

    plot.xlabel(xaxis)
    plot.ylabel("Average: " + yaxis)
    plot.title(f"Average {yaxis} at Size: {size}x{size}")
    plot.legend()
    plot.grid()


    os.makedirs("graphs", exist_ok=True)

    filename = f"graphs/graph_{size}_{xaxis}_{yaxis}.png"

    if yaxis in LOG_METRICS:
        plot.yscale("log")

    plot.savefig(filename)
    plot.close()

rows = LoadFile("data/resultsnew.csv")
print(len(rows))
print(rows[0])

groups = Group(rows, GROUPS)
print("Groups: ", len(groups))
for key in list(groups)[:3]:
    print(key, len(groups[key]))

summary = Summarize(groups, METRICS)
WriteSummary(summary, "data/summary.csv")

summaryRows = LoadFile("data/summary.csv")
sizes = sorted(set(row["cols"] for row in summaryRows))
graphs = {}
for metric in METRICS:
    graphs[metric] = {"xaxis": "obstacleRate", "yaxis": metric}

for name, graph in graphs.items():
    for size in sizes:
        PlotGraph(summaryRows, size, graph["xaxis"], graph["yaxis"])



