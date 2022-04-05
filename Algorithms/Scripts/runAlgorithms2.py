import multiprocessing 
import time 
import glob
import sys
from parse import parse
import argparse
import os
from os import path
import math

tool_description = 'Run algorithms to MBT - by Alfredo Lima'

# python3 runBRKGA.py -t 60 -r 10 -p ../../All\ Instances/Inputs/

# Arguments

parser = argparse.ArgumentParser(description=tool_description)
parser.add_argument('-p', dest='path', default='../../')
parser.add_argument('-r', dest='repeat', default=1)
parser.add_argument('-t', dest='timelimit', default=-1)
args = parser.parse_args()

print(parser.description)

############ BRKGA BP0
## populationElite      populationMutants   rhoe
## 0.11                 0.10                0.76
############ BRKGA FRFS0
## populationElite      populationMutants   rhoe
## 0.14                 0.12                0.70
############ BRKGA BP1
## populationElite      populationMutants   rhoe    alpha
## 0.23                 0.27                0.71    0.09
############ BRKGA FRFS1
## populationElite      populationMutants   rhoe    alpha
## 0.24                 0.25                0.71    0.06
############ BRKGA BP2
## populationElite      populationMutants   rhoe    beta
## 0.13                 0.19                0.71    0.08
############ BRKGA FRFS2
## populationElite      populationMutants   rhoe    beta
## 0.17                 0.14                0.73    0.12
############ BRKGA BP3 
## populationElite      populationMutants   rhoe    alpha   beta
## 0.12                 0.22                0.80    0.08    0.18
############ BRKGA FRFS3 
## populationElite      populationMutants   rhoe    alpha   beta
## 0.12                 0.22                0.80    0.08    0.18

def decoder2string(data):
	d = data[0]
	e = data[1]
	m = data[2]
	r = data[3]
	cmd = f'-d {d} -e {e} -m {m} -r {r}'
	if '1' in d:
		a = data[4]
		cmd += f' -a {a}'
	if '2' in d:
		b = data[4]
		cmd += f' -b {b}'
	if '3' in d:
		a = data[4]
		b = data[5]
		cmd += f' -a {a} -b {b}'

	return cmd

algorithms = {
	'BRKGA' : {
		'BP0' : ['BP0', 0.11, 0.10, 0.76],
		'BP1' : ['BP1', 0.23, 0.27, 0.71, 0.09],
		'BP2' : ['BP2', 0.13, 0.19, 0.71, 0.08],
		'BP3' : ['BP3', 0.12, 0.22, 0.80, 0.08, 0.18],

		'BPI0' : ['BPI0', 0.11, 0.10, 0.76],
		'BPI1' : ['BPI1', 0.23, 0.27, 0.71, 0.09],
		'BPI2' : ['BPI2', 0.13, 0.19, 0.71, 0.08],
		'BPI3' : ['BPI3', 0.12, 0.22, 0.80, 0.08, 0.18],

		'FRFS0' : ['FRFS0', 0.14, 0.12, 0.70],
		'FRFS1' : ['FRFS1', 0.24, 0.25, 0.71, 0.06],
		'FRFS2' : ['FRFS2', 0.17, 0.14, 0.73, 0.12],
		'FRFS3' : ['FRFS3', 0.12, 0.22, 0.80, 0.08, 0.18],

		'FRFSI0' : ['FRFSI0', 0.14, 0.12, 0.70],
		'FRFSI1' : ['FRFSI1', 0.24, 0.25, 0.71, 0.06],
		'FRFSI2' : ['FRFSI2', 0.17, 0.14, 0.73, 0.12],
		'FRFSI3' : ['FRFSI3', 0.12, 0.22, 0.80, 0.08, 0.18],

		'MST' : ['MST', 0.16, 0.10, 0.74],    
	},
	'ACS' : None,
	'HYBRID' : None,
}

answers = {
}
path_results = args.path + 'results.txt'

try:
	file = open(path_results, 'r')
	for line in file.readlines():
		try:
			name, mbt = line.split()
			answers[args.path + name] = mbt
		except Exception as e:
			continue
except Exception as e:
	print('Sem arquivo de referência')

path_exe = {
	'BRKGA' : '../BRKGAs/FrameworkMBT/framework_mbt',
	'HYBRID' : '../BRKGAs/BRKGA-MIP/hybrid_mbt',
	'ACS' : '../ACS/acs_mbt'
}

folders = []
folders = folders + [_ for _ in glob.glob(args.path + "**.in", recursive=True)]
for file in sorted(folders):
	print('*',file)
	path_output = file.replace('Inputs', 'Outputs')
	path_output = path_output.replace('.in', '')
	for r in range(int(args.repeat)):
		print('....', r)
		for method in algorithms.keys():
			if method == 'BRKGA':        
				for decoder in algorithms[method].keys():
					name = path_output + f'_{method}_{decoder}_{r}.out'
					if path.exists(name):
						continue
									
					cmd = f'{path_exe[method]} -w -i {file} -g 100000000 -s {r}'
					if args.timelimit != -1:
						cmd += f' -t {args.timelimit}'
				
					cmdOutput = f'-o {name}'
			
					cmdConfig = decoder2string(algorithms[method][decoder])

					cmdRef = ''
					if file in answers:
						cmdRef = f'-R {answers[file]}'

					os.system(f'{cmd} {cmdConfig} {cmdOutput} {cmdRef}')
					# print(f'{cmd} {cmdConfig} {cmdOutput} {cmdRef}')
			elif method == 'ACS':
				name = path_output + f'_{method}_{r}.out'
				if path.exists(name):
					continue
									
				cmd = f'{path_exe[method]} -w -i {file} -g 100000000'

				if args.timelimit != -1:
					cmd += f' -t {args.timelimit}'
				
				cmdOutput = f'-o {name}'
				os.system(f'{cmd} {cmdOutput}')
			elif method == 'HYBRID':
				name = path_output + f'_{method}_{r}.out'
				if path.exists(name):
					continue
								
				cmd = f'{path_exe[method]} -w -i {file} -g 100000000 -s {r}'
				if args.timelimit != -1:
					cmd += f' -t {args.timelimit}'
			
				cmdOutput = f'-o {name}'

				os.system(f'{cmd} {cmdOutput}')