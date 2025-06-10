#ifndef _BBDE_PROBLEMA_H_
#define _BBDE_PROBLEMA_H_

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "../inter.h"
#include "../constantes.h"
#include "../rand.h"
#include "bbde_buscalocal.h"
#include "bbde_uteis.h"

void bbde_inicia_pop(t_indice*, int, t_indice*, int, double*, double*);

void avaliacao_real(t_indice *, int, int);

void bbde_evaluate(t_indice*, int, t_indice*, t_indice*, int, int, int, int, long*);

#endif
