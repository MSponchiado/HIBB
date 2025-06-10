#include "bbde_uteis.h"

void bbde_printVector(double *vec, int size) {
  for (int i = 0; i < size; i++)
    printf("%d %f\n", i, vec[i]);
}

void bbde_printPop(t_indice *ipop, int np) {
  for (int i = 0; i < np; i++)
    printf("%d rc = %-15.10g re = %-15.10g\n", i, ipop[i].cost, ipop[i].viol);
  printf("-------------------------------------------------------------\n");
}

void bbde_printIndiv(double *indiv, int realD) {
  for (int i = 0; i < realD; i++)
    printf("%2d: %.7g\n", i, indiv[i]);
  printf("\n------------------------------------------\n");
}

void printInterv(t_inter *indiv, int realD) {
  for (int i = 0; i < realD; i++)
    printf("%2d [%.7g, %.7g]\n", i, indiv[i].bi, indiv[i].bs);
  printf("\n------------------------------------------\n");
}

void bbde_verificaNan(t_indice *caixa, int d) {
  for (int i = 0; i < d; i++) {
    if (isnan((*caixa).ind[i].bi) || isnan((*caixa).ind[i].bs)) {
      for (int j = 0; j < d; j++)
        printf("%d: [%f, %f]\n", j, (*caixa).ind[j].bi, (*caixa).ind[j].bs);
      exit(1);
    }
  }
}

void bbde_corrigeInfsDom(t_indice *caixa, int d) {
  int sinal;
  for (int i = 0; i < d; i++) {
    if ((sinal = isinf((*caixa).ind[i].bi))) {
      if (sinal > 0) (*caixa).ind[i].bi = INFINITODE;
      else (*caixa).ind[i].bi = -INFINITODE;
    }
    if ((sinal = isinf((*caixa).ind[i].bs))) {
      if (sinal > 0) (*caixa).ind[i].bs = INFINITODE;
      else (*caixa).ind[i].bs = -INFINITODE;
    }
  }
}

void bbde_sinalConflito(t_indice *ipop, int i) {
  ipop[i].cost = INFINITY;
  ipop[i].viol = INFINITY;
}

int bbde_comparaDouble(const void *a, const void *b) {
  if (*((double *)a) < *((double *)b))
    return -1;
  return 1;
}

int bbde_comparaIndivReal(const void *a, const void *b) {
  t_indice *ia, *ib;
  ia = (t_indice*) a;
  ib = (t_indice*) b;
  if (bbde_epsConstCompReal(ia->cost, ia->viol, ib->cost, ib->viol))
    return -1;
  return 1;
}

void bbde_ordenaPop(t_indice *ipop, int np) {
  qsort(ipop, np, sizeof(t_indice), bbde_comparaIndivReal);
}

void bbde_controle(t_indice *ipop, int np, double *cmean, double *cvar,
  double *emean, double *evar, double *cstd, double *estd) {

  int i;
  double cstdaux = 0.0, estdaux = 0.0;
  (*cmean) = (*cvar) = (*emean) = (*evar) = (*cstd) = (*estd) = 0.0;

  // média
  for (i = 0; i < np; i++)
    (*cmean) += ipop[i].cost;
  (*cmean) = (*cmean) / np;
  for (i = 0; i < np; i++)
    (*emean) += ipop[i].viol;
  (*emean) = (*emean) / np;

  // variância
  for (i = 0; i < np; i++)
    (*cvar) += (ipop[i].cost - (*cmean)) * (ipop[i].cost - (*cmean));
  (*cvar) = (*cvar) / (np - 1);
  for (i = 0; i < np; i++)
    (*evar) += (ipop[i].viol - (*emean)) * (ipop[i].viol - (*emean));
   (*evar) = (*evar) / (np - 1);

  // desvio padrão
  for (i = 0; i < np; i++)
    cstdaux += pow(ipop[i].cost - (*cmean), 2);
  (*cstd) = sqrt(cstdaux / np);
  for (i = 0; i < np; i++)
    estdaux += pow(ipop[i].viol - (*emean), 2);
  (*estd) = sqrt(estdaux / np);
}

void bbde_atualizaDados(t_indice *ipop, double *cmin, double *emin,
  double *meanviol, int ncons) {
  (*cmin) = ipop[0].cost;
  (*emin) = ipop[0].viol;
  (*meanviol) = (*emin) / ncons;
  if (isnan(*meanviol))
    (*meanviol) = 0.0;
}

void bbde_copiaIndiv(t_indice *ipop1, int i1, t_indice *ipop2, int i2, int realD) {
  ipop1[i1].cost = ipop2[i2].cost;
  ipop1[i1].viol = ipop2[i2].viol;
  memcpy(ipop1[i1].ponto, ipop2[i2].ponto, realD * (sizeof(double)));
}
