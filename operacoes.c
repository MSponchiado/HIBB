#include "operacoes.h"

int rbest, r1, r2, r3;
double ampi, amp1, amp2, amp3, amptmp, ampbest, ampdom;
double medi, med1, med2, med3, medtmp, medbest;

// void geraRand(int i, int np) {
//   do {
//     r1 = (int)(gera_random() * (float)np * 0.5);
//   } while(r1==i);
//   do {
//     r2 = (int)(gera_random() * (float)np * 0.5);
//   } while((r2==i) || (r2==r1));
//   do {
//     r3 = (int)(gera_random() * np);
//   } while((r3==i) || (r3==r1) || (r3==r2));
// }
//
// void alocaIndAux(t_inter *aux, t_inter *ipop, int interD) {
//   memcpy(aux, ipop, interD * (sizeof(t_inter)));
//   corrigeInfsDom(aux, interD);
// }
//
// // garante que o novo intervalo estará dentro dos limites originais
// void boundConstrained(t_indice *ipold, t_inter *tmp, t_indice *dominio,
//   int i, int n) {
//   double aleat, desloc;
//
//   // desloc é a distância entre a borda do indivíduo de popold
//   // e a borda do domínio
//   if (tmp[n].bi <= dominio[n].bi) {
//     aleat = gera_random();
//     desloc = ipold[i].ind[n].bi - dominio[n].bi;
//     if ((desloc + amptmp) > ampdom)
//       desloc = ampdom - amptmp;
//     tmp[n].bi = dominio[n].bi + (aleat * desloc);
//     tmp[n].bs = tmp[n].bi + amptmp;
//   }
//   if (tmp[n].bs >= dominio[n].bs) {
//     aleat = gera_random();
//     desloc = dominio[n].bs - ipold[i].ind[n].bs;
//     if ((desloc + amptmp) > ampdom)
//       desloc = ampdom - amptmp;
//     tmp[n].bs = dominio[n].bs - (aleat * desloc);
//     tmp[n].bi = tmp[n].bs - amptmp;
//   }
//
//   // para prevenir erro numérico
//   if ((tmp[n].bi < dominio[n].bi) || (tmp[n].bi > dominio[n].bs))
//     tmp[n].bi = dominio[n].bi;
//   if ((tmp[n].bs > dominio[n].bs) || (tmp[n].bs < dominio[n].bi))
//     tmp[n].bs = dominio[n].bs;
//
//   if ((tmp[n].bi < dominio[n].bi) || (tmp[n].bs > dominio[n].bs)) {
//     printf("estouro de borda: dom[%g, %g] - tmp[%g, %g]\n",
//     dominio[n].bi, dominio[n].bs, tmp[n].bi, tmp[n].bs);
//     exit(1);
//   }
// }
//
// void operacao1(t_strat *strat, int i) {
//   if (amp1 == 0) amp1 = TOLERANCIA;
//   if (amp2 == 0) amp2 = TOLERANCIA;
//   if (amp3 == 0) amp3 = TOLERANCIA;
//
//   amptmp = (amp1 * (1 + (*strat).pop_sf[i] * ((amp2 / amp3) - 1))) + TOLERANCIA;
//
//   if (amptmp > ampdom)
//     amptmp = (amp1 + (gera_random() * (ampdom - amp1))) - TOLERANCIA;
//
//   medtmp = med1 + (*strat).pop_sf[i] * (med2 - med3);
// }
//
// void operacao2(t_strat *strat, int i) {
//   double rnd = gera_random();
//
//   if (ampi == 0) ampi = TOLERANCIA;
//   if (amp1 == 0) amp1 = TOLERANCIA;
//   if (amp2 == 0) amp2 = TOLERANCIA;
//   if (amp3 == 0) amp3 = TOLERANCIA;
//
//   amptmp = (ampi * (1 + rnd * ((amp1 / ampi) - 1)) *
//     (1 + (*strat).pop_sf[i] * ((amp2 / amp3) - 1))) + TOLERANCIA;
//
//   if (amptmp > ampdom)
//     amptmp = (ampi + (gera_random() * (ampdom - ampi))) - TOLERANCIA;
//
//   medtmp = medi + rnd * (med1 - medi) + (*strat).pop_sf[i] * (med2 - med3);
// }
//
// void operacao3(t_strat *strat, int i) {
//   if (ampi == 0) ampi = TOLERANCIA;
//   if (amp1 == 0) amp1 = TOLERANCIA;
//   if (amp2 == 0) amp2 = TOLERANCIA;
//
//   amptmp = (ampi * (1 + (*strat).pop_sf[i] * ((ampbest / ampi) - 1)) *
//     (1 + (*strat).pop_sf[i] * ((amp1 / amp2) - 1))) + TOLERANCIA;
//
//   if (amptmp > ampdom)
//     amptmp = (ampi + (gera_random() * (ampdom - ampi))) - TOLERANCIA;
//
//   medtmp = medi + (*strat).pop_sf[i] * (medbest - medi) +
//     (*strat).pop_sf[i] * (med1 - med2);
// }
//
// void completaIndiv(t_inter *tmp, t_inter *dominio, int interD, int realD) {
//   for (int j = realD; j < interD; j++) {
//     tmp[j].bi = dominio[j].bi;
//     tmp[j].bs = dominio[j].bs;
//   }
// }
//
// // DE/rand/1/bin
// // tmp = r1 + F * (r2 - r3),
// // onde r1 e r2 são sorteados dentre os melhores np/2 indivíduos
// void strategy1bin(t_strat *strat, t_indice *ipold, t_inter *tmp,
//   t_inter *dominio, int i, int np, int interD, int realD,
//   int nfeval, long maxfes) {
//
//   int n, l;
//
//   geraRand(i, np);
//   geraParam(strat, i, nfeval, maxfes);
//   memcpy(tmp, ipold[i].ind, interD * (sizeof(t_inter)));
//   corrigeInfsDom(tmp, interD);
//
//   t_inter *aux1 = aloca_vecStruct(interD);
//   t_inter *aux2 = aloca_vecStruct(interD);
//   t_inter *aux3 = aloca_vecStruct(interD);
//
//   alocaIndAux(aux1, ipold[r1].ind, interD);
//   alocaIndAux(aux2, ipold[r2].ind, interD);
//   alocaIndAux(aux3, ipold[r3].ind, interD);
//
//   n = (int)(gera_random() * realD);
//
//   for (l = 0; l < realD; l++) {
//     if ((gera_random() < (*strat).pop_cr[i]) || (l == (realD - 1))) {
//
//       amp1 = aux1[n].bs - aux1[n].bi;
//       amp2 = aux2[n].bs - aux2[n].bi;
//       amp3 = aux3[n].bs - aux3[n].bi;
//       ampdom = dominio[n].bs - dominio[n].bi;
//       if (isinf(ampdom)) ampdom = INFINITODE * 2;
//
//       med1 = aux1[n].bi + (amp1 / 2);
//       med2 = aux2[n].bi + (amp2 / 2);
//       med3 = aux3[n].bi + (amp3 / 2);
//
//       operacao1(strat, i);
//       tmp[n].bi = medtmp - (amptmp / 2);
//       tmp[n].bs = medtmp + (amptmp / 2);
//       boundConstrained(ipold, tmp, dominio, i, n);
//     }
//     n = (n + 1) % realD;
//   }
//
//   completaIndiv(tmp, dominio, interD, realD);
//   verificaNan(tmp, interD);
//
//   free(aux1);
//   free(aux2);
//   free(aux3);
// }
//
// // DE/rand/1/exp
// // tmp = r1 + F * (r2 - r3),
// // onde r1 e r2 são sorteados dentre os melhores np/4 indivíduos
// void strategy1exp(t_strat *strat, t_indice *ipold, t_inter *tmp,
//   t_inter *dominio, int i, int np, int interD, int realD,
//   int nfeval, long maxfes) {
//
//   int n, l;
//
//   geraRand(i, np);
//   geraParam(strat, i, nfeval, maxfes);
//   memcpy(tmp, ipold[i].ind, interD * (sizeof(t_inter)));
//   corrigeInfsDom(tmp, interD);
//
//   t_inter *aux1 = aloca_vecStruct(interD);
//   t_inter *aux2 = aloca_vecStruct(interD);
//   t_inter *aux3 = aloca_vecStruct(interD);
//
//   alocaIndAux(aux1, ipold[r1].ind, interD);
//   alocaIndAux(aux2, ipold[r2].ind, interD);
//   alocaIndAux(aux3, ipold[r3].ind, interD);
//
//   n = (int)(gera_random() * realD);
//   l = 0;
//
//   do {
//     // calcula as amplitudes dos intervalos
//     amp1 = aux1[n].bs - aux1[n].bi;
//     amp2 = aux2[n].bs - aux2[n].bi;
//     amp3 = aux3[n].bs - aux3[n].bi;
//     ampdom = dominio[n].bs - dominio[n].bi;
//     if (isinf(ampdom)) ampdom = INFINITODE * 2;
//
//     // calcula o ponto médio dos intervalos
//     med1 = aux1[n].bi + (amp1 / 2);
//     med2 = aux2[n].bi + (amp2 / 2);
//     med3 = aux3[n].bi + (amp3 / 2);
//
//     // aplica mutação no intervalo de tmp e atualiza bordas
//     operacao1(strat, i);
//     tmp[n].bi = medtmp - (amptmp / 2);
//     tmp[n].bs = medtmp + (amptmp / 2);
//     boundConstrained(ipold, tmp, dominio, i, n);
//
//     n = (n + 1) % realD;
//     l++;
//
//   } while((gera_random() < (*strat).pop_cr[i]) && (l < realD));
//
//   completaIndiv(tmp, dominio, interD, realD);
//   verificaNan(tmp, interD);
//
//   free(aux1);
//   free(aux2);
//   free(aux3);
// }
//
// // DE/current-to-rand/1
// // tmp = ri + rnd * (r1 - ri) + F * (r2 - r3)
// void strategy2(t_strat *strat, t_indice *ipold, t_inter *tmp,
//   t_inter *dominio, int i, int np, int interD, int realD,
//   int nfeval, long maxfes) {
//
//   int n, l;
//
//   geraRand(i, np);
//   geraParam(strat, i, nfeval, maxfes);
//   memcpy(tmp, ipold[i].ind, interD * (sizeof(t_inter)));
//   corrigeInfsDom(tmp, interD);
//
//   t_inter *auxi = aloca_vecStruct(interD);
//   t_inter *aux1 = aloca_vecStruct(interD);
//   t_inter *aux2 = aloca_vecStruct(interD);
//   t_inter *aux3 = aloca_vecStruct(interD);
//
//   alocaIndAux(auxi, ipold[i].ind, interD);
//   alocaIndAux(aux1, ipold[r1].ind, interD);
//   alocaIndAux(aux2, ipold[r2].ind, interD);
//   alocaIndAux(aux3, ipold[r3].ind, interD);
//
//   // escolhe um ponto aleatório do vetor para começar a operação
//   n = (int)(gera_random() * realD);
//
//   for (l = 0; l < realD; l++) {
//
//     ampi = auxi[n].bs - auxi[n].bi;
//     amp1 = aux1[n].bs - aux1[n].bi;
//     amp2 = aux2[n].bs - aux2[n].bi;
//     amp3 = aux3[n].bs - aux3[n].bi;
//     ampdom = dominio[n].bs - dominio[n].bi;
//     if (isinf(ampdom)) ampdom = INFINITODE * 2;
//
//     medi = auxi[n].bi + (ampi / 2);
//     med1 = aux1[n].bi + (amp1 / 2);
//     med2 = aux2[n].bi + (amp2 / 2);
//     med3 = aux3[n].bi + (amp3 / 2);
//
//     operacao2(strat, i);
//     tmp[n].bi = medtmp - (amptmp / 2);
//     tmp[n].bs = medtmp + (amptmp / 2);
//     boundConstrained(ipold, tmp, dominio, i, n);
//
//     n = (n + 1) % realD;
//   }
//
//   completaIndiv(tmp, dominio, interD, realD);
//   verificaNan(tmp, interD);
//
//   free(auxi);
//   free(aux1);
//   free(aux2);
//   free(aux3);
// }
//
// // DE/current-to-pBest/1/bin
// // tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// // onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
// void strategy3bin(t_strat *strat, t_indice *ipold,
//   t_inter *tmp, t_inter *dominio, int i, int np, int interD, int realD,
//   int nfeval, long maxfes) {
//
//   int n, l;
//
//   geraParam(strat, i, nfeval, maxfes);
//   memcpy(tmp, ipold[i].ind, interD * (sizeof(t_inter)));
//   corrigeInfsDom(tmp, interD);
//
//   int pbestNum = (int)((float)np * 0.25);
//   if (pbestNum <= 1)
//     pbestNum = 2;
//
//   do {
//     // rbest = rand() % pbestNum;
//     rbest = (int)(gera_random() * pbestNum);
//   } while(rbest==i);
//   do {
//     r1 = (int)(gera_random() * (float)np * 0.5);
//   } while((r1==i) || (r1==rbest));
//   do {
//     r2 = (int)(gera_random() * (float)(np));
//   } while((r2==i) || (r2==r1) || (r2==rbest));
//
//   t_inter *auxi = aloca_vecStruct(interD);
//   t_inter *aux1 = aloca_vecStruct(interD);
//   t_inter *aux2 = aloca_vecStruct(interD);
//   t_inter *auxbest = aloca_vecStruct(interD);
//
//   alocaIndAux(auxi, ipold[i].ind, interD);
//   alocaIndAux(aux1, ipold[r1].ind, interD);
//   alocaIndAux(aux2, ipold[r2].ind, interD);
//   alocaIndAux(auxbest, ipold[r3].ind, interD);
//
//   // escolhe um ponto aleatório do vetor para começar a operação
//   n = (int)(gera_random() * realD);
//
//   for (l = 0; l < realD; l++) {
//     if ((gera_random() < (*strat).pop_cr[i]) || (l == (realD - 1))) {
//
//       ampi = auxi[n].bs - auxi[n].bi;
//       amp1 = aux1[n].bs - aux1[n].bi;
//       amp2 = aux2[n].bs - aux2[n].bi;
//       ampbest = auxbest[n].bs - auxbest[n].bi;
//       ampdom = dominio[n].bs - dominio[n].bi;
//       if (isinf(ampdom)) ampdom = INFINITODE * 2;
//
//       medi = auxi[n].bi + (ampi / 2);
//       med1 = aux1[n].bi + (amp1 / 2);
//       med2 = aux2[n].bi + (amp2 / 2);
//       medbest = auxbest[n].bi + (ampbest / 2);
//
//       operacao3(strat, i);
//       tmp[n].bi = medtmp - (amptmp / 2);
//       tmp[n].bs = medtmp + (amptmp / 2);
//       boundConstrained(ipold, tmp, dominio, i, n);
//     }
//
//     n = (n + 1) % realD;
//   }
//
//   completaIndiv(tmp, dominio, interD, realD);
//   verificaNan(tmp, interD);
//
//   free(auxi);
//   free(aux1);
//   free(aux2);
//   free(auxbest);
// }
//
// // DE/current-to-pBest/1/exp
// // tmp = ri + F * (pBest - ri) + F * (r1 - r2),
// // onde pbest um indivíduo aleatório dentre os melhores pBest indivíduos
// void strategy3exp(t_strat *strat, t_indice *ipold,
//   t_inter *tmp, t_inter *dominio, int i, int np, int interD, int realD,
//   int nfeval, long maxfes) {
//
//   int n, l;
//
//   geraParam(strat, i, nfeval, maxfes);
//   memcpy(tmp, ipold[i].ind, interD * (sizeof(t_inter)));
//   corrigeInfsDom(tmp, interD);
//
//   int pbestNum = (int)((float)np * 0.25);
//   if (pbestNum <= 1)
//     pbestNum = 2;
//
//   do {
//     // rbest = rand() % pbestNum;
//     rbest = (int)(gera_random() * pbestNum);
//   } while(rbest==i);
//   do {
//     r1 = (int)(gera_random() * (float)np * 0.5);
//   } while((r1==i) || (r1==rbest));
//   do {
//     r2 = (int)(gera_random() * (float)(np));
//   } while((r2==i) || (r2==r1) || (r2==rbest));
//
//   t_inter *auxi = aloca_vecStruct(interD);
//   t_inter *aux1 = aloca_vecStruct(interD);
//   t_inter *aux2 = aloca_vecStruct(interD);
//   t_inter *auxbest = aloca_vecStruct(interD);
//
//   alocaIndAux(auxi, ipold[i].ind, interD);
//   alocaIndAux(aux1, ipold[r1].ind, interD);
//   alocaIndAux(aux2, ipold[r2].ind, interD);
//   alocaIndAux(auxbest, ipold[r3].ind, interD);
//
//   // escolhe um ponto aleatório do vetor para começar a operação
//   n = (int)(gera_random() * realD);
//   l = 0;
//
//   do {
//
//     ampi = auxi[n].bs - auxi[n].bi;
//     amp1 = aux1[n].bs - aux1[n].bi;
//     amp2 = aux2[n].bs - aux2[n].bi;
//     ampbest = auxbest[n].bs - auxbest[n].bi;
//     ampdom = dominio[n].bs - dominio[n].bi;
//     if (isinf(ampdom)) ampdom = INFINITODE * 2;
//
//     medi = auxi[n].bi + (ampi / 2);
//     med1 = aux1[n].bi + (amp1 / 2);
//     med2 = aux2[n].bi + (amp2 / 2);
//     medbest = auxbest[n].bi + (ampbest / 2);
//
//     operacao3(strat, i);
//     tmp[n].bi = medtmp - (amptmp / 2);
//     tmp[n].bs = medtmp + (amptmp / 2);
//     boundConstrained(ipold, tmp, dominio, i, n);
//
//     n = (n + 1) % realD;
//     l++;
//
//   } while((gera_random() < (*strat).pop_cr[i]) && (l < realD));
//
//   completaIndiv(tmp, dominio, interD, realD);
//   verificaNan(tmp, interD);
//
//   free(auxi);
//   free(aux1);
//   free(aux2);
//   free(auxbest);
// }
