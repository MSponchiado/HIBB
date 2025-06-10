#ifndef _INTER_H_
#define _INTER_H_

#include "constantes.h"

typedef struct t_inter {
  double bi;
  double bs;
} t_inter;

typedef struct t_indice {
  t_inter *ind;
  double *ponto;
  double *vprop;
  double cost;
  double viol;
  long int cestag;
  long int numdiv;
} t_indice;

typedef struct t_dimInd {
  double perimMin;
  double perimMax;
  double perimAvg;
  double perimStd;
  double dimMin;
  double dimMax;
  double dimAvg;
  double dimStd;
} t_dimInd;

typedef struct t_strat {
  int memory_pos;
  int wins_pos;
  int nsucc_params;
  double sr;
  double memory_sf[5];
  double memory_cr[5];
  double *success_sf;
  double *success_cr;
  double *pop_sf;
  double *pop_cr;
  double *dif_fitness;
  double *wins;
} t_strat;

typedef struct t_restr {
  int idc;
  int idd;
  int ide;
} t_restr;

#endif
