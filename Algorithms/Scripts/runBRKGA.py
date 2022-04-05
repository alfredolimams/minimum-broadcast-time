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

configs = {
    'HI0' : ['HI0', 0.11, 0.10, 0.76],
    # 'HOI0' : ['HOI0', 0.14, 0.12, 0.70],
    # 'HI1' : ['HI1', 0.23, 0.27, 0.71, 0.09],
    # 'HOI1' : ['HOI1', 0.24, 0.25, 0.71, 0.06],
    # 'HI2' : ['HI2', 0.13, 0.19, 0.71, 0.08],
    # 'HOI2' : ['HOI2', 0.17, 0.14, 0.73, 0.12],
    # 'HI3' : ['HI3', 0.12, 0.22, 0.80, 0.08, 0.18],
    # 'HOI3' : ['HOI3', 0.13, 0.18, 0.57, 0.17, 0.05],
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

# print(answers)

path_exe = '../FrameworkMBT2/main.run'
folders = []
folders = folders + [_ for _ in glob.glob(args.path + "**.in", recursive=True)]
for file in sorted(folders):
    print('*',file)
    path_output = file.replace('Inputs', 'Outputs')
    path_output = path_output.replace('.in', '')
    for r in range(int(args.repeat)):
        print('....', r)
        names = []
        for key in sorted(configs.keys()):
            name = path_output + f'_BRKGA_{key}_{r}.out'
            if not path.exists(name):
                names.append(name)
        
        cmd = f'{path_exe} -w -i {file} -g 10000000 -s {r}'
        if args.timelimit != -1:
            cmd += f' -t {args.timelimit}'
        
        cmdOutputs = '-o'
        for name in names:
            cmdOutputs += f' {name}'
        
        cmdConfig = '-c'
        for key in sorted(configs.keys()):
            for par in configs[key]:
                cmdConfig += f' {par}'

        cmdRef = ''
        if file in answers:
            cmdRef = f'-R {answers[file]}'

        os.system(f'{cmd} -d {len(configs)} {cmdConfig} {cmdOutputs} {cmdRef}')
