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

#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------------------------------------
lista cria_lista(void) {
  lista l = (lista) malloc(sizeof(t_lista));

  if (l)
    l->primeiro = l->ultimo = NULL;
  else
    printf("ERRO: Impossível criar lista\n");
  
  return l;
}

//---------------------------------------------------------------------------------------
void limpa_lista(lista l, void destroi_objeto(void *o)) {
  no aux, p;
  for (p = primeiro_no(l); p; p = aux) {
    destroi_objeto(objeto(p));
    aux = proximo_no(p);
    free(p);
  }

  l->primeiro = l->ultimo = NULL;
}

//---------------------------------------------------------------------------------------
void destroi_lista(lista l, void destroi_objeto(void *o)) {
  limpa_lista(l, destroi_objeto);
  free(l);
}

//---------------------------------------------------------------------------------------
void nao_destroi(void *o) { ; }

//---------------------------------------------------------------------------------------
no primeiro_no(lista l) { return l->primeiro; }

//---------------------------------------------------------------------------------------
no ultimo_no(lista l) { return l->ultimo; }

//---------------------------------------------------------------------------------------
no empilha(void *o, lista l) {
  no n = (no) malloc(sizeof(t_no));
  if (n) {
    n->obj = o;
    n->proximo = primeiro_no(l);
    n->anterior = NULL;
    no aux = primeiro_no(l);
    if (aux) aux->anterior = n;
    l->primeiro = n;
    if (!ultimo_no(l))
      l->ultimo = n;
  }
  else
    printf("ERRO: Impossível empilhar objeto\n");
  
  return n;
}

//---------------------------------------------------------------------------------------
no enfileira(void *o, lista l) {
  no n = (no) malloc(sizeof(t_no));
  if (n) {
    n->obj = o;
    n->proximo = NULL;
    n->anterior = ultimo_no(l);
    no aux = ultimo_no(l);
    if (aux) aux->proximo = n;
    l->ultimo = n;
    if (!primeiro_no(l))
      l->primeiro = n;
  }
  else
    printf("ERRO: Impossível enfileirar objeto\n");

  return n; 
}

//---------------------------------------------------------------------------------------
// insere imediatamente anterior ao no q
no insere(void *o, no q, lista l) {
  no n = (no) malloc(sizeof(t_no));
  if (n) {
    n->obj = o;
    n->proximo = q;
    n->anterior = q->anterior;
    q->anterior = n;
    if (n->anterior)
      n->anterior->proximo = n;
    else
      l->primeiro = n;
  }
  else
    printf("ERRO: Impossível inserir objeto\n");

  return n; 
}

//---------------------------------------------------------------------------------------
no busca(void *chave, lista l, int compara_objeto(void *o, void *chave)) {
  no p = primeiro_no(l);
  
  while (p && !compara_objeto(objeto(p), chave))
    p = proximo_no(p);
  
  return p;
}
//---------------------------------------------------------------------------------------
void remove_no(no n, lista l, void destroi_objeto(void *o)) {
    destroi_objeto(objeto(n));
    no aux = proximo_no(n);
    if (aux)
      aux->anterior = anterior_no(n);
    else
      l->ultimo = anterior_no(n);

    aux = anterior_no(n);
    if (aux)
      aux->proximo = proximo_no(n);
    else
      l->primeiro = proximo_no(n);

    free(n);
}

//---------------------------------------------------------------------------------------
no proximo_no(no n) { return n->proximo; }

//---------------------------------------------------------------------------------------
no anterior_no(no n) { return n->anterior;}

//---------------------------------------------------------------------------------------
void *objeto(no n) { return n->obj; }

//---------------------------------------------------------------------------------------
void troca(no m, no n, lista l) {
  
  no aux1 = proximo_no(m);
  no aux2 = proximo_no(n);

  if (aux1) {
    aux1->anterior = n;
    n->proximo = aux1;
  }
  else {
    l->ultimo = n;
    n->proximo = NULL;
  }
  if (aux2) {
    aux2->anterior = m;
    m->proximo = aux2;
  }
  else {
    l->ultimo = m;
    m->proximo = NULL;
  }

  aux1 = anterior_no(m);
  aux2 = anterior_no(n);

  if (aux1) {
    aux1->proximo = n;
    n->anterior = aux1;
  }
  else {
    l->primeiro = n;
    n->anterior = NULL;
  }
  if (aux2) {
    aux2->proximo = m;
    m->anterior = aux2;
  }
  else {
    l->primeiro = m;
    m->anterior = NULL;
  }
}
