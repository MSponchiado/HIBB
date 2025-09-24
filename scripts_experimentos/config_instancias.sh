#!/usr/bin/env bash

problema=${1}
seed=${2}

nome=$(basename ${problema} .txt)

np="5"
nfes="100"
bbde_np="10"
bbde_nfes="6000"
pbest="0.25"
npbest="0.5"
tc="0.85"
printlog="0"

DIR=$(dirname ${0})
DE="${DIR}/bin/executa_${nome}"

DIR_RESULTADOS="${DIR}/resultados/${nome}"
DIR_PARAMS="${DIR_RESULTADOS}/params"
DIR_FITNESS="${DIR_RESULTADOS}/fitness"
DIR_TEMPO="${DIR_RESULTADOS}/tempo"
DIR_LOG="${DIR_RESULTADOS}/log"

mkdir -p ${DIR_PARAMS}
mkdir -p ${DIR_FITNESS}
mkdir -p ${DIR_TEMPO}
mkdir -p ${DIR_LOG}

echo "INICIO; ${nome}; $(date)"

for x in ${nfes}; do
  param="${DIR_PARAMS}/${nome}_nfes${x}_s${seed}.par"
  fitness="${DIR_FITNESS}/${nome}_nfes${x}_s${seed}.fit"
  tempo="${DIR_TEMPO}/${nome}_nfes${x}_s${seed}.tempo"
  log="${DIR_LOG}/${nome}_nfes${x}_s${seed}.log"
  echo -n "" > ${param}
  echo "${np}" >> ${param}
  echo "${x}" >> ${param}
  echo "${bbde_np}" >> ${param}
  echo "${bbde_nfes}" >> ${param}
  echo "${pbest}" >> ${param}
  echo "${npbest}" >> ${param}
  echo "${tc}" >> ${param}
  echo "${printlog}" >> ${param}
  echo "${seed}" >> ${param}

  ( time ${DE} ${param} ${fitness} ${log} < ${problema} ) > ${tempo} &>> ${tempo}

done

echo "FINAL; ${nome}; $(date)"

