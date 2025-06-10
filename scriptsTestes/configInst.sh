#!/usr/bin/env bash

problema=${1}
seed=${2}

nome=$(basename ${problema} .txt)

nfes="10000000"
#nfes=$(grep "$nome;" ../../../minlplib/ate100dim-orcdim.txt | cut -d ";" -f2)
bbde_np="10"
bbde_nfes="10000"
printlog="0"

DIR=$(dirname ${0})
DE="${DIR}/bin/inde-${nome}"

DIR_RESULTADOS="${DIR}/resultados/${nome}"
DIR_PARAMS="${DIR_RESULTADOS}/params"
DIR_FITNESS="${DIR_RESULTADOS}/fitness"
DIR_TEMPO="${DIR_RESULTADOS}/tempo"
DIR_LOG="${DIR_RESULTADOS}/log"
DIR_GAC="${DIR_RESULTADOS}/gac"

mkdir -p ${DIR_PARAMS}
mkdir -p ${DIR_FITNESS}
mkdir -p ${DIR_TEMPO}
mkdir -p ${DIR_LOG}
mkdir -p ${DIR_GAC}

echo "INICIO; ${nome}; $(date)"

for x in ${nfes}; do
  param="${DIR_PARAMS}/${nome}_nfes${x}_s${seed}.par"
  fitness="${DIR_FITNESS}/${nome}_nfes${x}_s${seed}.fit"
  tempo="${DIR_TEMPO}/${nome}_nfes${x}_s${seed}.tempo"
  log="${DIR_LOG}/${nome}_nfes${x}_s${seed}.log"
  gac="${DIR_GAC}/${nome}_nfes${x}_s${seed}.gac"
  echo -n "" > ${param}
  echo "${x}" >> ${param}
  echo "${bbde_np}" >> ${param}
  echo "${bbde_nfes}" >> ${param}
  echo "${printlog}" >> ${param}
  echo "${seed}" >> ${param}

  ( time ${DE} ${param} ${fitness} ${log} ${gac} < ${problema} ) > ${tempo} &>> ${tempo}

done

echo "FINAL; ${nome}; $(date)"
