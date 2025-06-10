#include "bbde_buscalocal.h"
#include "bbde_globais.h"
#include "bbde_alocDef.h"

double **bbde_pop1, **bbde_pop2, **bbde_popTmp;
t_indice *bbde_ipop1, *bbde_ipop2, *bbde_ipopTmp;
t_strat bbde_strat1, bbde_strat2, bbde_strat3;
t_indice *bbde_ipold, *bbde_ipnew, *bbde_ipswap;
int bbde_realD, bbde_ncons;

void bbde_init(int realD, int ncons, int bbde_np) {
  bbde_realD = realD;
  bbde_ncons = ncons;

  bbde_aloca_tudo(&bbde_pop1, &bbde_pop2, &bbde_popTmp,
    &bbde_ipop1, &bbde_ipop2, &bbde_ipopTmp,
    &bbde_strat1, &bbde_strat2, &bbde_strat3, bbde_realD, bbde_np);
}

/*----------Busca local----------------------------------------*/
void bbde_principal(t_indice *caixa, int bbde_np, long bbde_maxfes,
  t_indice *contrEstoc, long *bbde_nfeval) {

  *bbde_nfeval = 0;

  double cmin, emin, cmean, emean, cvar, evar, cstd, estd;
  double violponto = 0.0, violprop = 0.0;
  double taxaPbest = 0.25, tc = 0.85, meanviol;
  int succ, ult_succ = 0, npinit = bbde_np, gen = 0, indpop;
  int injetouBestpoint = 0, injetouVProp = 0;
  int tcFes = (int)(bbde_maxfes * tc);

  for (int i = 0; i < bbde_realD; i++) {
    (*contrEstoc).ind[i].bi = INFINITODE;
    (*contrEstoc).ind[i].bs = -INFINITODE;
  }

  /*----------Inicializa população-------------------------------*/
  bbde_startEpsLvl();
  bbde_corrigeInfsDom(caixa, bbde_realD);

  bbde_inicia_pop(bbde_ipop1, bbde_np, caixa, bbde_realD, &violponto, &violprop);

  for (int i = 0; i < bbde_np; i++)
    bbde_evaluate(bbde_ipop1, i, caixa, contrEstoc, bbde_realD,
      bbde_ncons, tcFes, gen, bbde_nfeval);

  indpop = 0;
  if (violponto == 0.0) {
    violponto = bbde_ipop1[indpop].viol;
    indpop++;
  }
  if (violprop == 0.0)
    violprop = bbde_ipop1[indpop].viol;

  bbde_ordenaPop(bbde_ipop1, bbde_np);
  bbde_inicializaEpsLvl(bbde_ipop1, bbde_np);
  bbde_atualizaDados(bbde_ipop1, &cmin, &emin, &meanviol, bbde_ncons);
  bbde_controle(bbde_ipop1, bbde_np, &cmean, &cvar, &emean, &evar, &cstd, &estd);

  bbde_ipold = bbde_ipop1; /* população antiga (geração G)   */
  bbde_ipnew = bbde_ipop2; /* população nova   (geração G+1) */

  /*==================================================================*/
  /*=========Loop de Operações========================================*/
  /*==================================================================*/
  while (*bbde_nfeval < bbde_maxfes) {
    gen++;
    succ = 0;
    bbde_strat1.nsucc_params = bbde_strat2.nsucc_params = bbde_strat3.nsucc_params = 0;

    /*----------Opera população A----------------------------------*/
    for (int i = 0; i < bbde_np/2; i++) { // loop de indivíduos
      // DE/rand/1 - tmp = r1 + F * (r2 - r3)
      bbde_strategy1exp(&bbde_strat1, bbde_ipold, bbde_ipopTmp[0].ponto,
        caixa, i, bbde_np, bbde_realD, *bbde_nfeval, bbde_maxfes);
      // DE/current-to-rand/1 - tmp = ri + rnd * (r1 - ri) + F * (r2 - r3)
      bbde_strategy2(&bbde_strat2, bbde_ipold, bbde_ipopTmp[1].ponto,
        caixa, i, bbde_np, bbde_realD, *bbde_nfeval, bbde_maxfes);
      // DE/current-to-pBest/1 - tmp = ri + F * (pBest - ri) + F * (r1 - r2)
      bbde_strategy3exp(&bbde_strat3, bbde_ipold, bbde_ipopTmp[2].ponto,
        caixa, i, bbde_np, bbde_realD, taxaPbest, *bbde_nfeval, bbde_maxfes);

      // contabiliza custo e epsilon de cada tmp
      for (int j = 0; j < 3; j++)
        bbde_evaluate(bbde_ipopTmp, j, caixa, contrEstoc,
          bbde_realD, bbde_ncons, tcFes, gen, bbde_nfeval);

      // define quem é o melhor tmp / trial
      int itmp = 0;
      double tmpRCost = bbde_ipopTmp[0].cost;
      double tmpREps = bbde_ipopTmp[0].viol;
      for (int j = 1; j < 3; j++) {
        int melhor = bbde_epsConstCompReal(bbde_ipopTmp[j].cost, bbde_ipopTmp[j].viol,
          tmpRCost, tmpREps);
        if (melhor) {
          itmp = j;
          tmpRCost = bbde_ipopTmp[j].cost;
          tmpREps = bbde_ipopTmp[j].viol;
        }
      }

      // compara com o pai
      int melhor = bbde_epsConstCompReal(bbde_ipopTmp[itmp].cost, bbde_ipopTmp[itmp].viol,
        bbde_ipold[i].cost, bbde_ipold[i].viol);

      if (melhor) {
        // copia para a pop da próxima geração
        bbde_copiaIndiv(bbde_ipnew, i, bbde_ipopTmp, itmp, bbde_realD);
        succ++;

        // registra sucesso da estratégia que gerou o indivíduo experimental
        if (itmp == 0)
          bbde_registraMelhoria(bbde_ipold, bbde_ipnew, i, bbde_realD, &bbde_strat1);
        else if (itmp == 1)
          bbde_registraMelhoria(bbde_ipold, bbde_ipnew, i, bbde_realD, &bbde_strat2);
        else
          bbde_registraMelhoria(bbde_ipold, bbde_ipnew, i, bbde_realD, &bbde_strat3);
      }
      else {
        // se o melhor indivíduo não for melhor que o pai
        // o pai vai para a próxima geração
        bbde_copiaIndiv(bbde_ipnew, i, bbde_ipold, i, bbde_realD);
      }
    } // fim do for da população A

    /*----------Atualiza melhor estratégia-------------------------*/
    bbde_atualizaMelhorEstr(&bbde_strat1, &bbde_strat2, &bbde_strat3, gen);

    /*----------Opera população B----------------------------------*/
    for (int i = bbde_np/2; i < bbde_np; i++) {
      double tmpRand = gera_random();

      // DE/rand/1 - tmp = r1 + F * (r2 - r3)
      if (tmpRand <= bbde_strat1.sr)
        bbde_strategy1exp(&bbde_strat1, bbde_ipold, bbde_ipopTmp[0].ponto,
          caixa, i, bbde_np, bbde_realD, *bbde_nfeval, bbde_maxfes);
      // DE/current-to-rand/1 - tmp = ri + rnd * (r1 - ri) + F * (r2 - r3)
      else if (tmpRand <= bbde_strat1.sr + bbde_strat2.sr)
        bbde_strategy2(&bbde_strat2, bbde_ipold, bbde_ipopTmp[0].ponto,
          caixa, i, bbde_np, bbde_realD, *bbde_nfeval, bbde_maxfes);
      // DE/current-to-pBest/1 - tmp = ri + F * (pBest - ri) + F * (r1 - r2)
      else
        bbde_strategy3exp(&bbde_strat3, bbde_ipold, bbde_ipopTmp[0].ponto,
          caixa, i, bbde_np, bbde_realD, taxaPbest, *bbde_nfeval, bbde_maxfes);

      // avalia a solução gerada pela operação
      bbde_evaluate(bbde_ipopTmp, 0, caixa, contrEstoc,
        bbde_realD, bbde_ncons, tcFes, gen, bbde_nfeval);

      // compara com o pai
      int melhor = bbde_epsConstCompReal(bbde_ipopTmp[0].cost, bbde_ipopTmp[0].viol,
        bbde_ipold[i].cost, bbde_ipold[i].viol);

      if (melhor) {
        bbde_copiaIndiv(bbde_ipnew, i, bbde_ipopTmp, 0, bbde_realD);
        succ++;
      }

      // se o melhor indivíduo não for melhor que o pai
      // o pai vai para a próxima geração
      else
        bbde_copiaIndiv(bbde_ipnew, i, bbde_ipold, i, bbde_realD);

    } // fim do for da população B

    // atualiza memórias de F e CR
    bbde_atualizaParam(&bbde_strat1, bbde_np);
    bbde_atualizaParam(&bbde_strat2, bbde_np);
    bbde_atualizaParam(&bbde_strat3, bbde_np);

    bbde_ordenaPop(bbde_ipnew, bbde_np);

    /*----------Redução linear da população------------------------*/
    int novoNp = round(npinit -
      ((double)(*bbde_nfeval) / bbde_maxfes) * (npinit - 5));
    if ((novoNp < bbde_np) && (novoNp > 5)) {
      bbde_np = novoNp;
      if (taxaPbest >= 0.25/2)
        taxaPbest = taxaPbest * (1.0 - 0.5 * (*bbde_nfeval) / (double)bbde_maxfes);
    }

    /*----------Reinjeção do bestpoint e do valor de propagação----*/
    indpop = bbde_np - 1;
    if ((bbde_valorEpsLvl() <= violponto) && (violponto != INFINITODE) &&
      (!injetouBestpoint)) {
      for (int k = 0; k < bbde_realD; k++)
        bbde_ipnew[indpop].ponto[k] = (*caixa).ponto[k];
      avaliacao_real(bbde_ipnew, indpop, bbde_ncons);
      injetouBestpoint = 1;
      indpop--;
    }

    if ((bbde_valorEpsLvl() <= violprop) && (violprop != INFINITODE) &&
      (!injetouVProp) && (APLICA_CGAC)) {
      for (int k = 0; k < bbde_realD; k++)
        bbde_ipnew[indpop].ponto[k] = (*caixa).vprop[k];
      avaliacao_real(bbde_ipnew, indpop, bbde_ncons);
      injetouVProp = 1;
    }

    /*----------Atualiza dados da exeução--------------------------*/
    bbde_atualizaDados(bbde_ipnew, &cmin, &emin, &meanviol, bbde_ncons);
    bbde_controle(bbde_ipnew, bbde_np, &cmean, &cvar, &emean, &evar, &cstd, &estd);

    /*----------Verifica se deve abortar a execução----------------*/
    if (succ == 0)
      ult_succ++;
    else
      ult_succ = 0;

    // double porcfes = ((double)*bbde_nfeval * 100) / (double)bbde_maxfes;
    // printf("\tbbde: %3.1f; %ld; np:%d; eL:%.5g min: %g;%g; avg: %g;%g; std: %g;%g; succ:%d\n",
    //   porcfes, *bbde_nfeval, bbde_np, bbde_valorEpsLvl(),
    //   cmin, emin, cmean, emean, cstd, estd, succ);

    bbde_ipswap = bbde_ipold;
    bbde_ipold  = bbde_ipnew;
    bbde_ipnew  = bbde_ipswap;

    if ((ABORTA_BBDE) && (ult_succ == MEM_SUCC_BBDE))
      break;
  } // fim do loop de gerações

  // salva melhor ponto encontrado na busca local
  (*contrEstoc).cost = bbde_ipold[0].cost;
  (*contrEstoc).viol = bbde_ipold[0].viol;
  memcpy((*contrEstoc).ponto, bbde_ipold[0].ponto, bbde_realD * (sizeof(double)));
}

/*-------Fim da busca local-----------------------------------------*/
