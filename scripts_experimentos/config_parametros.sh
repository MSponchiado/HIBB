#!/usr/bin/env bash

problema=${1}
seed=${2}

nome=$(basename ${problema} .txt)

nfes="2000000"    # Fator de multiplicacao do número maximo de solucoes 
                  # avaliadas (orcamento geral)
bbde_np="10"      # Fator de multiplicacao do número de solucoes exploradas 
                  # simultaneamente a cada busca local (tamanho populacao NP)
bbde_nfes="2000"  # Fator de multiplicacao do numero maximo de solucoes exploradas 
                  # a cada busca local (orcamento busca local)
printlog="0"      # Imprime log de execução na tela

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
  echo "${x}" >> ${param}
  echo "${bbde_np}" >> ${param}
  echo "${bbde_nfes}" >> ${param}
  echo "${printlog}" >> ${param}
  echo "${seed}" >> ${param}

  ( time ${DE} ${param} ${solucoes} ${log} ${gac} < ${problema} ) > ${tempo} &>> ${tempo}

done

echo "FINAL; ${nome}; $(date)"
