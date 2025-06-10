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
 *  Implementação da estrutura de dados para variaveis contínuas.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "variavel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------

Variavel cria_Variavel(char *n, intervalo I, int ind) {

  Variavel X = (Variavel) malloc(sizeof(t_Variavel));
  if (X) {
    X->nome = (char *) malloc(sizeof(char)*(strlen(n)+2));
    X->restricoes = cria_lista();
    X->restarcos = cria_lista();
    X->multi_intervalo = cria_Mintervalo();
    X->index = ind;
    X->flag_index = 1;

    // printf("criando variavel %s com indice %d\n", n, ind);

    if (!X->nome || !X->restricoes || !X->restarcos || !X->multi_intervalo) {
      free(X);
      return NULL;
    }

    strcpy(X->nome, n);
    enfileira(I, multi_intervalo(X));
  }
  else
    printf("ERRO: Impossível criar Variavel\n");

  return X;
}

//--------------------------------------------------------------------------

// TODO: as restrições só podem ser destruídas no final. Se
// for necessário, criar uma função só para isso.
void destroi_Variavel(void *X) {
  destroi_lista(restricoes(X), nao_destroi);
  destroi_lista(restarcos(X), nao_destroi);
  destroi_Mintervalo(multi_intervalo(X));
  free(nome(X));
  free(X);
}

//--------------------------------------------------------------------------

char *nome(Variavel X) { return X->nome; }

int indexV(Variavel X) { return X->index; }

int flag_index(Variavel X) { return X->flag_index; }

lista restricoes(Variavel X) { return X->restricoes; }

lista restarcos(Variavel X) { return X->restarcos; }

Mintervalo multi_intervalo(Variavel X) { return X->multi_intervalo; }

//--------------------------------------------------------------------------

// retorna 1 se nome(X) == s
// s eh char *, mas foi colocado como void *
// para ser usada no busca da lista.
int compara_Variavel(void *X, void *s) {
  return !strcmp(nome(X), s);
}

//--------------------------------------------------------------------------

void imprime_Variavel(Variavel X) {

  printf("%s", nome(X));
  imprime_Mintervalo(multi_intervalo(X));
}

//--------------------------------------------------------------------------
