#include "bbde_alocDef.h"
#include "bbde_globais.h"

int *bbde_aloca_vecInt(int tam) {
  int *v;

  v = (int *) malloc (tam * sizeof(int));
  if (!v) {
    printf("Falta memoria no aloca_vecInt\n");
    exit(1);
  }
  return v;
}

double *bbde_aloca_vecDouble(int tam) {
  double *v;

  v = (double *) malloc (tam * sizeof(double));
  if (!v) {
    printf("Falta memoria no aloca_vecDouble\n");
    exit(1);
  }
  return v;
}

t_inter *bbde_aloca_vecStruct(int tam) {
  t_inter *v;

  v = (t_inter *) malloc (tam * sizeof(t_inter));
  if (!v) {
    printf("Falta memoria no aloca_vecStruct\n");
    exit(1);
  }
  return v;
}

double** bbde_aloca_pop(int bbde_np, int realD) {
  double **m, *p;
  int i;

  m = (double **) malloc((bbde_np * sizeof(double *)) + (bbde_np * realD * sizeof(double)));
  if (!m) {
    printf("Falta memoria no aloca_pop\n");
    exit(1);
  }
  p = (double *) &m[bbde_np];
  for (i = 0; i < bbde_np; i++)
    m[i] = &p[realD * i];

  return m;
}

t_indice *bbde_aloca_ipop(int bbde_np) {
  t_indice *v;

  v = (t_indice *) malloc (bbde_np * sizeof(t_indice));
  if (!v) {
    printf("Falta memoria no aloca_ipop\n");
    exit(1);
  }
  return v;
}

t_strat bbde_aloca_strat(int bbde_np) {
  t_strat v;
  v.memory_pos = 0;
  v.wins_pos = 0;
  v.nsucc_params = 0;
  v.sr = 0.33;
  v.success_sf = bbde_aloca_vecDouble(bbde_np);
  v.success_cr = bbde_aloca_vecDouble(bbde_np);
  v.pop_sf = bbde_aloca_vecDouble(bbde_np);
  v.pop_cr = bbde_aloca_vecDouble(bbde_np);
  v.dif_fitness = bbde_aloca_vecDouble(bbde_np);
  v.wins = bbde_aloca_vecDouble(25);
  return v;
}

void bbde_aloca_tudo(double ***pop1, double ***pop2, double ***popTmp,
  t_indice **ipop1, t_indice **ipop2, t_indice **ipopTmp,
  t_strat *strat1, t_strat *strat2, t_strat *strat3, int realD, int bbde_np) {

  *pop1 = bbde_aloca_pop(bbde_np, realD);
  *ipop1 = bbde_aloca_ipop(bbde_np);
  for (int i = 0; i < bbde_np; i++)
    (*ipop1)[i].ponto = (*pop1)[i];

  *pop2 = bbde_aloca_pop(bbde_np, realD);
  *ipop2 = bbde_aloca_ipop(bbde_np);
  for (int i = 0; i < bbde_np; i++)
    (*ipop2)[i].ponto = (*pop2)[i];

  *popTmp = bbde_aloca_pop(3, realD);
  *ipopTmp = bbde_aloca_ipop(3);
  for (int i = 0; i < 3; i++)
    (*ipopTmp)[i].ponto = (*popTmp)[i];

  *strat1 = bbde_aloca_strat(bbde_np);
  *strat2 = bbde_aloca_strat(bbde_np);
  *strat3 = bbde_aloca_strat(bbde_np);

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
