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

qtVertices, _, qtEdges = file.readline().split()
for _ in range(int(qtEdges)):
	u, v = file.readline().split()
	G.add_edge(int(u),int(v))

nx.draw_kamada_kawai(G, with_labels=True)
plt.show()