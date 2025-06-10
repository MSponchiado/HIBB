#include "problema.h"
#include "instancia.h"
// #include "instancia.c"

/*----------Inicialização de arquivos---------------------------*/
void arquiv_init(int argc, FILE *fpar, FILE *fout, FILE *flog, FILE *fgac) {
  if (argc != 5){
    printf("\nUso : inde <input-file> <output-file> <fitness-file><gac-log-file>\n");
    exit(1);
  }

  if ((fpar == NULL) || (fout == NULL) || (flog == NULL) || (fgac == NULL)) {
    printf("\nErro ao abrir arquivos.\n");
    fclose(fpar);
    fclose(flog);
    fclose(fout);
    fclose(fgac);
    exit(1);
  }
}

/*----------Inicialização e leitura de parâmetros---------------*/
void params_init(FILE *fpar, long int *multmaxfes,
  int *bbde_multnp, long int *bbde_multmaxfes, int *log, int *seed) {

  int erroParams = 0;
  if (!fscanf(fpar,"%ld",multmaxfes)) erroParams = 1;
  if (!fscanf(fpar,"%d",bbde_multnp)) erroParams = 1;
  if (!fscanf(fpar,"%ld",bbde_multmaxfes)) erroParams = 1;
  if (!fscanf(fpar,"%d",log)) erroParams = 1;
  if (!fscanf(fpar,"%d",seed)) erroParams = 1;
  if (erroParams == 1) {
    printf("Erro no tipo dos parametros.\n");
    exit(1);
  }
  fclose(fpar);
}

/*----------Inicialização da Epífita e dominio pós GAC---------*/
void epifita_dominio_init(epifita C, t_indice *dominio) {
  if (C == NULL) {
    printf("\nErro na Epifita.\n");
    exit(1);
  }
  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n)) {
    for (no p = primeiro_no(variaveis(objeto(n))); p; p = proximo_no(p)) {
      Variavel X = objeto(p);
      dominio->ind[indexV(X)].bi = i_min(objeto(primeiro_no(multi_intervalo(X))));
      dominio->ind[indexV(X)].bs = i_max(objeto(ultimo_no(multi_intervalo(X))));
      if (X->nome[0] == '&')
        indiceFunObj = indexV(X);
    }
  }
}

/*----------Inicialização do tamanho da instancia--------------*/
void tamanho_instancia(int *realD, int *ncons) {
  problem_size(realD, ncons);
}

int geraCaixasIniciais(epifita C, t_indice **caixas_iniciais, int nbuscasinit,
  t_indice *auxgac, t_indice *dominio, int realD, int interD,
  double *mediacontrGAC, long int *consisGAC, long int *inconsisGAC,
  long long int *nfeval, FILE *fgac) {

  int ultima_caixa = 0;
  int ivar = 0;
  while (ultima_caixa < (nbuscasinit - 1)) {
    int inovacaixa = ultima_caixa + 1;
    for (int icaixa = 0; (icaixa <= ultima_caixa) && (inovacaixa < nbuscasinit); icaixa++) {
      copiaIndiv(caixas_iniciais[inovacaixa], caixas_iniciais[icaixa], realD, interD, 1);
      double meio = caixas_iniciais[icaixa]->ind[ivar].bi/2 + caixas_iniciais[icaixa]->ind[ivar].bs/2;
      caixas_iniciais[icaixa]->ind[ivar].bs = meio;
      caixas_iniciais[inovacaixa]->ind[ivar].bi = meio;
      caixas_iniciais[icaixa]->numdiv++;
      caixas_iniciais[inovacaixa]->numdiv++;
      // se evaluate retorna diferente de 0 a caixa é inconsistente
      // aplica consistência local do OGRe para contrair intervalos
      int status_icaixa = contrator_GAC(C, caixas_iniciais[icaixa], auxgac, dominio,
        realD, interD, mediacontrGAC, consisGAC, inconsisGAC, nfeval, fgac);
      int status_inovacaixa = contrator_GAC(C, caixas_iniciais[inovacaixa], auxgac, dominio,
        realD, interD, mediacontrGAC, consisGAC, inconsisGAC, nfeval, fgac);
      if ((status_icaixa == 1) && (status_inovacaixa == 1)) {
        inovacaixa--;
        if (icaixa < inovacaixa)
          copiaIndiv(caixas_iniciais[icaixa], caixas_iniciais[inovacaixa], realD, interD, 1);
      }
      if (status_icaixa == 1) // primeira é inconsistente
        copiaIndiv(caixas_iniciais[icaixa], caixas_iniciais[inovacaixa], realD, interD, 1);
      else if (status_inovacaixa != 1) // segunda nao é inconsistente
        inovacaixa++;
    }
    ultima_caixa = inovacaixa - 1;
    ivar = (ivar + 1) % realD;

    if (ultima_caixa < 0)
      return 1;
  }
  return 0;
}

/*----------Avaliação da caixa com busca local e---------------*/
/*----------aplicação de contratores---------------------------*/
int evaluate(t_indice *caixa, long bbde_maxfes, int bbde_np,
  t_indice *dominio, t_indice *bestbox, t_indice *contrEstoc, int tcFes,
  int interD, long long int *nfevalbest, long long int *nfeval) {

  long bbde_nfeval = 0;
  int newbestbox = 0;

  bbde_principal(caixa, bbde_np, bbde_maxfes, contrEstoc, &bbde_nfeval);

  // soma o orçamento utilizado na busca local com o orçamento geral utilizado
  (*nfeval) += bbde_nfeval;

  if (APLICA_EPSLVL)
    atualizaEpsLvl(nfeval, tcFes);

  // se melhorar o ponto da mãe
  if (epsConstCompReal((*contrEstoc).cost, (*contrEstoc).viol,
    (*caixa).cost, (*caixa).viol))
    (*caixa).cestag = 0;
  else
    (*caixa).cestag++;

  int nacaixa = 1;
  for (int i = 0; i < bbde_realD; i++) {
    if (((*caixa).ponto[i] < (*caixa).ind[i].bi) ||
      ((*caixa).ponto[i] > (*caixa).ind[i].bs)) {
      nacaixa = 0;
      break;
    }
  }

  // se o ponto da mãe não estiver na caixa ou estiver na caixa e for melhorado
  if ((nacaixa == 0) || ((nacaixa == 1) && (*caixa).cestag == 0)) {
    (*caixa).cost = (*contrEstoc).cost;
    (*caixa).viol = (*contrEstoc).viol;
    memcpy((*caixa).ponto, (*contrEstoc).ponto, bbde_realD * (sizeof(double)));
  }

  // atualiza novo bestbox
  if (epsConstCompReal((*caixa).cost, (*caixa).viol,
    (*bestbox).cost, (*bestbox).viol)) {
      copiaIndiv(bestbox, caixa, bbde_realD, interD, 1);
      if (((*bestbox).viol == 0.0) && ((*bestbox).cost < bsFunObj)) {
        bsFunObj = (*bestbox).cost;
      }
      (*nfevalbest) = (*nfeval);
      newbestbox = 1;
  }

  contracao_blackbox(caixa, contrEstoc, dominio, bbde_realD);

  return newbestbox;
}
