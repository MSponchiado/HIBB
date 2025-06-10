/******************************************************************************
 *
 * Copyright (C) 2018 Guilherme Alex Derenievicz
 * This file is part of OGRe
 *
 * OGRe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGRe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGRe.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************


 ******************************************************************************
 *
 *  OGRe - Otimização Global Relaxada
 *
 *  Biblioteca multi-intervalar.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "multi-intervalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------

Mintervalo cria_Mintervalo() {
  return cria_lista();
}

//--------------------------------------------------------------------------

void destroi_Mintervalo(void *X) {
  destroi_lista(X, destroi_intervalo);
}

//--------------------------------------------------------------------------

Mintervalo copia_Mintervalo(Mintervalo X) {

  Mintervalo Y = cria_Mintervalo();
  no p;

  for (p = primeiro_no(X); p; p = proximo_no(p))
    enfileira(copia_intervalo(objeto(p)), Y);

  return Y;
}

//--------------------------------------------------------------------------

void imprime_Mintervalo(Mintervalo X) {

  no p;

  printf("{");
  for (p = primeiro_no(X); p; p = proximo_no(p)) {
    imprime_intervalo(objeto(p));
    printf(" ");
  }
  printf("}");
}

//--------------------------------------------------------------------------

int M_eh_vazio(Mintervalo X) {

  no p;

  for (p = primeiro_no(X); p; p = proximo_no(p))
    if (!eh_vazio(objeto(p)))
      return FALSE;

  return TRUE;
}

//--------------------------------------------------------------------------

int M_degenerado(Mintervalo X) {
  no n = primeiro_no(X);
  return (!proximo_no(n) && degenerado(objeto(n)));
}

//--------------------------------------------------------------------------

int M_intervalos_degenerados(Mintervalo X) {
  for (no n = primeiro_no(X); n; n = proximo_no(n))
    if (!degenerado(objeto(n)))
      return FALSE;
  return TRUE;
}

//--------------------------------------------------------------------------

double M_comprimento(Mintervalo X) {
  double sum = 0;
  for (no n = primeiro_no(X); n; n = proximo_no(n))
    sum += comprimento(objeto(n));
  return sum;
}

//--------------------------------------------------------------------------

void limpa_Mintervalo(Mintervalo X) {
  limpa_lista(X, destroi_intervalo);
}


//--------------------------------------------------------------------------

// X <- X intersecao Y.
// devolve 1 em caso de sucesso e alteração de X
// e 0 em caso de falha ou de X == X intersecao Y
int Mcap(Mintervalo X, Mintervalo Y) {


  //printf("Mcap: ");
  //imprime_Mintervalo(X);
  //imprime_Mintervalo(Y);
  //printf("\n");
  // Se X está contido em Y, então X == X intersecao Y
  // Caso contrário, estamos podando valores de X
  if (Msubset(X, Y)) return FALSE;

  intervalo Z;

  lista aux;
  no p, q;

  aux = cria_lista();

  p = primeiro_no(X);
  q = primeiro_no(Y);

  while (p && q) {

    intervalo I = objeto(p);
    intervalo J = objeto(q);

    if (r_cmp(I->b, "<", inf(J), &(I->b)) || (r_cmp(I->b, "==", inf(J), &(I->b)) && !(Td(I) && Te(J))))
      p = proximo_no(p);
    else {

      Z = copia_intervalo(I);
      i_cap(Z, J);

      if (eh_vazio(Z))
        destroi_intervalo(Z);
      else
        enfileira(Z, aux);

      if (r_cmp(I->b, "<", sup(J), &(I->b)))
        p = proximo_no(p);
      else
        q = proximo_no(q);
    }
  }

  // se todas deram vazio (ou os próprios operandos são vazios), cria intervalo vazio
  if (primeiro_no(aux) == NULL) {
    Z = cria_intervalo(0, 0, 0, 0);
    enfileira(Z, aux);
  }

  // aux eh o resultado da interseção
  limpa_Mintervalo(X);
  for (p = primeiro_no(aux); p; p = proximo_no(p))
    enfileira(objeto(p), X);
  destroi_lista(aux, nao_destroi);

  //printf("-> ");
  //imprime_Mintervalo(X);
  //printf("\n");

  return TRUE;
}

//--------------------------------------------------------------------------

// opera X com Y.
// a operação também deve ser passada como parâmetro.
// pela definição X op Y é o resultado de op aplicado a todo
// par de intervalos de ambos X e Y
Mintervalo Mop(Mintervalo X, lista i_ope(intervalo A, intervalo B),
               Mintervalo Y){

  no p, q;
  lista tmp;

  Mintervalo Z = cria_Mintervalo();

  for (p = primeiro_no(X); p; p = proximo_no(p))
    for (q = primeiro_no(Y); q; q = proximo_no(q)) {
      tmp = i_ope(objeto(p), objeto(q));
      merge_ordenado(Z, tmp);
    }

  if (primeiro_no(Z) == NULL)
    enfileira(cria_intervalo(0, 0, 0, 0), Z);

  return Z;
}

//--------------------------------------------------------------------------

// devolve 1 se X é subconjunto ou igual a Y
int Msubset(Mintervalo X, Mintervalo Y) {

  no p, q;

  p = primeiro_no(X);
  q = primeiro_no(Y);

  while (p && q) {

    intervalo I = objeto(p);
    intervalo J = objeto(q);

    if (r_cmp(I->a, ">", inf(J), &(I->a)) || (r_cmp(I->a, "==", inf(J), &(I->a)) && (Te(I) <= Te(J)))) {
      if (r_cmp(I->b, "<", sup(J), &(I->b)) || (r_cmp(I->b, "==", sup(J), &(I->b)) && (Td(I) <= Td(J))))
        p = proximo_no(p);
      else
        q = proximo_no(q);
    }
    else
      return 0;
  }

  return (!p && q);
}

//--------------------------------------------------------------------------

// particiona X (verificando sempre a granularidade_minima), ficando com o
// o lado esquerdo ('a') ou o direito ('b') desse multi-intervalo.
void M_particiona(Mintervalo X, char lado, double delta) {

  // conta quantos intervalos tem X
  int i, cont = 0;
  no p, q, r;
  for (p = primeiro_no(X); p; p = proximo_no(p))
    cont++;

  i = 0;
  for (p = primeiro_no(X); i < cont/2; p = proximo_no(p))
    i++;

  // remove intervalos que não pertencem ao lado escolhido
  if (lado == 'a')
    for (q = proximo_no(p); q; ) {
      r = proximo_no(q);
      remove_no(q, X, destroi_intervalo);
      q = r;
    }
  if (lado == 'b')
    for (q = anterior_no(p); q; ) {
      r = anterior_no(q);
      remove_no(q, X, destroi_intervalo);
      q = r;
    }

  // se numero de intervalos é par, é preciso remover
  // o próprio nó p, no caso do lado ser 'a'
  if (cont % 2 == 0) {
    if (lado == 'a')
      remove_no(p, X, destroi_intervalo);
  }

  // do contrário, é preciso ramificar o intervalo p
  else {

    // Se particiona gera intervalo vazio, retira ele do
    // multi-intervalo (a nao ser que seja o unico)
    particiona(objeto(p), lado, delta);
    if (eh_vazio(objeto(p)) && (proximo_no(p) || anterior_no(p)))
      remove_no(p, X, destroi_intervalo);
  }
}

//--------------------------------------------------------------------------

// atribui a X um valor double que pode ser min X, se pos == 'a', ou max X, se
// pos == 'c', ou um valor do meio de X, se pos == 'b'. Tambem pode ser o multi-
// intervalo formado pelos 3 reais representados por 'a', 'b' e 'c', se pos == 't'.

void M_real_valuation(Mintervalo X, char pos) {

  no n;
  double value;
  intervalo I, J, K;

  if (pos == 'a' || pos == 't') {
    n = primeiro_no(X);
    value = i_min(objeto(n));
    I = cria_intervalo(1, 1, value, value);
  }
  if (pos == 'c' || pos == 't') {
    n = ultimo_no(X);
    value = i_max(objeto(n));
    K = cria_intervalo(1, 1, value, value);
  }
  if (pos == 'b' || pos == 't') {
    // conta quantos intervalos tem X
    int cont = 0;
    for (n = primeiro_no(X); n; n = proximo_no(n))
      cont++;

    if (ESCOLHA_ALEATORIA_INDE)
      cont = rand() % cont;
    else
      cont = cont/2;

    // pega intervalo
    int i = 0;
    for (n = primeiro_no(X); i < cont; n = proximo_no(n))
      i++;

    J = copia_intervalo(objeto(n));

    real_valuation(J, 'b');
  }

  limpa_Mintervalo(X);

  if (pos == 'a' || pos == 't')
    enfileira(I, X);
  if (pos == 'b' || pos == 't')
    enfileira(J, X);
  if (pos == 'c' || pos == 't')
    enfileira(K, X);
}

//--------------------------------------------------------------------------

