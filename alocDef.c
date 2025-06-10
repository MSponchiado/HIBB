#include "alocDef.h"

void box_init(t_indice *box, int tam) {
  box->cestag = 0;
  box->numdiv = 0;
  box->cost = box->viol = INFINITY;
  box->ind = (t_inter*) malloc(sizeof(t_inter) * tam);
  if (!box->ind) {
      printf("memory allocation error in box_init\n");
      exit(1);
  }
  box->ponto = (double*) malloc(sizeof(double) * tam);
  if (!box->ponto) {
      printf("memory allocation error in box_init\n");
      exit(1);
  }
  box->vprop = (double*) malloc(sizeof(double) * tam);
  if (!box->vprop) {
      printf("memory allocation error in box_init\n");
      exit(1);
  }
}

void box_free(t_indice *box) {
    free(box->ind);
    free(box->ponto);
    free(box->vprop);
}

t_indice *aloca_box(int tam) {
  t_indice *box;
  box = malloc(sizeof(t_indice));
  if (!box) {
    printf("memory allocation error in direct\n");
    exit(1);
  }
  box_init(box, tam);

  return box;
}

int *aloca_vecInt(int tam) {
  int *v;

  v = (int *) malloc (tam * sizeof(int));
  if (!v) {
    printf("Falta memoria no aloca_vecInt\n");
    exit(1);
  }
  return v;
}

double *aloca_vecDouble(int tam) {
  double *v;

  v = (double *) malloc (tam * sizeof(double));
  if (!v) {
    printf("Falta memoria no aloca_vecDouble\n");
    exit(1);
  }
  return v;
}

t_inter *aloca_vecStruct(int tam) {
  t_inter *v;

  v = (t_inter *) malloc (tam * sizeof(t_inter));
  if (!v) {
    printf("Falta memoria no aloca_vecStruct\n");
    exit(1);
  }
  return v;
}

t_inter **aloca_pop(int np, int d) {
  t_inter **m;
  int i;

  m = (t_inter **) malloc (np * sizeof(t_inter *));
  if (!m) {
    printf("Falta memoria no aloca_pop\n");
    exit(1);
  }
  for (i = 0; i < np; i++) {
    m[i] = (t_inter *) malloc (d * sizeof(t_inter));
    if (!m[i]) {
      printf("Falta memoria no aloca_pop\n");
      exit(1);
    }
  }
  return m;
}

t_indice *aloca_ipop(int np) {
  t_indice *v;

  v = (t_indice *) malloc (np * sizeof(t_indice));
  if (!v) {
    printf("Falta memoria no aloca_ipop\n");
    exit(1);
  }
  return v;
}

t_strat aloca_strat(int np) {
  t_strat v;
  v.memory_pos = 0;
  v.wins_pos = 0;
  v.nsucc_params = 0;
  v.sr = 0.33;
  v.success_sf = aloca_vecDouble(np);
  v.success_cr = aloca_vecDouble(np);
  v.pop_sf = aloca_vecDouble(np);
  v.pop_cr = aloca_vecDouble(np);
  v.dif_fitness = aloca_vecDouble(np);
  v.wins = aloca_vecDouble(25);
  return v;
}

void aloca_tudo(t_inter ***pop1, t_inter ***pop2, t_inter ***popTmp,
  t_inter ***popAux, t_inter ***popC, t_indice **ipop1, t_indice **ipop2,
  t_indice **ipopTmp, t_indice **ipopAux, t_indice **ipopC,
  t_strat *strat1, t_strat *strat2, t_strat *strat3,
  int realD, int interD, int np, int popC_maxnp) {

  *pop1 = aloca_pop(np, interD);
  *ipop1 = aloca_ipop(np);
  for (int i = 0; i < np; i++) {
    (*ipop1)[i].ind = (*pop1)[i];
    (*ipop1)[i].ponto = aloca_vecDouble(realD);
    (*ipop1)[i].vprop = aloca_vecDouble(interD);
  }

  *pop2 = aloca_pop(np, interD);
  *ipop2 = aloca_ipop(np);
  for (int i = 0; i < np; i++) {
    (*ipop2)[i].ind = (*pop2)[i];
    (*ipop2)[i].ponto = aloca_vecDouble(realD);
    (*ipop2)[i].vprop = aloca_vecDouble(interD);
  }

  *popTmp = aloca_pop(3, interD);
  *ipopTmp = aloca_ipop(3);
  for (int i = 0; i < 3; i++) {
    (*ipopTmp)[i].ind = (*popTmp)[i];
    (*ipopTmp)[i].ponto = aloca_vecDouble(realD);
    (*ipopTmp)[i].vprop = aloca_vecDouble(interD);
  }

  *popC = aloca_pop(popC_maxnp, interD);
  *ipopC = aloca_ipop(popC_maxnp);
  for (int i = 0; i < popC_maxnp; i++) {
    (*ipopC)[i].ind = (*popC)[i];
    (*ipopC)[i].ponto = aloca_vecDouble(realD);
    (*ipopC)[i].vprop = aloca_vecDouble(interD);
  }

  *popAux = aloca_pop(5, interD);
  *ipopAux = aloca_ipop(5);
  for (int i = 0; i < 5; i++) {
    (*ipopAux)[i].ind = (*popAux)[i];
    (*ipopAux)[i].ponto = aloca_vecDouble(realD);
    (*ipopAux)[i].vprop = aloca_vecDouble(interD);
  }

  *strat1 = aloca_strat(np);
  *strat2 = aloca_strat(np);
  *strat3 = aloca_strat(np);

  // para controle da adaptação de estratégias
  for (int i = 0; i < 25; i++) {
    (*strat1).wins[i] = 0.0;
    (*strat2).wins[i] = 0.0;
    (*strat3).wins[i] = 0.0;
  }

  // inicializa os vetores de memória F e CR
  for (int i = 0; i < 5; i++) {
    (*strat1).memory_sf[i] = 0.3; (*strat1).memory_cr[i] = 0.8;
    (*strat2).memory_sf[i] = 0.3; (*strat2).memory_cr[i] = 0.8;
    (*strat3).memory_sf[i] = 0.3; (*strat3).memory_cr[i] = 0.8;
  }
}
