#ifndef _OPERACOES_H_
#define _OPERACOES_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inter.h"
#include "rand.h"
#include "uteis.h"
#include "constantes.h"
#include "adaptParam.h"
#include "alocDef.h"

// void geraRand(int, int);
//
// void alocaIndAux(t_inter*, t_inter*, int);
//
// void boundConstrained(t_indice*, t_inter*, t_inter*, int, int);
//
// void operacao1(t_strat*,  int);
//
// void operacao2(t_strat*,  int);
//
// void operacao3(t_strat*,  int);
//
// void completaIndiv(t_inter*, t_inter*, int, int);
//
// // DE/rand/1/bin
// // tmp = r1 + F * (r2 - r3)
// void strategy1bin(t_strat*, t_indice*, t_inter*, t_inter*, int, int, int, int,
//   int, long);
//
// // DE/rand/1/exp
// // tmp = r1 + F * (r2 - r3)
// void strategy1exp(t_strat*, t_indice*, t_inter*, t_inter *, int, int, int, int,
//   int, long);
//
// // DE/current-to-rand/1
// // tmp = ri + rnd * (r1 - ri) + F * (r2 - r3)
// void strategy2(t_strat*, t_indice*, t_inter*, t_inter *, int, int, int, int,
//   int, long);
//
// // DE/current-to-pBest/1/bin
// // tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// // onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
// void strategy3bin(t_strat*, t_indice*, t_inter*, t_inter *, int, int, int, int,
//   int, long);
//
// // DE/current-to-pBest/1/exp
// // tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// // onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
// void strategy3exp(t_strat*, t_indice*, t_inter*, t_inter *, int, int, int, int,
//   int, long);

#endif
