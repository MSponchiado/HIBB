#!/usr/bin/python3

#Process level parallelism for shell commands
import sys
import subprocess as sp
import multiprocessing as mp
from os.path import dirname as dirname

global program
program = dirname(sys.argv[0])+'/config_parametros.sh'

def work(params):
    # """Defines the work unit on an input file"""
    problema=params[0]
    seed=params[1]
    sp.call([program,problema,seed])
    return 0

if __name__ == '__main__':
    #Specify files to be worked with typical shell syntax and glob module
    problemas = []
    with open(sys.argv[1], 'r') as filehandle:
        problemas = [t.rstrip() for t in filehandle.readlines()]

    seed = sys.argv[2]

    tasks = []
    for p in problemas:
        tasks.append([p,str(seed)]);

    #Set up the parallel task pool to use all available processors
    count = mp.cpu_count()
    pool = mp.Pool(processes=count)

    #Run the jobs
    pool.map(work, tasks)

    print("*************** FIM ***************")
