import numpy as np
from scipy import spatial
import networkx as nx
import matplotlib.pyplot as plt
nnodes = 50
r = 0.175
positions =  np.random.rand(nnodes,2)
kdtree = spatial.KDTree(positions)
pairs = kdtree.query_pairs(r)
G = nx.Graph()
G.add_nodes_from(range(nnodes))
G.add_edges_from(list(pairs))
pos = dict(zip(range(nnodes),positions))
nx.draw(G,pos)
plt.show()