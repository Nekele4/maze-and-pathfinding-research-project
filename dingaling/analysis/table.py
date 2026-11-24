import csv
import os

SUMMARY   = "data/summary.csv"
RAW_FILES = ["data/resultsmain.csv", "data/results10k.csv"] 
OUT_DIR   = "tables"
ALGOS     = ["BFS", "DFS", "Dijkstra", "AStar"]
REF       = {"cols": 500, "rate": 50, "cost": 10}  
TIE       = 0.01 # tiebreaker
MIN_TIME_SIZE = 500    

def Convert(v):
    try:
        return int(v)
    except ValueError:
        try:
            return float(v)
        except ValueError:
            return v

def LoadFile(filename):
    rows = []
    with open(filename, newline="") as f:
        for row in csv.DictReader(f):
            rows.append({k: Convert(v) for k, v in row.items()})
    return rows

def Index(rows):

    return {(r["algorithm"], r["cols"], r["obstacleRate"], r["maxCost"]): r for r in rows}

def MemoryMeans(files):

    sums = {}
    for filename in files:
        with open(filename, newline="") as f:
            for row in csv.DictReader(f):
                key = (row["algorithm"], int(row["cols"]))
                if key not in sums:
                    sums[key] = [0, 0.0, 0.0]
                sums[key][0] += 1
                sums[key][1] += float(row["totalBytes"])
                sums[key][2] += float(row["peakBytes"])
    means = {}
    for key, (n, total, peak) in sums.items():
        means[key] = {"totalBytes": total / n, "peakBytes": peak / n}
    return means


def Fmt(x, d=None):
    if x is None:
        return "-"
    if d is None:
        d = 0 if abs(x) >= 1000 else (1 if abs(x) >= 10 else 2)
    s = f"{x:,.{d}f}"
    return s.replace(",", "X").replace(".", ",").replace("X", ".")

def Pct(x):
    return "-" if x is None else Fmt(x * 100, 1) + "%"

def Div(a, b):
    if a is None or b is None or b == 0:
        return None
    return a / b

def Name(a):
    return "A*" if a == "AStar" else a

TABLES = []

def Emit(tid, title, header, rows, note=""):
    TABLES.append((tid, title, header, rows, note))
    os.makedirs(OUT_DIR, exist_ok=True)
    with open(f"{OUT_DIR}/{tid}.csv", "w", newline="", encoding="utf-8-sig") as f:
        w = csv.writer(f, delimiter=",")
        w.writerow(header)
        w.writerows(rows)

def WriteMarkdown():
    with open(f"{OUT_DIR}/tables.md", "w", encoding="utf-8") as f:
        for tid, title, header, rows, note in TABLES:
            f.write(f"## Tabel {tid}: {title}\n\n")
            if note:
                f.write(f"*{note}*\n\n")
            f.write("| " + " | ".join(header) + " |\n")
            f.write("|" + "---|" * len(header) + "\n")
            for r in rows:
                f.write("| " + " | ".join(str(c) for c in r) + " |\n")
            f.write("\n")

class Data:
    def __init__(self, idx, mem):
        self.idx = idx
        self.mem = mem
        self.sizes = sorted(set(k[1] for k in idx))
        self.rates = sorted(set(k[2] for k in idx))
        self.costs = sorted(set(k[3] for k in idx))

    def Mean(self, a, cols, rate, cost, metric):
        r = self.idx.get((a, cols, rate, cost))
        return None if r is None else r[metric]

    def Std(self, a, cols, rate, cost, metric):
        r = self.idx.get((a, cols, rate, cost))
        return None if r is None else r[metric + "_std"]

    def N(self, cols, rate, cost):
        r = self.idx.get((ALGOS[0], cols, rate, cost))
        return None if r is None else r["n"]

    def Memory(self, a, cols, metric):
        m = self.mem.get((a, cols))
        return None if m is None else m[metric]

def RefNote(cols=None, rate=None, cost=None):
    parts = []
    if cols is not None: parts.append(f"{cols} x {cols}")
    if rate is not None: parts.append(f"murendichtheid {rate}%")
    if cost is not None: parts.append(f"max kost {cost}")
    return "Vast: " + ", ".join(parts)

