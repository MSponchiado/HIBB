#include "bbde_operacoes.h"

int blrbest, blr1, blr2, blr3;

void bbde_geraRand(int i, int np) {
  do {
    blr1 = (int)(gera_random() * (float)np * 0.5);
  } while(blr1==i);
  do {
    blr2 = (int)(gera_random() * (float)np * 0.5);
  } while((blr2==i) || (blr2==blr1));
  do {
  blr3 = (int)(gera_random() * np);
  if (blr3 == np) blr3 = np - 1;
  } while((blr3==i) || (blr3==blr1) || (blr3==blr2));
}

int bbde_boundConstrained(t_indice *caixa, double *tmp, int n) {
  int corrigido = 0;
  // garante que o novo indivíduo estará dentro dos domínios das variáveis
  if ((tmp[n] < (*caixa).ind[n].bi) || (tmp[n] > (*caixa).ind[n].bs)) {
    double ampcaixa = (*caixa).ind[n].bs - (*caixa).ind[n].bi;
    tmp[n] = (*caixa).ind[n].bi + (gera_random() * ampcaixa);
    corrigido = 1;
  }
  return corrigido;
}

// DE/rand/1/bin
// tmp = blr1 + F * (blr2 - blr3),
// onde blr1 e blr2 são sorteados dentre os melhores np/2 indivíduos
void bbde_strategy1bin(t_strat *strat, t_indice *ipold, double *tmp, t_indice *caixa,
  int i, int np, int realD, int bbde_nfeval, int bbde_maxfes) {

  int n, l;
  double x1, x2, x3;

  bbde_geraRand(i, np);
  bbde_geraParam(strat, i, bbde_nfeval, bbde_maxfes);
  memcpy(tmp, ipold[i].ponto, realD * (sizeof(double)));

  n = (int)(gera_random() * realD);

  for (l = 0; l < realD; l++) {
    if ((gera_random() < (*strat).pop_cr[i]) || (l == (realD - 1))) {
      x1 = ipold[blr1].ponto[n];
      x2 = ipold[blr2].ponto[n];
      x3 = ipold[blr3].ponto[n];

      tmp[n] = x1 + ((*strat).pop_sf[i] * (x2 - x3));

      bbde_boundConstrained(caixa, tmp, n);
    }
    n = (n + 1) % realD;
  }
}

// DE/rand/1/exp
// tmp = blr1 + F * (blr2 - blr3),
// onde blr1 e blr2 são sorteados dentre os melhores np/4 indivíduos
void bbde_strategy1exp(t_strat *strat, t_indice *ipold, double *tmp, t_indice *caixa,
  int i, int np, int realD, int bbde_nfeval, int bbde_maxfes) {

  int n, l;
  double x1, x2, x3;

  bbde_geraRand(i, np);
  bbde_geraParam(strat, i, bbde_nfeval, bbde_maxfes);
  memcpy(tmp, ipold[i].ponto, realD * (sizeof(double)));

  n = (int)(gera_random() * realD);
  l = 0;

  do {
    x1 = ipold[blr1].ponto[n];
    x2 = ipold[blr2].ponto[n];
    x3 = ipold[blr3].ponto[n];

    tmp[n] = x1 + (*strat).pop_sf[i] * (x2 - x3);

    bbde_boundConstrained(caixa, tmp, n);

    n = (n + 1) % realD;
    l++;
  } while((gera_random() < (*strat).pop_cr[i]) && (l < realD));
}

// DE/current-to-rand/1
// tmp = ri + rnd * (blr1 - ri) + F * (blr2 - blr3)
void bbde_strategy2(t_strat *strat, t_indice *ipold, double *tmp, t_indice *caixa,
  int i, int np, int realD, int bbde_nfeval, int bbde_maxfes) {

  int n, l;
  double rnd, xi, x1, x2, x3;

  bbde_geraRand(i, np);
  bbde_geraParam(strat, i, bbde_nfeval, bbde_maxfes);
  memcpy(tmp, ipold[i].ponto, realD * (sizeof(double)));

  // escolhe um ponto aleatório do vetor para começar a operação
  n = (int)(gera_random() * realD);

  for (l = 0; l < realD; l++) {
    rnd = gera_random();

    xi = ipold[i].ponto[n];
    x1 = ipold[blr1].ponto[n];
    x2 = ipold[blr2].ponto[n];
    x3 = ipold[blr3].ponto[n];

    tmp[n] = xi + rnd * (x1 - xi) + (*strat).pop_sf[i] * (x2 - x3);

    bbde_boundConstrained(caixa, tmp, n);

    n = (n + 1) % realD;
  }
}

