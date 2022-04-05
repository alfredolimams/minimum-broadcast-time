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
parser.add_argument('-gmin', dest='min_gateway', default=1, type=int)
parser.add_argument('-gmax', dest='max_gateway', default=1, type=int)
parser.add_argument('-dmin', dest='min_density', default=10, type=int)
parser.add_argument('-dstep', dest='step_density', default=5, type=int)
parser.add_argument('-dmax', dest='max_density', default=25, type=int)
parser.add_argument('-s', dest='samples', default=2, type=int)
parser.add_argument('-v', dest='vertices', default=8, type=int)
parser.add_argument('-m', dest='method', default='bo', type=str)
args = parser.parse_args()

methods = {
	'bo' : 'WeightedRandomGraphOptimum.py -t b',
	'ro' : 'WeightedRandomGraphOptimum.py',
	'r' : 'WeightedRandomGraph.py',
}

if not args.method in methods:
	print('Invalid argument "-m"')
	quit() 

for i in range(args.min_gateway,args.max_gateway+1):
	for d in range(args.min_density,args.max_density+1,args.step_density):
		for s in range(1,args.samples+1):
			os.system(f'python3 {methods[args.method]} -g {i} -minV {args.vertices} -maxV {args.vertices} -minD {d/100} -maxD {d/100} -o {args.method.upper()}{int(args.vertices*i)}_G{i}_D{d}_{s}.in -minWV 0 -maxWV 5 -minWE 1 -maxWE 6')