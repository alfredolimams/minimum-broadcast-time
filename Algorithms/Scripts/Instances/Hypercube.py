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
import random

tool_description = 'Generate hypercube graphs'
parser = argparse.ArgumentParser(description=tool_description)
# parser.add_argument('-o', dest='output', default='hypercube_graph.txt')
parser.add_argument('-dmin', dest='dmin', type=int, default=1)
parser.add_argument('-dmax', dest='dmax', type=int, default=1)
# Number of gateway
args = parser.parse_args()

for d in range(args.dmin, args.dmax+1):
	graph = nx.hypercube_graph(d)
	file = open(f'HC_{d}.in', 'w')

	gateways = [0]

	number_of_nodes = nx.number_of_nodes(graph)
	number_of_edges = nx.number_of_edges(graph)
	number_of_gateways = len(gateways)

	file.write(f'{number_of_nodes} {number_of_gateways} {number_of_edges}\n')

	idxs = {}

	for idx,v in enumerate(graph.nodes()):
		idxs[v] = idx
		# print(v)

	for e in graph.edges():
		# print(e)
		file.write(f'{idxs[e[0]]+1} {idxs[e[1]]+1}\n')

	for g in gateways:
		file.write(f'{g+1}\n')