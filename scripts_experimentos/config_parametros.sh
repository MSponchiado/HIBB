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
DIR_SOLUCOES="${DIR_RESULTADOS}/solucoes"
DIR_TEMPO="${DIR_RESULTADOS}/tempo"
DIR_LOG="${DIR_RESULTADOS}/log"
DIR_GAC="${DIR_RESULTADOS}/gac"

mkdir -p ${DIR_PARAMS}
mkdir -p ${DIR_SOLUCOES}
mkdir -p ${DIR_TEMPO}
mkdir -p ${DIR_LOG}
mkdir -p ${DIR_GAC}

echo "INICIO; ${nome}; $(date)"

for x in ${nfes}; do
  param="${DIR_PARAMS}/${nome}_nfes${x}_s${seed}.par"
  solucoes="${DIR_SOLUCOES}/${nome}_nfes${x}_s${seed}.sol"
  tempo="${DIR_TEMPO}/${nome}_nfes${x}_s${seed}.tempo"
  log="${DIR_LOG}/${nome}_nfes${x}_s${seed}.log"
  gac="${DIR_GAC}/${nome}_nfes${x}_s${seed}.gac"
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

  ( time ${DE} ${param} ${solucoes} ${gac} < ${problema} ) > ${tempo} &>> ${tempo}

done

echo "FINAL; ${nome}; $(date)"

