import csv
from collections import *
import matplotlib.pyplot as plot
import numpy as np
import os

GROUPS = ["algorithm", "cols", "obstacleRate", "maxCost"]
METRICS = ["pathLength", "nodesExpanded", "maxFrontier", "timeMs", "pathCost"]
LOG_METRICS = ["nodesExpanded", "maxFrontier", "pathLength", "pathCost"]
MEMORY_METRICS = ["totalBytes", "peakBytes"]
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

def WriteSummaryDoc(summary, filename, groupCols, metrics):
    with open(filename, "w") as f:
        f.write("# Results summary\n\n")
        algorithms = sorted(set(key[0] for key in summary))

        for algorithm in algorithms:
            f.write(f"## {algorithm}\n\n")
            header = groupCols[1:] + ["n"] + metrics
            f.write("| " + " | ".join(header) + " |\n")
            f.write("|" + "---|" * len(header) + "\n")

            for key in sorted(k for k in summary if k[0] == algorithm):
                cells = [str(v) for v in key[1:]]            # the key columns after algorithm
                cells.append(str(summary[key]["n"]))
                for metric in metrics:
                    if metric == "timeMs":
                        cells.append(f"{summary[key][metric]:.3f}")
                    else:
                        cells.append(f"{summary[key][metric]:.1f}")
                f.write("| " + " | ".join(cells) + " |\n")
            f.write("\n")

# xaxis needs to be a variable (GROUPS), yaxis needs to be the resulting value (METRICS) 
# size is rows and cols count, rows only for program
def PlotGraph(rows, size, cost, xaxis, yaxis):
    algorithms = sorted(set(row["algorithm"] for row in rows))
    
    for algorithm in algorithms:
        points = []

        for row in rows:
            if row["algorithm"] == algorithm and row["cols"] == size and row["maxCost"] == cost:
                points.append((row[xaxis], row[yaxis]))

        points.sort()
        x = [p[0] for p in points]
        y = [p[1] for p in points]

        plot.plot(x, y, marker="o", label=algorithm, color=COLORS.get(algorithm))

    plot.xlabel(xaxis)
    plot.ylabel("Average: " + yaxis)
    plot.title(f"Average {yaxis} at {size}x{size}, max cost {cost}")
    plot.legend()
    plot.grid()


    os.makedirs("graphs", exist_ok=True)

    filename = f"graphs/graph_{size}x{size}_{xaxis}_{yaxis}_maxcost{cost}.png"

    if yaxis in LOG_METRICS:
        plot.yscale("log")

    plot.savefig(filename)
    plot.close()

def PlotMemory(memSummary, yaxis):
    algorithms = sorted(set(key[0] for key in memSummary))

    for algorithm in algorithms:
        points = []
        for key, values in memSummary.items():
            if key[0] == algorithm:
                cols = key[1]
                points.append((cols*cols, values[yaxis]))

        points.sort()
        x = [p[0] for p in points]
        y = [p[1] for p in points]
        
        plot.plot(x, y, marker="o", label=algorithm, color=COLORS.get(algorithm))

    plot.xscale("log")
    plot.yscale("log")
    plot.xlabel("Cells (cols x cols)")
    plot.ylabel(yaxis + " (bytes)")
    plot.title(f"{yaxis} by maze size")
    plot.legend()
    plot.grid()

    os.makedirs("graphs", exist_ok=True)
    plot.savefig(f"graphs/memory_{yaxis}.png")
    plot.close()

rows = LoadFile("data/resultsmain.csv") + LoadFile("data/results10k.csv")
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


costs = sorted(set(row["maxCost"] for row in summaryRows))
for name, graph in graphs.items():
    for cost in costs:
        for size in sizes:
            PlotGraph(summaryRows, size, cost, graph["xaxis"], graph["yaxis"])

# Memory graphs
memGroups  = Group(rows, ["algorithm", "cols"])
memSummary = Summarize(memGroups, MEMORY_METRICS)
for metric in MEMORY_METRICS:
    PlotMemory(memSummary, metric)

# SUMMARY DOCUMENT
DOC_GROUPS  = ["algorithm", "maxCost", "cols"]
DOC_METRICS = METRICS + MEMORY_METRICS
docGroups   = Group(rows, DOC_GROUPS)
docSummary  = Summarize(docGroups, DOC_METRICS)
WriteSummaryDoc(docSummary, "data/summary.md", DOC_GROUPS, DOC_METRICS)