def TableA(d):
    c, k = REF["cols"], REF["cost"]
    for tid, metric, label in [("A1", "nodesExpanded", "onderzochte cellen"), ("A2", "maxFrontier", "maximale frontier")]:
        rows = []
        for rate in d.rates:
            rows.append([f"{rate}%"] + [Fmt(d.Mean(a, c, rate, k, metric)) for a in ALGOS])
        Emit(tid, f"effect van murendichtheid op {label}", ["Dichtheid"] + [Name(a) for a in ALGOS], rows,
             RefNote(cols=c, cost=k) + f", n = {d.N(c, d.rates[0], k)} doolhoven per cel. Gemiddelden.")

def TableB(d):
    rate, cost = REF["rate"], REF["cost"]
    rows = []
    for c in d.sizes:
        row = [f"{c} x {c}", Fmt(c * c)]
        for a in ALGOS:
            row.append(Fmt(d.Mean(a, c, rate, cost, "nodesExpanded")))
        for a in ALGOS:
            row.append(Pct(Div(d.Mean(a, c, rate, cost, "nodesExpanded"), c * c)))
        rows.append(row)
    header = ["Grootte", "Cellen"] + [Name(a) for a in ALGOS] + ["Aandeel " + Name(a) for a in ALGOS]
    Emit("B", "effect van grootte op onderzochte cellen", header, rows,
         RefNote(rate=rate, cost=cost) + ". Aandeel = onderzochte cellen / aantal cellen.")

def TableC(d):
    rows = []
    for c in d.sizes:
        row = [f"{c} x {c}"]
        for a in ALGOS:
            row.append(Fmt(d.Memory(a, c, "totalBytes")))
        for a in ALGOS:
            row.append(Fmt(d.Memory(a, c, "peakBytes")))
        for a in ALGOS:
            row.append(Fmt(Div(d.Memory(a, c, "peakBytes"), c * c), 1))
        rows.append(row)
    header = (["Grootte"] + ["Totaal " + Name(a) for a in ALGOS] + ["Piek " + Name(a) for a in ALGOS]
              + ["Piek/cel " + Name(a) for a in ALGOS])
    Emit("C", "geheugengebruik", header, rows,
         "Bytes, gemiddeld over alle dichtheden en kosten. Meet wat de implementaties aanvragen (zonder het doolhof zelf), niet wat de algoritmes in principe nodig hebben.")

def TableD(d):
    rate, cost = REF["rate"], REF["cost"]
    sizes = [c for c in d.sizes if c >= MIN_TIME_SIZE]
    rows = []
    for c in sizes:
        row = [f"{c} x {c}", d.N(c, rate, cost)]
        for a in ALGOS:
            m, s = d.Mean(a, c, rate, cost, "timeMs"), d.Std(a, c, rate, cost, "timeMs")
            row.append("-" if m is None else f"{Fmt(m)} +/- {Fmt(s)}")
        rows.append(row)
    Emit("D", "uitvoeringstijd in ms (gemiddelde +/- standaardafwijking)", ["Grootte", "n"] + [Name(a) for a in ALGOS], rows,
         RefNote(rate=rate, cost=cost) + f". Alleen vanaf {MIN_TIME_SIZE} x {MIN_TIME_SIZE}: de klok meet in hele milliseconden.")

def TableE(d):
    c = REF["cols"]
    rows = []
    for rate in [0, 50, 100]:
        for cost in [1, 10]:
            if d.Mean("BFS", c, rate, cost, "pathLength") is None:
                continue
            row = [f"{rate}%, kost {cost}"]
            for a in ALGOS:
                row.append(Fmt(d.Mean(a, c, rate, cost, "pathLength")))
            for a in ALGOS:
                row.append(Fmt(d.Mean(a, c, rate, cost, "pathCost")))
            for a in ["BFS", "DFS"]:
                row.append(Fmt(Div(d.Mean(a, c, rate, cost, "pathCost"), d.Mean("Dijkstra", c, rate, cost, "pathCost")), 2))
            rows.append(row)
    header = (["Situatie"] + ["Lengte " + Name(a) for a in ALGOS] + ["Kost " + Name(a) for a in ALGOS]
              + ["Kost BFS / Dijkstra", "Kost DFS / Dijkstra"])
    Emit("E", "kwaliteit van het pad", header, rows,
         f"{c} x {c}. 1,00 = optimale kost (Dijkstra). A* heeft dezelfde kost als Dijkstra, dus A* staat niet in de verhoudingskolommen.")

