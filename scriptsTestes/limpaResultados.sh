#!/usr/bin/env bash

tudo=${1}

rm -rf bin/ logs/

if [[ $(echo "$tudo == 1" | bc -l) == 1 ]]; then
  rm -rf resultados/ compilados/
else
  for i in $(ls ./resultados/*/log/*); do
    rm -rf $i
  done

  for i in $(ls ./resultados/*/params/*); do
    rm -rf $i
  done
fi
