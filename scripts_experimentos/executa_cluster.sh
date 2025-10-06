#!/usr/bin/env bash

DIR_BIN="./bin"
DIR_BASE="./../.."
DIR_EPIF="${DIR_BASE}/MINLPLib/epifitas"
DIR_AMPL="${DIR_BASE}/MINLPLib/funcoes"
LNK_SIMB="../instancia.c"
LST_INST="./instancias.txt"

################### COMPILA BINÁRIOS ###################

if [ -d ${DIR_BIN} ]; then
  rm -rf ${DIR_BIN}
fi
mkdir -p ${DIR_BIN}

# echo ${LNK_SIMB}

for i in $(ls ${DIR_AMPL}/*.c); do
  nome=$(basename ${i} .c)
  bin="${DIR_BIN}/executa_${nome}"
  echo "

  *************** COMPILANDO: ${nome} ***************

  "
  rm ${LNK_SIMB}; cp ${i} ${LNK_SIMB}
  make -C .. executa && mv ../executa ${bin}
done

################### LISTA INSTÂNCIAS ###################

if [[ -f ${LST_INST} ]]; then
  rm -rf ${LST_INST}
fi

for f in $(find ${DIR_EPIF} -iname "*.txt"); do
  echo $(pwd)/$f >> ${LST_INST}
done

##################### DISPARA JOBS #####################

./config_jobs.sh ${LST_INST}
