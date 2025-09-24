#!/usr/bin/env bash

listas=$*

RUN="$(pwd)/paraleliza_experimentos.py"

DIR_JOBS="/home/mrscassenote/tmp_sbatch_jobs"
DIR_LOGS="$(pwd)/logs"
mkdir -p ${DIR_JOBS}
mkdir -p ${DIR_LOGS}

seeds=$(seq 1 25)

for s in $seeds; do

for l in $listas; do

lname=$(basename ${l})
job="$(mktemp -d ${DIR_JOBS}/job_${lname}_${s}_XXXXXX)/job"

echo \
"#!/usr/bin/env bash
#SBATCH -J HIBB${s}
#SBATCH -o ${DIR_LOGS}/${lname}_${s}.out
#SBATCH -c 3
#SBATCH -N 1
#SBATCH -t 168:00:00
#SBATCH -p nodes
${RUN} ${l} ${s}
" > ${job}

sbatch $job

done

done