def TableF(d):
    for c in [500, 10000]:
        if c not in d.sizes:
            continue
        rows = []
        for cost in d.costs:
            rows.append([f"kost {cost}"] + [Pct(Div(d.Mean("AStar", c, rate, cost, "nodesExpanded"),
                                                   d.Mean("Dijkstra", c, rate, cost, "nodesExpanded"))) for rate in d.rates])
        Emit(f"F{1 if c == 500 else 2}", f"A* onderzochte cellen als aandeel van Dijkstra ({c} x {c})",
             ["Max kost"] + [f"{r}%" for r in d.rates], rows,
             f"{c} x {c}, per murendichtheid, n = {d.N(c, d.rates[0], d.costs[0])}. Onder 100% onderzoekt A* minder dan Dijkstra.")

def TableG(d):
    c = max(d.sizes)
    rate = REF["rate"]
    rows = []
    for cost in d.costs:
        for a in ALGOS:
            nodes = d.Mean(a, c, rate, cost, "nodesExpanded")
            t = d.Mean(a, c, rate, cost, "timeMs")
            per = None if (nodes in (None, 0) or t is None) else t * 1000 / nodes  
            rows.append([cost, Name(a), Fmt(nodes), Fmt(t), Fmt(per, 3)])
    Emit("G", "tijd tegenover onderzochte cellen", ["Max kost", "Algoritme", "Onderzochte cellen", "Tijd (ms)", "Tijd per cel (microseconden)"], rows,
         f"{c} x {c}, dichtheid {rate}%, n = {d.N(c, rate, d.costs[0])}.")

def Winners(values):
    ok = {a: v for a, v in values.items() if v is not None}
    if not ok:
        return "-"
    best = min(ok.values())
    names = [Name(a) for a, v in ok.items() if v <= best * (1 + TIE) + 1e-12]
    return " / ".join(names)

def TableH(d):
    c = REF["cols"]
    situations = [(rate, cost) for rate in [0, 50, 100] for cost in [d.costs[0], d.costs[-1]]]
    metrics = [("pathLength", "Padlengte"), ("pathCost", "Padkost"), ("nodesExpanded", "Onderzochte cellen"),
               ("maxFrontier", "Frontier"), ("timeMs", "Tijd"), ("peakBytes", "Geheugen (piek)")]
    rows = []
    for metric, label in metrics:
        row = [label]
        for rate, cost in situations:
            vals = {}
            for a in ALGOS:
                if metric == "peakBytes":
                    vals[a] = d.Memory(a, c, "peakBytes")
                elif metric == "timeMs" and c < MIN_TIME_SIZE:
                    vals[a] = None
                else:
                    vals[a] = d.Mean(a, c, rate, cost, metric)
            row.append(Winners(vals))
        rows.append(row)
    header = ["Meetwaarde"] + [f"{r}%, kost {k}" for r, k in situations]
    Emit("H", "beste algoritme per meetwaarde en situatie", header, rows,
         f"{c} x {c}. Laagste waarde wint; verschil onder {int(TIE*100)}% telt als gelijkspel. Geheugen hangt niet van dichtheid of kost af. Controleer ook met 200 en 1000.")

def Series(d, a, metric, factor):
    c0, r0, k0 = REF["cols"], REF["rate"], REF["cost"]
    out = []
    if factor == "size":
        sizes = d.sizes if metric != "timeMs" else [s for s in d.sizes if s >= MIN_TIME_SIZE]
        for c in sizes:
            out.append(Div(d.Mean(a, c, r0, k0, metric), c * c))
    elif factor == "rate":
        for r in d.rates:
            out.append(Div(d.Mean(a, c0, r, k0, metric), c0 * c0))
    else:
        for k in d.costs:
            out.append(Div(d.Mean(a, c0, r0, k, metric), c0 * c0))
    return [v for v in out if v is not None]

def Spread(values):
    if len(values) < 2 or min(values) <= 0:
        return None
    return max(values) / min(values)

