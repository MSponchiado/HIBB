#!/usr/bin/env bash

listas=$*

RUN="$(pwd)/runPython.py"

DIR_JOBS="/home/mrscassenote/tmp_sbatch_jobs"
DIR_LOGS="$(pwd)/logs"
mkdir -p ${DIR_JOBS}
mkdir -p ${DIR_LOGS}

for l in $(cat ${listas}); do

lname=$(basename ${l} .txt)
job="$(mktemp -d ${DIR_JOBS}/job_${lname}_${s}_XXXXXX)/job"

echo \
"#!/usr/bin/env bash
#SBATCH -J sa04h-${lname}
#SBATCH -o ${DIR_LOGS}/inde-${lname}.out
#SBATCH --mem=7G
#SBATCH -c 1
#SBATCH -N 1
#SBATCH -t 168:00:00
#SBATCH -p nodes
${RUN} ${l}
" > ${job}

sbatch $job

done

