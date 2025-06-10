#include "bbde_problema.h"
#include "../instancia.h"

void bbde_inicia_pop(t_indice *ipop, int np, t_indice *caixa, int realD,
  double *violponto, double *violprop) {

  // verifica se o melhor ponto já encontrado está dentro da caixa
  int foraprop = 0, foraponto = 0, indpop = 0;

  for (int i = 0; i < realD; i++) {
    if (((*caixa).ponto[i] < (*caixa).ind[i].bi) ||
        ((*caixa).ponto[i] > (*caixa).ind[i].bs)) {
        foraponto = 1;
        *violponto = INFINITODE;
    }

    if ((APLICA_CGAC == 0) || ((*caixa).vprop[i] < (*caixa).ind[i].bi) ||
        ((*caixa).vprop[i] > (*caixa).ind[i].bs)) {
        foraprop = 1;
        *violprop = INFINITODE;
    }
  }

  // coloca valores promissores na população inicial
  if (!foraponto) {   // melhor ponto encontrado
    for (int i = 0; i < realD; i++)
      ipop[indpop].ponto[i] = (*caixa).ponto[i];
    indpop++;
  }
  if (!foraprop) {    // valor de propagação
    for (int i = 0; i < realD; i++)
      ipop[indpop].ponto[i] = (*caixa).vprop[i];
    indpop++;
  }

  // cria o restante da população do jeito aleatório clássico
  double amp;
  for (int i = indpop; i < np; i++) {
    for (int j = 0; j < realD; j++) {
      amp = (*caixa).ind[j].bs - (*caixa).ind[j].bi;
      ipop[i].ponto[j] = (*caixa).ind[j].bi + (amp * gera_random());
    }
  }
}

void avaliacao_real(t_indice *ipop, int i, int ncons) {
  double vec_viol[ncons];

  problem_eval(ipop[i].ponto, &ipop[i].cost, vec_viol);

  ipop[i].viol = 0.0;
  for (int j = 0; j < ncons; j++)
    ipop[i].viol += vec_viol[j];

  // // if ((isnan(ipop[i].cost)) || (isnan(ipop[i].viol)) ||
  // //     (isinf(ipop[i].cost)) || (isinf(ipop[i].viol))) {
  // //     ipop[i].cost = INFINITODE;
  // //     ipop[i].viol = INFINITODE;
  // // }

}

void bbde_evaluate(t_indice *ipop, int i, t_indice *caixa,
  t_indice *contrEstoc, int realD, int ncons, int tcFes, int gen, long *bbde_nfeval) {

  (*bbde_nfeval)++;

  avaliacao_real(ipop, i, ncons);

  if (gen > 0)
    bbde_atualizaEpsLvl(*bbde_nfeval, tcFes);

  if (gen > 1) {
    // verifica se é epsilon-factivel
    if (ipop[i].viol <= bbde_valorEpsLvl()) {

      double perimAntes = 0.0;
      for (int k = 0; k < realD; k++)
        perimAntes += (*contrEstoc).ind[k].bs - (*contrEstoc).ind[k].bi;

      // verifica se esse ponto está contido na caixa contrEstoc atual
      // se não estiver, aumenta a caixa
      double auxbi, auxbs, auxborda;
      for (int k = 0; k < realD; k++) {
        auxborda = ((*caixa).ind[k].bs - (*caixa).ind[k].bi) * FATOR_PARTIC;

        auxbi = ipop[i].ponto[k] - auxborda;
        if (auxbi < (*caixa).ind[k].bi)
          auxbi = (*caixa).ind[k].bi;

        auxbs = ipop[i].ponto[k] + auxborda;
        if (auxbs > (*caixa).ind[k].bs)
          auxbs = (*caixa).ind[k].bs;

        if (auxbi < (*contrEstoc).ind[k].bi)
          (*contrEstoc).ind[k].bi = auxbi;

        if (auxbs > (*contrEstoc).ind[k].bs)
          (*contrEstoc).ind[k].bs = auxbs;
      }

      double perimDepois = 0.0;
      for (int k = 0; k < realD; k++)
        perimDepois += (*contrEstoc).ind[k].bs - (*contrEstoc).ind[k].bi;

      if (perimAntes > perimDepois) {
        printf("consistencia estocastica diminuiu! antes: %g  depois: %g\n",
        perimAntes, perimDepois);
        exit(1);
      }
    }
  }
}