// DE/current-to-pBest/1/bin
// tmp = ri + F * (pBest - ri) + F * (blr1 - blr2),
// onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
void bbde_strategy3bin(t_strat *strat, t_indice *ipold, double *tmp, t_indice *caixa,
  int i, int np, int realD, double taxaPbest, int bbde_nfeval, int bbde_maxfes) {

  int n, l;
  double xi, xbest, x1, x2;

  bbde_geraParam(strat, i, bbde_nfeval, bbde_maxfes);
  memcpy(tmp, ipold[i].ponto, realD * (sizeof(double)));

  int pbestNum = (int)((float)np * taxaPbest);
  if (pbestNum <= 1)
    pbestNum = 2;

  do {
    // blrbest = rand() % pbestNum;
    blrbest = (int)(gera_random() * pbestNum);
  } while(blrbest==i);
  do {
    blr1 = (int)(gera_random() * (float)np * 0.5);
  } while((blr1==i) || (blr1==blrbest));
  do {
    blr2 = (int)(gera_random() * (float)(np));
  } while((blr2==i) || (blr2==blr1) || (blr2==blrbest));

  // escolhe um ponto aleatório do vetor para começar a operação
  n = (int)(gera_random() * realD);

  for (l = 0; l < realD; l++) {
    if ((gera_random() < (*strat).pop_cr[i]) || (l == (realD - 1))) {
      xi = ipold[i].ponto[n];
      x1 = ipold[blr1].ponto[n];
      x2 = ipold[blr2].ponto[n];
      xbest = ipold[blrbest].ponto[n];

      tmp[n] = xi + (*strat).pop_sf[i] * (xbest - xi) + (*strat).pop_sf[i] * (x1 - x2);

      bbde_boundConstrained(caixa, tmp, n);
    }
    n = (n + 1) % realD;
  }
}

// DE/current-to-pBest/1/exp
// tmp = ri + F * (pBest - ri) + F * (blr1 - blr2),
// onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
void bbde_strategy3exp(t_strat *strat, t_indice *ipold, double *tmp, t_indice *caixa,
  int i, int np, int realD, double taxaPbest, int bbde_nfeval, int bbde_maxfes) {

  int n, l;
  double xi, xbest, x1, x2;

  bbde_geraParam(strat, i, bbde_nfeval, bbde_maxfes);
  memcpy(tmp, ipold[i].ponto, realD * (sizeof(double)));

  int pbestNum = (int)((float)np * taxaPbest);
  if (pbestNum <= 1)
    pbestNum = 2;

  do {
    blrbest = (int)(gera_random() * pbestNum);
  } while(blrbest==i);
  do {
    blr1 = (int)(gera_random() * (float)np * 0.5);
  } while((blr1==i) || (blr1==blrbest));
  do {
    blr2 = (int)(gera_random() * (float)(np));
  } while((blr2==i) || (blr2==blr1) || (blr2==blrbest));

  // escolhe um ponto aleatório do vetor para começar a operação
  n = (int)(gera_random() * realD);
  l = 0;

  do {
    xi = ipold[i].ponto[n];
    x1 = ipold[blr1].ponto[n];
    x2 = ipold[blr2].ponto[n];
    xbest = ipold[blrbest].ponto[n];

    tmp[n] = xi + (*strat).pop_sf[i] * (xbest - xi) + (*strat).pop_sf[i] * (x1 - x2);

    bbde_boundConstrained(caixa, tmp, n);

    n = (n + 1) % realD;
    l++;
  } while((gera_random() < (*strat).pop_cr[i]) && (l < realD));
}