def TableI(d):
    metrics = [("nodesExpanded", "cellen/cel"), ("maxFrontier", "frontier/cel"), ("pathCost", "padkost/cel"), ("timeMs", "tijd/cel")]
    rows = []
    for metric, label in metrics:
        for a in ALGOS:
            row = [label, Name(a)]
            for factor in ["size", "rate", "cost"]:
                row.append(Fmt(Spread(Series(d, a, metric, factor)), 2))
            rows.append(row)
    Emit("I", "gevoeligheid per factor (grootste waarde / kleinste waarde)", ["Meetwaarde (per cel)", "Algoritme", "Grootte", "Murendichtheid", "Max kost"], rows,
         f"Eén factor loopt over het hele bereik, de rest op {REF['cols']} x {REF['cols']}, {REF['rate']}%, kost {REF['cost']}. Alles gedeeld door het aantal cellen. Tijd voor grootte: vanaf {MIN_TIME_SIZE}. 1,00 = geen invloed.")

def RatioList(d, factor, fn):
    c0, r0, k0 = REF["cols"], REF["rate"], REF["cost"]
    out = []
    if factor == "size":
        for c in d.sizes: out.append(fn(c, r0, k0))
    elif factor == "rate":
        for r in d.rates: out.append(fn(c0, r, k0))
    else:
        for k in d.costs: out.append(fn(c0, r0, k))
    return [v for v in out if v is not None]

def TableJ(d):
    M = d.Mean
    ratios = [
        ("A* / Dijkstra (cellen)", lambda c, r, k: Div(M("AStar", c, r, k, "nodesExpanded"), M("Dijkstra", c, r, k, "nodesExpanded"))),
        ("BFS / Dijkstra (padkost)", lambda c, r, k: Div(M("BFS", c, r, k, "pathCost"), M("Dijkstra", c, r, k, "pathCost"))),
        ("DFS / BFS (padlengte)", lambda c, r, k: Div(M("DFS", c, r, k, "pathLength"), M("BFS", c, r, k, "pathLength"))),
    ]
    rows = []
    for factor, label in [("size", "Grootte"), ("rate", "Murendichtheid"), ("cost", "Max kost")]:
        row = [label]
        for name, fn in ratios:
            v = RatioList(d, factor, fn)
            row.append("-" if not v else f"{Fmt(min(v), 2)} tot {Fmt(max(v), 2)}")
        rows.append(row)
    Emit("J", "invloed van elke factor op de verhouding tussen algoritmes", ["Factor"] + [n for n, _ in ratios], rows,
         f"Eén factor loopt over het bereik, de rest op {REF['cols']} x {REF['cols']}, {REF['rate']}%, kost {REF['cost']}. Elke cel: kleinste tot grootste verhouding. Hoe breder het bereik, hoe meer de factor het verschil bepaalt.")

def TableK(d):
    cost = REF["cost"]
    structures = [(10, 0), (10, 50), (10, 100), (100, 0), (100, 50), (100, 100), (500, 0), (500, 50), (500, 100), (1000, 0), (1000, 50), (1000, 100), (10000, 0), (10000, 50), (10000, 100)]
    rows = []
    for c, rate in structures:
        row = [f"{c} x {c}, {rate}%"]
        for a in ALGOS:
            row.append(Pct(Div(d.Mean(a, c, rate, cost, "nodesExpanded"), c * c)))
        for a in ALGOS:
            row.append(Fmt(Div(d.Mean(a, c, rate, cost, "pathCost"), d.Mean("Dijkstra", c, rate, cost, "pathCost")), 2))
        rows.append(row)
    header = ["Structuur"] + ["Inspanning " + Name(a) for a in ALGOS] + ["Kwaliteit " + Name(a) for a in ALGOS]
    Emit("K", "doolhofstructuur en efficientie", header, rows,
         f"Kost {cost}. Inspanning = onderzochte cellen / cellen. Kwaliteit = padkost / padkost Dijkstra (1,00 = optimaal).")

def Main():
    d = Data(Index(LoadFile(SUMMARY)), MemoryMeans(RAW_FILES))
    TableA(d); TableB(d); TableC(d); TableD(d); TableE(d); TableF(d)
    TableG(d); TableH(d); TableI(d); TableJ(d); TableK(d)
    WriteMarkdown()
    print(f"{len(TABLES)} tabellen geschreven naar {OUT_DIR}/ (csv per tabel + tables.md)")

Main()