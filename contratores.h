#ifndef _CONTRATORES_H_
#define _CONTRATORES_H_

extern int indiceFunObj;
extern double bsFunObj;

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "inter.h"
#include "constantes.h"
#include "problema.h"

void completaIndiv(t_indice*, t_indice*, int, int);

int contrator_GAC(epifita, t_indice*, t_indice*, t_indice*, int, int,
  double*, long int*, long int*, long long int*, FILE*);

int bbde_contr_estocastica(t_indice*, t_indice*, int);

void contrator_borda(t_indice*, int, t_indice*);

void contrator_bordadominio(t_indice*, int, t_indice*);

void contracao_blackbox(t_indice*, t_indice*, t_indice *, int);

#endif
