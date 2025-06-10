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
 *  Implementação da estrutura de dados para Árvore Epífita.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "epifita.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long long int MAX_STEPS_CONSISTENCY = 100;
unsigned long long int DEBUG_MAX_STEPS_REACHED = 0;         // contador
unsigned long long int MAX_STEPS_USED = 0;
unsigned long long int STEPS_USED = 0;

//--------------------------------------------------------------------------

epifita cria_epifita(void) {

  epifita C = (epifita) malloc(sizeof(t_epifita));
  if (C) {
    C->var_objetivo = NULL;
    C->arvores = cria_lista();
    C->complete_odv = cria_lista();
    if (!C->arvores || !C->complete_odv) {
      free(C);
      return  NULL;
    }
  }
  else
    printf("ERRO: Impossível criar Árvore Epífita\n");

  return C;
}

//--------------------------------------------------------------------------

void destroi_epifita(epifita C) {

  destroi_lista(arvores(C), destroi_arvore);
  destroi_lista(complete_odv(C), nao_destroi);
  free(C);
}

//--------------------------------------------------------------------------

lista arvores(epifita C) { return C->arvores; }

lista complete_odv(epifita C) { return C->complete_odv; }

Variavel var_objetivo(epifita C) { return C->var_objetivo; }

//--------------------------------------------------------------------------
// cria uma ordenação de restrições com grau de interseção no máximo 2,
// seguindo a estrutura da Árvore Epífita.
// odv = ordem de valoração (ou de instanciação das variáveis)
void calcula_complete_odv(epifita C) {

  no p, q, r;

  for (p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
    arvore T = objeto(p);
    restricao R = arco(T);
    if (R)
      enfileira(R, complete_odv(C));

    R = raiz(T);
    enfileira(R, odv(T));
    enfileira(R, complete_odv(C));
    for (q = primeiro_no(odv(T)); q; q = proximo_no(q))
      for (r = primeiro_no(filhos(objeto(q))); r; r = proximo_no(r)) {
        R = objeto(r);
        enfileira(R, odv(T));
        enfileira(R, complete_odv(C));
      }
  }
}

//--------------------------------------------------------------------------
// seta a variável objetivo da decomposição epífita
void seta_variavel_objetivo(epifita C) {
  C->var_objetivo = cabeca(raiz(objeto(primeiro_no(arvores(C)))));
}

//--------------------------------------------------------------------------
// imprime todas as variaveis da cascata
void imprime_variaveis(epifita C) {
  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p))
    for (no q = primeiro_no(variaveis(objeto(p))); q; q = proximo_no(q)) {
      imprime_Variavel(objeto(q));
      printf("\n");
    }
}

//--------------------------------------------------------------------------
// imprime a variavel chamada s da cascata
void imprime_variavel(char *s, epifita C) {
  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p))
    for (no q = primeiro_no(variaveis(objeto(p))); q; q = proximo_no(q))
      if (!strcmp(nome(objeto(q)), s)) {
        imprime_Variavel(objeto(q));
        return;
      }
}

//--------------------------------------------------------------------------

void imprime_epifita(epifita C) {
  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
    imprime_restricoes(raiz(objeto(p)), 1);
    printf("\n++++++++++++++++++++++++++\n");

    if (arco(objeto(p)))
      imprime_restricoes(arco(objeto(p)), 0);

    printf("\n##########################\n");
  }
}

//--------------------------------------------------------------------------
// Aplica a consistência na Árvore Epífita inteira retornando, caso houver, o
// conflito. A lista de variáveis no argumento serve como ponto inicial
// do algoritmo. Se a lista for NULL, basta aplicar em todas as restrições.
int global_consistency(epifita C, lista vars) {

  int res, conflito = FALSE;
  unsigned long long int k;

  no p;
  restricao R;

  if (!vars) {

    for (p = primeiro_no(arvores(C)); p && !conflito; p = proximo_no(p))
      arc_consistency(objeto(p), &conflito);

    if (conflito) {
      STEPS_USED = -0.0;
      return TRUE;
    }

    vars = cria_lista();
    for (p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
      R = arco(objeto(p));
      if (R) {
        res = local_consistency(R, &conflito);
        if (conflito) {
          destroi_lista(vars, nao_destroi);
          STEPS_USED = -0.0;
          return TRUE;
        }

        if (res % 10)
          enfileira(cabeca(R), vars);
        if ((res / 10) % 10)
          enfileira(cauda_E(R), vars);
        if (res / 100)
          enfileira(cauda_D(R), vars);
      }
    }
  }

  k = 0;
  while (primeiro_no(vars) && k < MAX_STEPS_CONSISTENCY) {
    k++;

    for ( p = primeiro_no(restricoes(objeto(primeiro_no(vars)))); p; p = proximo_no(p)) {

      R = objeto(p);
      res = local_consistency(R, &conflito);
      if (conflito) {
        destroi_lista(vars, nao_destroi);
        STEPS_USED = k;
        return TRUE;
      }

      if (res % 10)
        enfileira(cabeca(R), vars);
      if ((res / 10) % 10)
        enfileira(cauda_E(R), vars);
      if (res / 100)
        enfileira(cauda_D(R), vars);
    }

    for ( p = primeiro_no(restarcos(objeto(primeiro_no(vars)))); p; p = proximo_no(p)) {

      R = objeto(p);
      res = local_consistency(R, &conflito);
      if (conflito) {
        destroi_lista(vars, nao_destroi);
        STEPS_USED = k;
        return TRUE;
      }

      if (res % 10)
        enfileira(cabeca(R), vars);
      if ((res / 10) % 10)
        enfileira(cauda_E(R), vars);
      if (res / 100)
        enfileira(cauda_D(R), vars);
    }

    remove_no(primeiro_no(vars), vars, nao_destroi);
  }

  destroi_lista(vars, nao_destroi);
  //imprime_variaveis(C);

  if (k > MAX_STEPS_USED)
    MAX_STEPS_USED = k;

  if (k == MAX_STEPS_CONSISTENCY)
    DEBUG_MAX_STEPS_REACHED++;

  STEPS_USED = k;

  return FALSE;
}

//--------------------------------------------------------------------------

// Aplica a consistência direcionada na Árvore Epífita inteira retornando, caso houver,
// o conflito. Se direcao >= 0, aplicada DAC, caso contrário, DAC reversa.
int directional_consistency(epifita C, no inicial, int direcao) {

  int conflito = FALSE;

  // DAC
  if (direcao >= 0) {
    if (!inicial)
      inicial = ultimo_no(complete_odv(C));
    DAC(inicial, &conflito);
  }

  // DAC reversa
  else {
    if (!inicial)
      inicial = primeiro_no(complete_odv(C));
    DAC_reverse(inicial, &conflito);
  }

  return conflito;
}

//--------------------------------------------------------------------------
// devolve o maior erro de RAC nos arcos da decomposição epífita
double max_erro_RAC(epifita C) {

  double erro, max = -INFINITY;

  for (no n = primeiro_no(arvores(C)); n; n = proximo_no(n)) {

    restricao R = arco(objeto(n));

    if (R) {
      erro = erro_RAC(R);
      if (r_cmp(erro, ">", max, &erro))
        max = erro;
    }
  }

  return max;
}
