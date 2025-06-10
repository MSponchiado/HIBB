#ifndef _ALOCDEF_H_
#define _ALOCDEF_H_

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "inter.h"
#include "constantes.h"
#include "bbde/bbde_alocDef.h"

void box_init(t_indice*, int);

void box_free(t_indice*);

t_indice* aloca_box(int);

int* aloca_vecInt(int);

double* aloca_vecDouble(int);

t_inter* aloca_vecStruct(int);

t_inter** aloca_pop(int, int);

t_indice* aloca_ipop(int);

t_strat aloca_strat(int);

void aloca_tudo(t_inter***, t_inter***, t_inter***, t_inter***, t_inter***,
  t_indice**, t_indice**, t_indice**, t_indice**, t_indice**,
  t_strat *strat1, t_strat *strat2, t_strat *strat3, int, int, int, int);

#endif
