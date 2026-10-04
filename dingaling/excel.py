""" import csv
from collections import defaultdict

data = defaultdict(lambda: defaultdict(lambda: defaultdict(list)))

class Averages():
    def __init__(self):
        self.pathLength = 0
        self.nodesExpanded = 0
        self.maxFrontier = 0
        self.timeMS = 0

averages = defaultdict(lambda: defaultdict(lambda: defaultdict(Averages)))

metrics = [
    "pathLength",
    "nodesExpanded",
    "maxFrontier",
    "timeMs"
]

def Average(rows, metric):


def ChooseAlgo():
    print("select algorith to analyse:" )
    selection = input()
    while selection not in data:
        print("Choose again: ")
        selection = input()

    return selection

with open('resultsnew.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    
    for row in reader:
        algorithm = row["algorithm"]
        size = int(row["cols"])
        obstacle = int(row["obstacleRate"])

        data[algorithm][size][obstacle].append(row)

selection = ChooseAlgo()
selected = data[selection]

rows = data["BFS"][100][50]
values = []

for row in rows:
    values.append(int(row["nodesExpanded"]))

print("Average node expanded BFS 50: ", sum(values) / len(values))

print("Selected:", selection)
print("Rows:", len(rows))        
"""
import csv
from collections import *
import matplotlib.pyplot as plot
import numpy as np
import os

GROUPS = ["algorithm", "cols", "obstacleRate"]
METRICS = ["pathLength", "nodesExpanded", "maxFrontier"]

def Convert(values):
    try:
        return int(values) #give int if possible 
    except ValueError:
        try:
            return float(values) # give float if no int
        except ValueError:
            return values #if not able to convert to int or float just give string

def LoadResults(filename):
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
            # YOUR PART: collect row[metric] for every row in rows,
            # then summary[key][metric] = sum / count
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

def LoadSummary(filename):
    rows = []
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            converted = {}
            for column, value in row.items():
                converted[column] = Convert(value)
            rows.append(converted)
    return rows

#hardcoded needs fix!
graphs = {
    "pathLength": {
        "xaxis": "obstacleRate",
        "yaxis": "pathLength",
        "sizes": [10, 20, 50, 100, 200, 500]
    },

    "nodesExpanded": {
        "xaxis": "obstacleRate",
        "yaxis": "nodesExpanded",
        "sizes": [10, 20, 50, 100, 200, 500]
    },

    "maxFrontier": {
        "xaxis": "obstacleRate",
        "yaxis": "maxFrontier",
        "sizes": [10, 20, 50, 100, 200, 500]
    },

    "runtime": {
        "xaxis": "obstacleRate",
        "yaxis": "timeMs",
        "sizes": [10, 20, 50, 100, 200, 500]
    }
}

# xaxis needs to be a variable (GROUPS), yaxis needs to be the resulting value (METRICS) 
# size is rows and cols count, rows only for program
def PlotGraph(rows, size, xaxis, yaxis):
    algorithms = set(row["algorithm"] for row in rows)

    for algorithm in algorithms:
        x = []
        y = []

        for row in rows:
            if row["algorithm"] == algorithm and row["cols"] == size:
                x.append(row[xaxis])
                y.append(row[yaxis])

        plot.plot(x, y, marker="o", label=algorithm)

    plot.xlabel(xaxis)
    plot.ylabel("Average: " + yaxis)
    plot.title(f"Average {yaxis} at Size: {size}x{size}")
    plot.legend()
    plot.grid()


    os.makedirs("graphs", exist_ok=True)

    filename = f"graphs/graph_{size}_{xaxis}_{yaxis}.png"

    plot.savefig(filename)
    plot.close()

rows = LoadResults("resultsnew.csv")
print(len(rows))
print(rows[0])

groups = Group(rows, GROUPS)
print("Groups: ", len(groups))
for key in list(groups)[:3]:
    print(key, len(groups[key]))

summary = Summarize(groups, METRICS)
WriteSummary(summary, "summary.csv")

summaryRows = LoadSummary("summary.csv")

for name, graph in graphs.items():
    for size in graph["sizes"]:
        PlotGraph(summaryRows, size, graph["xaxis"], graph["yaxis"])



