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
 *  Implementação de lista duplamente encadeada.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __LISTA__
#define __LISTA__

#include <stdio.h>
#include <stdlib.h>

typedef struct t_no *no;
typedef struct t_lista *lista;

typedef struct t_no {

  no proximo;
  no anterior;
  void *obj;

} t_no;

typedef struct t_lista {

  no primeiro;
  no ultimo;

} t_lista;


lista cria_lista(void);

// destroi os nós e usa destroi_objeto pra destruir o que tem no nó, mas não destrói a lista
void limpa_lista(lista l, void destroi_objeto(void *o));

// destroi os nós e a lista e usa destroi_objeto pra destruir o que tem no nó
void destroi_lista(lista l, void destroi_objeto(void *o));

no primeiro_no(lista l);

no ultimo_no(lista l);

// insere no inicio da lista
no empilha(void *o, lista l);

// insere no final da lista
no enfileira(void *o, lista l);

// insere imediatamente anterior ao no q
no insere(void *o, no q, lista l);

// compara os objetos que a função passada como parâmetro e retorna o nó encontrado
no busca(void *chave, lista l, int compara_objeto(void *o, void *chave));

// remove o nó passado como parâmetro
void remove_no(no n, lista l, void destroi_objeto(void *o));

no proximo_no(no n);

no anterior_no(no n);

void *objeto(no n);

// função que não faz nada, utilizada quando não se deseja destruir os objetos da lista, apenas a lista
void nao_destroi(void *o);

// troca as posições dos nós m e n na lista
void troca(no m, no n, lista l);

#endif
