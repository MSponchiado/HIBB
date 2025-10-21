#include "main.h"

double bsFunObj;
int indiceFunObj;

int main(int argc, char *argv[]) {

  clock_t inicio, fim;
  double tempo_usado = 0.0;

  inicio = clock();

  int realD = 0, ncons, bbde_multnp, newbest[2], dimpart, log, seed;
  int nbuscas, nbuscasinit, cont;
  int interD, omega, restr_tern, colapsa = 0, ciclos = 0;
  double porcfes = 0.0, porcnfevalbest = 0.0, dimpartmed;
  double mediacontrGAC = 0.0, mediacontrGACTotal = 0.0;
  long int numdivmax = 0, caixasexpl = 0;
  long int multmaxfes, bbde_multmaxfes, consisGAC = 0, inconsisGAC = 0;
  t_indice *dominio, *bestbox, *caixa, *cx1, *cx2, *contrEstoc, *auxgac;
  t_indice **caixas_iniciais;
  double *tamdominio;
  bsFunObj = INFINITY;

  /*----------Inicialização de arquivos e parâmetros--------------*/
  FILE *fpar = fopen(argv[1],"r");
  FILE *fout = fopen(argv[2],"w");
  FILE *flog = fopen(argv[3],"w");
  FILE *fgac = fopen(argv[4],"w");

  arquiv_init(argc, fpar, fout, flog, fgac);
  params_init(fpar, &multmaxfes, &bbde_multnp, &bbde_multmaxfes, &log, &seed);
  random_init(seed);
  startEpsLvl();

  /*----------Inicialização da instância--------------------------*/
  tamanho_instancia(&realD, &ncons);
  epifita C = epifita_init(&interD, &omega, &restr_tern);
  long long int maxfes = realD * multmaxfes, nfeval = 0, nfevalbest = 0;
  int tcFes = (int)(maxfes * 0.85);
  nbuscas = nbuscasinit = cont = (int)(MULTNBUSCAS * realD);  // TODO: pensar nesse valor
  if (nbuscas < 1)
    nbuscas = nbuscasinit = cont = 1;

  // alocação e definição das caixas intermediárias
  contrEstoc = aloca_box(interD);
  auxgac = aloca_box(interD);
  bestbox = aloca_box(interD);
  (*bestbox).cost = (*bestbox).viol = INFINITY;

  dominio = aloca_box(interD);
  tamdominio = aloca_vecDouble(realD);
  epifita_dominio_init(C, dominio);
  corrigeInfsDom(dominio, interD);

  // aplica GAC
  if(contrator_GAC(C, dominio, auxgac, dominio, realD, interD, &mediacontrGAC,
      &consisGAC, &inconsisGAC, &nfeval, fgac) == 1) {
    printf("Inconsistencia no dominio.\n");
    exit(1);
  }

  // cria nova caixa com o domínio do problema e avalia
  caixa = aloca_box(interD);
  memcpy((*caixa).ind, (*dominio).ind, interD * (sizeof(t_inter)));

  for (int i = 0; i < realD; i++)
    tamdominio[i] = (*dominio).ind[i].bs - (*dominio).ind[i].bi;

  // printbox(dominio, realD, 1);

  // alocação e inicialização do heap
  t_heap fila;
  heap_init(&fila);

  mediacontrGACTotal = (inconsisGAC + (consisGAC * mediacontrGAC)) /
    (inconsisGAC + consisGAC);

  /*----------Inicialização do BBDE e da busca--------------------*/
  int bbde_np = realD * bbde_multnp;
  long bbde_maxfes = realD * bbde_multmaxfes;
  bbde_init(realD, ncons, bbde_np);

  if (APLICA_EPSLVL) {
    evaluate(caixa, bbde_maxfes, bbde_np, dominio, bestbox, contrEstoc,
      tcFes, interD, &nfevalbest, &nfeval);
    inicializaEpsLvl(caixa);
  }

  caixas_iniciais = (t_indice**) malloc(nbuscasinit * sizeof(t_indice*));
  if (!caixas_iniciais) {
    printf("Falta memoria no aloca_ipop\n");
    exit(1);
  }
  caixas_iniciais[0] = caixa;
  for (int i = 1; i < nbuscasinit; i++)
    caixas_iniciais[i] = aloca_box(interD);

  if (geraCaixasIniciais(C, caixas_iniciais, nbuscasinit, auxgac, dominio,
    realD, interD, &mediacontrGAC, &consisGAC, &inconsisGAC, &nfeval, fgac) == 0) {
    // insere a caixa na fila
    for (int i = 0; i < nbuscasinit; i++) {
      evaluate(caixas_iniciais[i], bbde_maxfes, bbde_np, dominio, bestbox,
        contrEstoc, tcFes, interD, &nfevalbest, &nfeval);
      caixasexpl++;
      heap_insert(&fila, caixas_iniciais[i]);
    }
  }
  else {
    evaluate(caixa, bbde_maxfes, bbde_np, dominio, bestbox,
      contrEstoc, tcFes, interD, &nfevalbest, &nfeval);
    caixasexpl++;
    heap_insert(&fila, caixa);
  }

  heap_freeze(&fila);

  porcfes = ((double)nfeval * 100) / (double)maxfes;
  porcnfevalbest = ((double)nfevalbest * 100) / (double)maxfes;
  mediacontrGACTotal = (inconsisGAC + (consisGAC * mediacontrGAC)) /
    (inconsisGAC + consisGAC);

  fim = clock();
  tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

  // fprintf(flog, "%.8e;%.8e;%d;%d;%g;%d;%ld;%ld;%d;%ld;%ld;%g;%ld;%ld;%g;%g;%llu;%llu;%llu;%d;%f\n",
  // (*bestbox).cost, (*bestbox).viol, realD, restr_tern, porcfes, ciclos,
  // heap_size(&fila), caixasexpl, colapsa, numdivmax, (*bestbox).numdiv,
  // porcnfevalbest, consisGAC, inconsisGAC, mediacontrGAC, mediacontrGACTotal,
  // DEBUG_ERRO_INSTANCIACAO, DEBUG_MAX_STEPS_REACHED, MAX_STEPS_USED, nbuscas, tempo_usado);

  /*----------enquantou houver orçamento e caixas na fila---------*/
  while ((nfeval < maxfes) && (heap_size(&fila) != 0)) {
    ciclos++;

    // descongela
    if (cont == 0) {
      heap_unfreeze(&fila);
      cont = nbuscas = round(nbuscasinit - ((double)nfeval / (maxfes)) *
                    (nbuscasinit - 1));
    }

    // atualiza heap com o novo epsilon level
    if (APLICA_EPSLVL)
      heap_update(&fila, valorEpsLvl());

    // extrai a caixa da cabeça da fila, verificando se não é um ponto
    caixa = heap_extract(&fila);
    cont--;
    // congela
    heap_freeze(&fila);

    dimpart = maior_dimensao_relativa(caixa, tamdominio, realD);

    if (ABANDONA_CAIXA) {
      if (((((*caixa).ind[dimpart].bs - (*caixa).ind[dimpart].bi) /
          tamdominio[dimpart]) < DIM_MIN)) {
        colapsa++;
        box_free(caixa);
        continue;
      }
    }

    // cx1 recebe caixa. cx2 aloca e recebe uma cópia da caixa
    cx1 = caixa;
    cx2 = aloca_box(interD);
    copiaIndiv(cx2, caixa, realD, interD, 1);

    // dimpart = (int)(gera_random() * (float)realD);
    dimpartmed = (*caixa).ind[dimpart].bi / 2 + (*caixa).ind[dimpart].bs / 2;
    (*cx1).ind[dimpart].bs = dimpartmed;
    (*cx2).ind[dimpart].bi = dimpartmed;

    // aumenta registro de divisões das caixas
    (*cx1).numdiv++;
    if ((*cx1).numdiv > numdivmax)
      numdivmax = (*cx1).numdiv;

    (*cx2).numdiv++;
    if ((*cx2).numdiv > numdivmax)
      numdivmax = (*cx2).numdiv;

    newbest[0] = newbest[1] = 0;

    // se GAC não der inconsistência, aplica busca local em cx1 e insere na fila
    completaIndiv(cx1, dominio, interD, realD);
    if(!contrator_GAC(C, cx1, auxgac, dominio, realD, interD, &mediacontrGAC,
        &consisGAC, &inconsisGAC, &nfeval, fgac)) {
      newbest[0] = evaluate(cx1, bbde_maxfes, bbde_np, dominio, bestbox,
        contrEstoc, tcFes, interD, &nfevalbest, &nfeval);
      caixasexpl++;
      heap_insert(&fila, cx1);
    } else
      box_free(cx1);

    // se GAC não der inconsistência, aplica busca local em cx2 e insere na fila
    completaIndiv(cx2, dominio, interD, realD);
    if(!contrator_GAC(C, cx2, auxgac, dominio, realD, interD, &mediacontrGAC,
        &consisGAC, &inconsisGAC, &nfeval, fgac)) {
      newbest[1] = evaluate(cx2, bbde_maxfes, bbde_np, dominio, bestbox,
        contrEstoc, tcFes, interD, &nfevalbest, &nfeval);
      caixasexpl++;
      heap_insert(&fila, cx2);
    } else
      box_free(cx2);

    // se o best é atualizado, zera o contador de estagnação
    if ((newbest[0] == 1) || (newbest[1]) == 1)
      (*bestbox).cestag = 0;
    else
      (*bestbox).cestag++;

    /*----------Trace de execução----------------------------------*/
    porcfes = ((double)nfeval * 100) / (double)maxfes;
    porcnfevalbest = ((double)nfevalbest * 100) / (double)maxfes;

    mediacontrGACTotal = (inconsisGAC + (consisGAC * mediacontrGAC)) /
      (inconsisGAC + consisGAC);

    fim = clock();
    tempo_usado = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

    if ((newbest[0] == 1) || (newbest[1]) == 1) {
      // fprintf(flog, "%.8e;%.8e;%d;%d;%g;%d;%ld;%ld;%d;%ld;%ld;%g;%ld;%ld;%g;%g;%llu;%llu;%llu;%d;%f\n",
      // (*bestbox).cost, (*bestbox).viol, realD, restr_tern, porcfes, ciclos,
      // heap_size(&fila), caixasexpl, colapsa, numdivmax, (*bestbox).numdiv,
      // porcnfevalbest, consisGAC, inconsisGAC, mediacontrGAC, mediacontrGACTotal,
      // DEBUG_ERRO_INSTANCIACAO, DEBUG_MAX_STEPS_REACHED, MAX_STEPS_USED, nbuscas, tempo_usado);
      fflush(flog);

      if (log) {
        printf("-------------------- novo best %g %g --------------------\n",
            (*bestbox).cost, (*bestbox).viol);
        printf("c%g;v%g;D%d;rt%d;cic%d;fes%.1f;pbest%.1f;",
          (*bestbox).cost, (*bestbox).viol, realD, restr_tern, ciclos, porcfes, porcnfevalbest); 
        printf("gac%ld;%ld;redGAC%g;%g;%llu;%llu;%llu;%f;", 
          consisGAC, inconsisGAC, mediacontrGAC, mediacontrGACTotal,
          DEBUG_ERRO_INSTANCIACAO, DEBUG_MAX_STEPS_REACHED, MAX_STEPS_USED, tempo_usado);
        printf("cxe%ld;hs%ld;col%d;ndmax%ld;ndbest%ld;nbuscas%d;\n",
          caixasexpl, heap_size(&fila), colapsa, numdivmax, (*bestbox).numdiv, nbuscas);      
      }
    }

  } // fim do loop

  if (log)
    printbox(bestbox, realD, 1);

  contrator_bordadominio(bestbox, realD, dominio);
  newbest[0] = evaluate(bestbox, bbde_maxfes, bbde_np, dominio, bestbox,
    contrEstoc, tcFes, interD, &nfevalbest, &nfeval);
  caixasexpl++;

  if (newbest[0] == 1 && log)
    printf("Melhorou com BL final\n");

  fprintf(fout, "%.8e;%.8e;%d;%d;%d;%g;%g;%ld;%ld;%g;%g;%llu;%llu;%llu;%f;%ld;%ld;%d;%ld;%ld;%d;%d\n",
    (*bestbox).cost, (*bestbox).viol, realD, restr_tern, ciclos, 
    porcfes, porcnfevalbest, consisGAC, inconsisGAC, mediacontrGAC, mediacontrGACTotal,
    DEBUG_ERRO_INSTANCIACAO, DEBUG_MAX_STEPS_REACHED, MAX_STEPS_USED, tempo_usado,
    caixasexpl, heap_size(&fila), colapsa, numdivmax, (*bestbox).numdiv,
    nbuscas, newbest[0]);

  fclose(flog);
  fclose(fout);

  return 0;

}
