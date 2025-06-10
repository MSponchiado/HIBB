#!/usr/bin/env bash

DIR_BIN="./bin"
DIR_BASE="./../../.."
ARQ_INSTANCIAS="${DIR_BASE}/minlplib/franceses.txt"
DIR_EPIF="${DIR_BASE}/minlplib-fr/epifitas"
DIR_AMPL="${DIR_BASE}/minlplib-fr/funcoes"
LNK_SIMB="../instancia.c"
LST_INST="./instancias.txt"

if [[ -f ${LST_INST} ]]; then
  rm -rf ${LST_INST}
fi

################### COMPILA BINÁRIOS ###################

if [ -d ${DIR_BIN} ]; then
  rm -rf ${DIR_BIN}
fi
mkdir -p ${DIR_BIN}

# echo ${LNK_SIMB}

for i in $(cat ${ARQ_INSTANCIAS}); do
  nome=$(basename ${i})
  bin="${DIR_BIN}/inde-${nome}"
  echo "

  *************** COMPILANDO: ${nome} ***************

  "
  rm ${LNK_SIMB}; cp ${DIR_AMPL}/${i}.c ${LNK_SIMB}
  make -C .. inde && mv ../inde ${bin}

  for f in $(find ${DIR_EPIF} -iname "${nome}.txt"); do
    echo $(pwd)/$f >> ${LST_INST}
  done

done

##################### DISPARA JOBS #####################

./configJobs.sh ${LST_INST}
