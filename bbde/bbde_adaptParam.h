#ifndef _BBDE_ADAPTPARAM_H_
#define _BBDE_ADAPTPARAM_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inter.h"
#include "../constantes.h"
#include "../rand.h"
#include "bbde_uteis.h"
#include "bbde_operacoes.h"

void bbde_startEpsLvl();

void bbde_inicializaEpsLvl(t_indice*, int);

void bbde_atualizaEpsLvl(int, int);

void bbde_geraEpsLvl(t_indice*, int);

double bbde_valorEpsLvl();

int bbde_epsConstCompReal(double, double, double, double);

void bbde_geraParam(t_strat*, int, int, int);

void bbde_registraMelhoria(t_indice*, t_indice*, int, int, t_strat*);

void bbde_atualizaParam(t_strat*, int);

void bbde_atualizaMelhorEstr(t_strat*, t_strat*, t_strat*, int);

#endif
