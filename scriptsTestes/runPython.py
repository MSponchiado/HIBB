#!/usr/bin/python3

#Process level parallelism for shell commands
import sys
import subprocess as sp
import multiprocessing as mp
from os.path import dirname as dirname

global program
program = dirname(sys.argv[0])+'/configInst.sh'

def work(params):
    # """Defines the work unit on an input file"""
    problema=params[0]
    seed=params[1]
    sp.call([program,problema,seed])
    return 0

if __name__ == '__main__':
    problema = sys.argv[1].rstrip() 

    tasks = []
    for seed in range(1,16):
        tasks.append([problema,str(seed)]);

    #Set up the parallel task pool to use all available processors
    #count = mp.cpu_count()
    count = 1
    pool = mp.Pool(processes=count)

    #Run the jobs
    pool.map(work, tasks)

    print("*************** FIM ***************")
