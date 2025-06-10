#ifndef _BBDE_ALOCDEF_H_
#define _BBDE_ALOCDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inter.h"
#include "../constantes.h"

int* bbde_aloca_vecInt(int);

double* bbde_aloca_vecDouble(int);

t_inter* bbde_aloca_vecStruct(int);

double** bbde_aloca_pop(int, int);

t_indice* bbde_aloca_ipop(int);

t_strat bbde_aloca_strat(int);

void bbde_aloca_tudo(double***, double***, double***,
  t_indice**, t_indice**, t_indice**, t_strat*, t_strat*, t_strat*, int, int);

#endif
