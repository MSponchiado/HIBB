#ifndef _BBDE_BUSCALOCAL_H_
#define _BBDE_BUSCALOCAL_H_

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fenv.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "../inter.h"
#include "../rand.h"
#include "../constantes.h"
#include "../contratores.h"
#include "bbde_adaptParam.h"
#include "bbde_problema.h"
#include "bbde_operacoes.h"
#include "bbde_uteis.h"
#include "bbde_alocDef.h"
#include "bbde_globais.h"

void bbde_init(int, int, int);

void bbde_principal(t_indice*, int, long, t_indice*, long*);

#endif
