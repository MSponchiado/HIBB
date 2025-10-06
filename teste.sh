#/usr/bin/env bash

nomeinst=${1}
DIR_FUNCOES="../MINLPLib/funcoes"
DIR_EPIFITAS="../MINLPLib/epifitas"

if [ -e instancia.c ]; then
  rm instancia.c
fi

if [ ! -e ${DIR_FUNCOES}/${nomeinst}.c ]; then
  echo "A funcao $nomeinst.c nao existe"
  exit
fi

if [ ! -e ${DIR_EPIFITAS}/${nomeinst}.txt ]; then
  echo "A decomposicao Epifita $nomeinst.txt nao existe"
  exit
fi

if [ ! -e regras.mk ]; then
  for i in OGRe/*.c bbde/*.c *.c; do cc -MM $i; done > regras.mk
fi

ln -s ${DIR_FUNCOES}/${nomeinst}.c instancia.c

make clean && make all && ./executa fpar.dat fres.out flog.out fgac.out < ${DIR_EPIFITAS}/${nomeinst}.txt
