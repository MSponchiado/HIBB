#ifndef _ADAPTPARAM_H_
#define _ADAPTPARAM_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inter.h"
#include "rand.h"
#include "uteis.h"
#include "constantes.h"

void startEpsLvl();

void inicializaEpsLvl(t_indice*);

void atualizaEpsLvl(long long int*, int);

double valorEpsLvl();

int epsConstCompReal(double, double, double, double);

void geraParam(t_strat *, int, int, long);

void registraMelhoria(t_indice *, t_indice *, int, int, t_strat *);

void atualizaParam(t_strat *, int);

void atualizaMelhorEstr(t_strat *, t_strat *, t_strat *, int);

#endif
