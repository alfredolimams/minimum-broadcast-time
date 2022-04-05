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

tool_description = 'Generate connected random graph'
parser = argparse.ArgumentParser(description=tool_description)
# Number of gateway
parser.add_argument('-g', dest='gateway', default=1, type=int)
# Number of vertices
parser.add_argument('-minV', dest='minVertices', default=100, type=int)
parser.add_argument('-maxV', dest='maxVertices', default=100, type=int)
# Density of graph
parser.add_argument('-minD', dest='minDensity', default=0, type=float)
parser.add_argument('-maxD', dest='maxDensity', default=1, type=float)
# Interval of weights of vertices
parser.add_argument('-minWV', dest='minWeightedVertex', default=0, type=int)
parser.add_argument('-maxWV', dest='maxWeightedVertex', default=0, type=int)
# Interval of weights of vertices
parser.add_argument('-minWE', dest='minWeightedEdge', default=1, type=int)
parser.add_argument('-maxWE', dest='maxWeightedEdge', default=1, type=int)
# Name of output file
parser.add_argument('-o', dest='output', default='random_graph.txt')
args = parser.parse_args()

trees = []

totalVertices = 0
for _ in range(int(args.gateway)):
	vertices = int(args.minVertices) + random.random() * ( int(args.maxVertices) - int(args.minVertices) )
	vertices = int(vertices)
	totalVertices += vertices
	trees.append( nx.random_tree(vertices) )

density = float(args.minDensity) + random.random() * ( float(args.maxDensity) - float(args.minDensity) )
graph = nx.gnp_random_graph(totalVertices, density)

offset = 0
gateways = []
for t in trees:
	number_of_nodes = nx.number_of_nodes(t)
	g = int(random.random() * (number_of_nodes - 1))
	gateways.append( g + offset ) 
	for e in t.edges():
		u, v = e
		graph.add_edge(u + offset, v + offset)
	offset += number_of_nodes

file = open(args.output, 'w')

number_of_nodes = nx.number_of_nodes(graph)
number_of_edges = nx.number_of_edges(graph)
number_of_gateways = len(gateways)

file.write(f'{number_of_nodes} {number_of_gateways} {number_of_edges}\n')

for i in range(number_of_nodes):
	if i:
		file.write(' ')
	file.write(f'{random.randint(args.minWeightedVertex, args.maxWeightedVertex)}')
file.write('\n')

for e in graph.edges():
	file.write(f'{e[0]+1} {e[1]+1} ')
	file.write(f'{random.randint(args.minWeightedEdge, args.maxWeightedEdge)}\n')

for g in gateways:
	file.write(f'{g+1}\n')