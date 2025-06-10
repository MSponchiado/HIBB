#ifndef _BBDE_OPERACOES_H_
#define _BBDE_OPERACOES_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inter.h"
#include "../constantes.h"
#include "../rand.h"
#include "bbde_uteis.h"
#include "bbde_adaptParam.h"

int bbde_boundConstrained(t_indice*, double*, int);

// DE/rand/1/bin
// tmp = r1 + F * (r2 - r3)
void bbde_strategy1bin(t_strat*, t_indice*, double*, t_indice*,
  int, int, int, int, int);

// DE/rand/1/exp
// tmp = r1 + F * (r2 - r3)
void bbde_strategy1exp(t_strat*, t_indice*, double*, t_indice*,
  int, int, int, int, int);

// DE/current-to-rand/1
// tmp = ri + rnd * (r1 - ri) + F * (r2 - r3)
void bbde_strategy2(t_strat*, t_indice*, double*, t_indice*,
  int, int, int, int, int);

// DE/current-to-pBest/1/bin
// tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
void bbde_strategy3bin(t_strat*, t_indice*, double*, t_indice*,
  int, int, int, double, int, int);

// DE/current-to-pBest/1/exp
// tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
void bbde_strategy3exp(t_strat*, t_indice*, double*, t_indice*,
  int, int, int, double, int, int);

#endif
