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
parser.add_argument('-o', dest='output', default='random_graph')
# Path of SCHA-Weighted
parser.add_argument('-s', dest='scha', default='../../General/SCHAWeighted/scha_weighted')
parser.add_argument('-l', dest='lower', default='../../General/LowerBoundWeighted/lbweighted_mbt')
parser.add_argument('-t', dest='type', default='r')
args = parser.parse_args()

if not '.in' in args.output:
	args.output += '.in'

solutionName = args.output.replace('.in', '.sol')

# Build g trees
trees = []
totalVertices = 0
for _ in range(int(args.gateway)):
	if args.type == 'r':
		vertices = int(args.minVertices) + random.random() * ( int(args.maxVertices) - int(args.minVertices) )
		vertices = int(vertices)
		totalVertices += vertices
		trees.append( nx.random_tree(vertices) )
	else:
		min_log = math.ceil(math.log2(args.minVertices))
		max_log = math.ceil(math.log2(args.maxVertices))
		power = min_log + int( random.random() * (max_log - min_log) ) 
		vertices = 2**power
		totalVertices += vertices
		trees.append( nx.binomial_tree(power) )

# Merge the trees for build a forest
forest = nx.Graph()
offset = 0
gateways = []
for t in trees:
	number_of_nodes = nx.number_of_nodes(t)
	g = int(random.random() * (number_of_nodes - 1))
	gateways.append( g + offset ) 
	for e in t.edges():
		u, v = e
		forest.add_edge(u + offset, v + offset)
	offset += number_of_nodes

file = open('temp.in', 'w')

number_of_nodes = nx.number_of_nodes(forest)
number_of_edges = nx.number_of_edges(forest)
number_of_gateways = len(gateways)

file.write(f'{number_of_nodes} {number_of_gateways} {number_of_edges}\n')

# Generation random weights for vertices
Wv = {}
for i in range(number_of_nodes):
	if i:
		file.write(' ')
	Wv[ i+1 ] = random.randint(args.minWeightedVertex, args.maxWeightedVertex)
	file.write(f'{Wv[ i+1 ]}')
file.write('\n')

# Generation random weights for edges
We = {}
for e in forest.edges():
	v_min, v_max = min(e[0]+1,e[1]+1), max(e[0]+1,e[1]+1)
	We[(v_min, v_max)] = random.randint(args.minWeightedEdge, args.maxWeightedEdge)
	file.write(f'{v_min} {v_max} ')
	file.write(f'{We[(v_min, v_max)]}\n')

for g in gateways:
	file.write(f'{g+1}\n')

# Compute MBT
file.close()
os.system(f'./{args.scha} -w -i temp.in -o {solutionName}')

# Get information
file = open(solutionName)

mbt = int(file.readline().split()[1])
file.readline()

mbts = {}
mbts_without_setup = {}
for g in gateways:
	mbts_without_setup[ g+1 ] = 0
	mbts[ g+1 ] = Wv[ g+1 ]

while True:
	try:
		line = file.readline()
		line = line.replace(':', '')
		line = line.replace(',', '')
		_, t, *transmissions = line.split()
		t = int(t)
		transmissions = list(transmissions)
		for transmission in transmissions:
			u, v = transmission.split('-')
			u, v = int(u), int(v)
			v_min, v_max = min(u, v), max(u, v)
			mbts_without_setup[v] = t + We[(v_min, v_max)] - Wv[u]
			mbts[v] = mbts_without_setup[v] + Wv[v]	+ Wv[u]
	except Exception as e:
		break

file.close()

density = float(args.minDensity) + random.random() * ( float(args.maxDensity) - float(args.minDensity) )
random_graph = nx.gnp_random_graph(totalVertices, density)

# Merge forest with random graph
graph = nx.Graph()
for e in forest.edges():
	graph.add_edge(*e)
for e in random_graph.edges():
	graph.add_edge(*e)

number_of_nodes = nx.number_of_nodes(graph)
number_of_edges = nx.number_of_edges(graph)
number_of_gateways = len(gateways)

file = open(args.output, 'w')
# Information of graph
file.write(f'{number_of_nodes} {number_of_gateways} {number_of_edges}\n')

# Weights of vertices
for i in range(number_of_nodes):
	if i:
		file.write(' ')
	file.write(f'{Wv[ i+1 ]}')
file.write('\n')

# Weights of edges
for e in sorted(graph.edges()):
	v_min, v_max = min(e[0]+1,e[1]+1), max(e[0]+1,e[1]+1)
	file.write(f'{v_min} {v_max} ')
	if not (v_min, v_max) in We:
		# d = abs(mbts_without_setup[v_min] - mbts_without_setup[v_max]) + Wv[v_min] + Wv[v_max] + random.randint(1,10)
		d = mbt + random.randint(1,10)
		We[(v_min, v_max)] = d
	file.write(f'{We[(v_min, v_max)]}\n')

# Gateways
for g in gateways:
	file.write(f'{g+1}\n')

file.close()

os.system(f'./{args.lower} -w -i {args.output} -o temp.out')

file = open('temp.out')
lw = file.readline().split()[1]
file.close()

os.system(f'rm temp.in')
os.system(f'rm temp.out')

file = open(args.output, 'a')
file.write(f'{lw} {sum(Wv.values())+sum(We.values())} {mbt}\n')
file.close()