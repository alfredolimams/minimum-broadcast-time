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

# Arguments

parser = argparse.ArgumentParser(description=tool_description)
parser.add_argument('-p', dest='path', default='../../')
parser.add_argument('-m', dest='mode', default='execute')
parser.add_argument('-r', dest='repeat', default=1)
parser.add_argument('-t', dest='timelimit', default=-1)
parser.add_argument('-aco', dest='aco', default=False)
parser.add_argument('-brkga', dest='brkga', default=False)
parser.add_argument('-brkga-lazy', dest='brkga_lazy', default=False)
parser.add_argument('-brkga-slater', dest='brkga_slater', default=False)
parser.add_argument('-slater', dest='slater', default=False)
parser.add_argument('-ilp1', dest='ilp1', default=False)
parser.add_argument('-ilp2', dest='ilp2', default=False)
parser.add_argument('-ilp3', dest='ilp3', default=False)
parser.add_argument('-ilp4', dest='ilp4', default=False)
args = parser.parse_args()

path_aco = '../ACO/main.run'
path_brkga = '../BRKGA/main.run'
path_brkga_lazy = '../BRKGA-Lazy/main.run'
path_brkga_slater = '../BRKGA-Slater/main.run'
path_sousaILP = '../Sousa_ILP/main.run'
path_slater = '../Slater/main.run'

print(parser.description)

if args.mode == 'execute':
    folders = []
    folders = folders + [_ for _ in glob.glob(args.path + "**.in", recursive=True)]
    for file in folders:
        print('*',file)
        path_output = file.replace('Inputs', 'Outputs')
        path_output = path_output.replace('.in', '')
        for r in range(int(args.repeat)):
            label = str(r)
            brkga = path_output + f'_BRKGA_{label}.out'
            brkga_lazy = path_output + f'_BRKGA-Lazy_{label}.out'
            brkga_slater = path_output + f'_BRKGA-Slater_{label}.out'
            aco = path_output + f'_ACO_{label}.out'
            if not path.exists(brkga) and args.brkga:
                os.system(f'./{path_brkga} -w -S -t {args.timelimit} -i {file} -o {brkga} -e 0.22 -m 0.26 -r 0.66 -K 9 -g 2000 -p 250 -T 1 -s {r}')

            if not path.exists(brkga_lazy) and args.brkga_lazy:
                os.system(f'./{path_brkga_lazy} -w -S -t {args.timelimit} -i {file} -o {brkga_lazy} -e 0.22 -m 0.26 -r 0.66 -K 9 -g 2000 -p 250 -T 1 -s {r}')

            if not path.exists(brkga_slater) and args.brkga_slater:
                os.system(f'./{path_brkga_slater} -w -S -t {args.timelimit} -i {file} -o {brkga_slater} -e 0.22 -m 0.26 -r 0.66 -K 9 -g 2000 -p 250 -T 1 -s {r}')

            if not path.exists(aco) and args.aco:
                os.system(f'./{path_aco} -w -S -t {args.timelimit} -i {file} -o {aco} -g 2000 -p 250 -T 1')
        
        if args.slater:
            slater = path_output + f'_Slater.out'        
            if not path.exists(slater):
                os.system(f'./{path_slater} -S -i {file} -o {slater}')

        if args.ilp1:
            ilp1 = path_output + f'_ILP1.out'        
            if not path.exists(ilp1):
                os.system(f'./{path_sousaILP} -t 3600.0 -i {file} -o {ilp1} >> logging_ILP1.txt')

        if args.ilp2:
            ilp2 = path_output + f'_ILP2_1.out'        
            if not path.exists(ilp2):
                os.system(f'./{path_sousaILP} -t 3600.0 -i {file} -o {ilp2} -l >> logging_ILP2_1.txt')
            ilp2 = path_output + f'_ILP2_2.out'        
            if not path.exists(ilp2):
                os.system(f'./{path_sousaILP} -t 3600.0 -i {file} -o {ilp2} -l -L >> logging_ILP2_2.txt')
        
        if args.ilp3 or args.ilp4:
            outputs = sorted([_ for _ in glob.glob(path_output + '*BRKGA*.out')])
            upperBound = 1<<20
            for output in outputs:
                MBTfile = open(output, 'r')
                mbt = int(MBTfile.readline().split()[1])
                upperBound = min(upperBound, mbt)

            if args.ilp3:
                ilp3 = path_output + f'_ILP3.out'
                if not path.exists(ilp3):
                    os.system(f'./{path_sousaILP} -t 3600.0 -i {file} -o {ilp3} -u {upperBound} >> logging_ILP3.txt')

            if args.ilp4:
                ilp4 = path_output + f'_ILP4.out'
                if not path.exists(ilp4):
                    os.system(f'./{path_sousaILP} -t 3600.0 -i {file} -o {ilp4} -u {upperBound} -l >> logging_ILP4.txt')

