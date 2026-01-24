import matplotlib.pyplot as plt
import networkx as nx

# Defining the graphs for all possible partial orders on V = {1, 2, 3, 4}
# Note: We can't exhaustively generate all 15 partial orders programmatically in one go due to complexity
# So we will manually create a selection of representative Hasse diagrams

# 1. Discrete partial order (antichain) - no comparability
G1 = nx.DiGraph()
G1.add_nodes_from([1, 2, 3, 4])

# 2. Total order (chain) - 1 < 2 < 3 < 4
G2 = nx.DiGraph()
G2.add_edges_from([(1, 2), (2, 3), (3, 4)])

# 3. Diamond-shaped partial order (1 < 2 and 3 < 4 independently)
G3 = nx.DiGraph()
G3.add_edges_from([(1, 2), (3, 4)])

# 4. Mixed order (1 < 2 < 4, 3 independent)
G4 = nx.DiGraph()
G4.add_edges_from([(1, 2), (2, 4)])

# 5. Another mixed order (1 < 2, 1 < 3, 2 < 4, 3 < 4)
G5 = nx.DiGraph()
G5.add_edges_from([(1, 2), (1, 3), (2, 4), (3, 4)])

# Defining a function to draw Hasse diagrams
def draw_hasse_diagram(G, pos, ax):
    nx.draw(G, pos, with_labels=True, node_size=500, node_color='lightblue', arrows=False, ax=ax)
    ax.set_title("Hasse Diagram")

# Creating subplots to display the diagrams
fig, axs = plt.subplots(2, 3, figsize=(12, 8))
axs = axs.flatten()

# Layout and plot each diagram
pos1 = {1: (1, 1), 2: (2, 1), 3: (3, 1), 4: (4, 1)}
draw_hasse_diagram(G1, pos1, axs[0])
axs[0].set_title("Antichain (Discrete Order)")

pos2 = {1: (1, 1), 2: (2, 2), 3: (3, 3), 4: (4, 4)}
draw_hasse_diagram(G2, pos2, axs[1])
axs[1].set_title("Total Order (Chain)")

pos3 = {1: (1, 2), 2: (2, 3), 3: (3, 2), 4: (4, 3)}
draw_hasse_diagram(G3, pos3, axs[2])
axs[2].set_title("Diamond-shaped Order")

pos4 = {1: (1, 1), 2: (2, 2), 3: (3, 1), 4: (4, 3)}
draw_hasse_diagram(G4, pos4, axs[3])
axs[3].set_title("1 < 2 < 4, 3 Independent")

pos5 = {1: (1, 1), 2: (2, 2), 3: (3, 2), 4: (4, 3)}
draw_hasse_diagram(G5, pos5, axs[4])
axs[4].set_title("1 < 2, 1 < 3, 2 < 4, 3 < 4")

# Adjust layout
plt.tight_layout()
plt.show()
