#ifndef _BBDE_UTEIS_H_
#define _BBDE_UTEIS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../inter.h"
#include "../rand.h"
#include "../constantes.h"
#include "bbde_adaptParam.h"

void bbde_printVector(double*, int);

void bbde_printPop(t_indice*, int);

void bbde_printIndiv(double*, int);

void printInterv(t_inter*, int);

void bbde_verificaNan(t_indice *, int);

void bbde_corrigeInfsDom(t_indice *, int);

void bbde_sinalConflito(t_indice *, int);

int bbde_comparaDouble(const void *a, const void *b);

int bbde_comparaIndivReal(const void*, const void*);

void bbde_ordenaPop(t_indice*, int);

void bbde_controle(t_indice*, int, double*, double*, double*, double*, double*, double*);

void bbde_atualizaDados(t_indice*, double*, double*, double*, int);

void bbde_copiaIndiv(t_indice*, int, t_indice*, int, int);

#endif
