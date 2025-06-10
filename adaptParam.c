#include "adaptParam.h"

double epsLvl, epsLvlInicial, aux;

void startEpsLvl() {
  epsLvl = 0.0;
}

void inicializaEpsLvl(t_indice *caixa) {
  epsLvlInicial = epsLvl = (*caixa).viol;
  if ((APLICA_EPLVLMIN) && (epsLvlInicial < EPSLVLMIN))
    epsLvl = epsLvlInicial = EPSLVLMIN;
}

void atualizaEpsLvl(long long int *nfeval, int tcFes) {
  epsLvl = (*nfeval) > tcFes ? 0.0 :
    epsLvlInicial * pow(1.0 - (double)(*nfeval) / tcFes, 4.0);
}

double valorEpsLvl() {
  return epsLvl;
}

int epsConstCompReal(double rCostA, double rEpsA, double rCostB, double rEpsB) {
  /* --------------- Se A é melhor retorna 1 --------------- */
  if ((rEpsA <= epsLvl) && (rEpsB <= epsLvl))
    return rCostA < rCostB;
  else if (rEpsA == rEpsB)
    return rCostA < rCostB;
  else if (rEpsA < rEpsB)
    return 1;
  else
    return 0;
}

// A cada geração, cr_i e f_i usados por cada indivíduo x_i
// são gerados selecionando aleatoriamente um índice r_i
// de [1, H], em que H é MEMORY_SIZE
void geraParam(t_strat *s, int i, int nfeval, long maxfes) {
  double mu_sf, mu_cr;

  // A cada geração, CR_i e F_i usados por cada indivíduo x_i
  // são gerados selecionando aleatoriamente um índice r_i
  // de [1, H], em que H é memory_size
  int rnd = gera_random() * 5;
  if (rnd == 5 - 1) { // jSO
    mu_sf = 0.9;
    mu_cr = 0.9;
  }
  else {
    mu_sf = (*s).memory_sf[rnd];
    mu_cr = (*s).memory_cr[rnd];
  }

  // jSO - CR
  if (mu_cr < 0) {           // JANEZ
      (*s).pop_cr[i] = 0.0;  // LSHADE 0
  } else {
      (*s).pop_cr[i] = gauss(mu_cr, 0.1);
      if ((*s).pop_cr[i] > 1)
          (*s).pop_cr[i] = 1;
      else if ((*s).pop_cr[i] < 0)
          (*s).pop_cr[i] = 0;
  }
  if ((nfeval < (0.25 * maxfes)) && ((*s).pop_cr[i] < 0.7))
      (*s).pop_cr[i] = 0.7;  // jSO
  if ((nfeval < (0.50 * maxfes)) && ((*s).pop_cr[i] < 0.6))
      (*s).pop_cr[i] = 0.6;  // jSO

  // jSO - F
  do {
    (*s).pop_sf[i] = cauchy_g(mu_sf, 0.1);
  } while ((*s).pop_sf[i] <= 0.0);

  if ((*s).pop_sf[i] > 1)
    (*s).pop_sf[i] = 1.0;
  if ((nfeval < (0.6 * maxfes)) && ((*s).pop_sf[i] > 0.7))
    (*s).pop_sf[i] = 0.7;  // jSO

}

void registraMelhoria(t_indice *ipold, t_indice *ipnew, int i, int realD,
  t_strat *s) {

  if (ipold[i].viol < ipnew[i].viol)
    (*s).dif_fitness[(*s).nsucc_params] = TOLERANCIA;
  else {
    double tmp = 0.0;
    for (int j = 0; j < realD; j++)
      tmp += ((ipnew[i].ponto[j] - ipold[i].ponto[j]) *
        (ipnew[i].ponto[j] - ipold[i].ponto[j]));
    (*s).dif_fitness[(*s).nsucc_params] = sqrt(tmp);
  }

  (*s).success_sf[(*s).nsucc_params] = (*s).pop_sf[i];
  (*s).success_cr[(*s).nsucc_params] = (*s).pop_cr[i];
  (*s).nsucc_params++;

  (*s).wins[(*s).wins_pos] = (double)(*s).nsucc_params;
  (*s).wins_pos++;
  if((*s).wins_pos >= 25)
    (*s).wins_pos = 0;
}

void atualizaParam(t_strat *s, int np) {
  double temp_sum_sf, temp_sum_cr, sum;

  double old_sf = (*s).memory_sf[(*s).memory_pos];
  double old_cr = (*s).memory_cr[(*s).memory_pos];

  if ((*s).nsucc_params <= 0) return;

  (*s).memory_sf[(*s).memory_pos] = 0.0;
  (*s).memory_cr[(*s).memory_pos] = 0.0;
  sum = temp_sum_sf = temp_sum_cr = 0.0;

  // contabiliza diferenças de fitness dos operadores de sucesso
  for (int i = 0; i < (*s).nsucc_params; i++)
    sum += (*s).dif_fitness[i];
  if(sum == 0.0) sum = TOLERANCIA;

  // média ponderada de Lehmer
  for (int i = 0; i < (*s).nsucc_params; i++) {
    double weight = (*s).dif_fitness[i] / sum;
    if(weight == 0) weight = TOLERANCIA;
    (*s).memory_sf[(*s).memory_pos] +=
      weight * (*s).success_sf[i] * (*s).success_sf[i];
    temp_sum_sf += weight * (*s).success_sf[i];
    (*s).memory_cr[(*s).memory_pos] +=
      weight * (*s).success_cr[i] * (*s).success_cr[i];
    temp_sum_cr += weight * (*s).success_cr[i];
  }

  if(temp_sum_sf == 0.0) temp_sum_sf = TOLERANCIA;
  if(temp_sum_cr == 0.0) temp_sum_cr = TOLERANCIA;

  (*s).memory_sf[(*s).memory_pos] /= temp_sum_sf;

  if (temp_sum_cr == 0.0 || (*s).memory_cr[(*s).memory_pos] == -1)
    (*s).memory_cr[(*s).memory_pos] = -1;
  else
    (*s).memory_cr[(*s).memory_pos] /= temp_sum_cr;

  (*s).memory_sf[(*s).memory_pos] = ((*s).memory_sf[(*s).memory_pos] + old_sf) / 2.0;
  (*s).memory_cr[(*s).memory_pos] = ((*s).memory_cr[(*s).memory_pos] + old_cr) / 2.0;

  (*s).memory_pos++;
  if ((*s).memory_pos >= 5)
    (*s).memory_pos = 0;

  // limpa S_F, S_CR e variação de fitness
  for (int i = 0; i < np; i++) {
    (*s).success_sf[i] = 0.0;
    (*s).success_cr[i] = 0.0;
    (*s).dif_fitness[i] = 0.0;
  }
}

void atualizaMelhorEstr(t_strat *s1, t_strat *s2, t_strat *s3,
  int gen) {

  if (gen < 100)
      (*s1).sr = (*s2).sr = (*s3).sr = 0.33;
  else {
    double nw1 = 0, nw2 = 0, nw3 = 0;
    for (int g = 0; g < 25 - 1; g++) {
      nw1 = nw1 + (*s1).wins[g];
      nw2 = nw2 + (*s2).wins[g];
      nw3 = nw3 + (*s3).wins[g];
    }
    (*s1).sr = nw1 / (nw1 + nw2 + nw3 + TOLERANCIA);
    (*s2).sr = nw2 / (nw1 + nw2 + nw3 + TOLERANCIA);
    (*s3).sr = nw3 / (nw1 + nw2 + nw3 + TOLERANCIA);
  }
}
