"""
Farmer – Fox – Goose – Grain puzzle solver
------------------------------------------
State-space search using BFS with clean graph visualization.
"""

from collections import deque
import matplotlib.pyplot as plt

try:
    import networkx as nx
    HAVE_NX = True
except ImportError:
    HAVE_NX = False


# ---------- State representation ----------
def isValid(state):
    F, X, G, M = state
    # Goose alone with fox
    if F != X and X == G:
        return False
    # Goose alone with grain
    if F != G and G == M:
        return False
    return True


def getNeighbors(state):
    F, X, G, M = state
    result = []
    # Farmer moves alone
    candidate = (1 - F, X, G, M)
    if isValid(candidate):
        result.append(candidate)

    # Farmer takes fox
    if F == X:
        candidate = (1 - F, 1 - X, G, M)
        if isValid(candidate):
            result.append(candidate)

    # Farmer takes goose
    if F == G:
        candidate = (1 - F, X, 1 - G, M)
        if isValid(candidate):
            result.append(candidate)

    # Farmer takes grain
    if F == M:
        candidate = (1 - F, X, G, 1 - M)
        if isValid(candidate):
            result.append(candidate)

    return result


# ---------- BFS search ----------
def bfs(start, goal):
    q = deque([start])
    parent = {start: None}

    while q:
        cur = q.popleft()
        if cur == goal:
            path = []
            while cur is not None:
                path.append(cur)
                cur = parent[cur]
            return list(reversed(path))

        for nb in getNeighbors(cur):
            if nb not in parent:
                parent[nb] = cur
                q.append(nb)
    return None


# ---------- Pretty printing ----------
def stateLabel(state):
    F, X, G, M = state
    banks = ["L", "R"]
    return f"F={banks[F]} X={banks[X]} G={banks[G]} M={banks[M]}"


def printSolution(path):
    print("Solution found (BFS shortest path):")
    for i, s in enumerate(path):
        print(f"Step {i}: {stateLabel(s)}")


# ---------- Main ----------
if __name__ == "__main__":
    start = (0, 0, 0, 0)
    goal = (1, 1, 1, 1)

    path = bfs(start, goal)

    if path:
        printSolution(path)
    else:
        print("No solution found.")

    # ---------- Graph drawing ----------
    allStates = [(F, X, G, M)
                 for F in (0, 1)
                 for X in (0, 1)
                 for G in (0, 1)
                 for M in (0, 1)
                 if isValid((F, X, G, M))]

    edges = []
    for s in allStates:
        for n in getNeighbors(s):
            if n in allStates:
                edges.append((s, n))

    if HAVE_NX:
        G = nx.DiGraph()
        for s in allStates:
            G.add_node(s)
        for e in edges:
            G.add_edge(*e)

        pos = nx.spring_layout(G, seed=2)
        plt.figure(figsize=(10, 8))

        pathSet = set(path)
        nodeColors = ["red" if n in pathSet else "lightgray" for n in G.nodes()]
        nx.draw_networkx_nodes(G, pos, node_size=800, node_color=nodeColors)
        nx.draw_networkx_edges(G, pos, arrows=True, alpha=0.3)

        # draw labels separately, slightly shifted up
        labelPos = {n: (x, y + 0.08) for n, (x, y) in pos.items()}
        nx.draw_networkx_labels(G, labelPos,
                                {n: stateLabel(n) for n in G.nodes()},
                                font_size=7)

        # highlight path edges
        pathEdges = []
        if path:
            for i in range(len(path) - 1):
                pathEdges.append((path[i], path[i + 1]))
        nx.draw_networkx_edges(G, pos, edgelist=pathEdges,
                               edge_color="red", width=2, arrows=True)

        plt.title("Farmer–Fox–Goose–Grain: state graph (solution in red)")
        plt.axis("off")
        plt.show()
    else:
        print("Install networkx for graph drawing (pip install networkx).")
