#ifndef _PROBLEMA_H_
#define _PROBLEMA_H_

extern int indiceFunObj;
extern double bsFunObj;

#include <stdio.h>
#include <float.h>
#include <math.h>
#include "inter.h"
#include "constantes.h"
#include "rand.h"
#include "uteis.h"
#include "main.h"
#include "OGRe/ogre-de.h"
#include "OGRe/real.h"
#include "bbde/bbde_buscalocal.h"

void arquiv_init(int, FILE*, FILE*, FILE*, FILE*);

void params_init(FILE*, long int*, int*, long int*, int*, int*);

void epifita_dominio_init(epifita, t_indice*);

void tamanho_instancia(int*, int*);

int geraCaixasIniciais(epifita, t_indice**, int, t_indice*, t_indice*, int, int,
  double*, long int*, long int*, long long int*, FILE*);

int evaluate(t_indice*, long, int, t_indice*, t_indice*, t_indice*,
  int, int, long long int*, long long int*);

#endif
