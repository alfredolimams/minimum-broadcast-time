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

############ BRKGA H0
## populationElite 		populationMutants 	rhoe
## 0.11              	0.10 				0.76

############ BRKGA HO0
## populationElite 		populationMutants 	rhoe
## 0.14              	0.12 				0.70

############ BRKGA H1
## populationElite 		populationMutants 	rhoe 	alpha
## 0.23              	0.27 				0.71  	0.09

############ BRKGA HO1
## populationElite 		populationMutants 	rhoe 	alpha
## 0.24              	0.25 				0.71  	0.06

############ BRKGA H2
## populationElite 		populationMutants 	rhoe 	beta
## 0.13              	0.19 				0.71 	0.08

############ BRKGA HO2
## populationElite 		populationMutants 	rhoe 	beta
## 0.17              	0.14 				0.73 	0.12

############ BRKGA H3 
## populationElite 		populationMutants 	rhoe 	alpha	beta
## 0.12              	0.22 				0.80  	0.08 	0.18

config = ['BRKGA-MIP', 'HOI0', 0.14, 0.12, 0.70]

answers = {
}
path_results = args.path + 'results.txt'

# try:
#     file = open(path_results, 'r')
#     for line in file.readlines():
#         try:
#             name, mbt = line.split()
#             answers[args.path + name] = mbt
#         except Exception as e:
#             continue
# except Exception as e:
#     print('Sem arquivo de referência')

path_exe = '../BRKGAs/Hybrid/hybrid_mbt'
folders = []
folders = folders + [_ for _ in glob.glob(args.path + "**.in", recursive=True)]
for file in sorted(folders):
    print('*',file)
    path_output = file.replace('Inputs', 'Outputs')
    path_output = path_output.replace('.in', '')
    for r in range(int(args.repeat)):
        print('...', r)
        name = path_output + f'_{config[0]}_{r}.out'
        log = path_output + f'_{config[0]}_{r}.log'
        if path.exists(name):
            continue

        cmd = f'{path_exe} -w -i {file} -g 10000000 -s {r}'
        if args.timelimit != -1:
            cmd += f' -t {args.timelimit}'
        
        cmdOutput = f'-o {name}'
        
        cmdConfig = f'-d {config[1]} -e {config[2]} -m {config[3]} -r {config[4]}'

        cmdLog = f'>> {log}'

        os.system(f'{cmd} {cmdConfig} {cmdOutput} {cmdLog}')
