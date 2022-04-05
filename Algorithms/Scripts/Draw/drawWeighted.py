import networkx as nx
import matplotlib.pyplot as plt
import multiprocessing 
import time 
import glob
import sys
from parse import parse
import argparse
import os
from os import path
import math

tool_description = 'Draw graph - by Alfredo Lima'

# Arguments

parser = argparse.ArgumentParser(description=tool_description)
parser.add_argument('-i', dest='input')
args = parser.parse_args()

file = open(args.input)

G = nx.Graph()

qtVertices, qtGateways, qtEdges = file.readline().split()

weights = file.readline().split()

for _ in range(int(qtEdges)):
	u, v, w = file.readline().split()
	G.add_edge(int(u),int(v),weight=int(w))

gateways = []
for _ in range(int(qtGateways)):
	gateways.append(int(file.readline()))

color_map = []
for node in G:
	if node in gateways:
		color_map.append('green')
	else:
		color_map.append('red')

labeldict = {}
for idx, node in enumerate(G):
	labeldict[node] = f'{node}: {weights[node-1]}'

# pos = nx.circular_layout(G)
# pos = nx.kamada_kawai_layout(G)
pos = nx.planar_layout(G)
labels = nx.get_edge_attributes(G,'weight')
nx.draw(G, pos, labels=labeldict, node_color=color_map, with_labels=True, node_size=800)
nx.draw_networkx_edge_labels(G,pos,edge_labels=labels)
plt.show()