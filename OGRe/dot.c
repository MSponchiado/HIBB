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
 *  Converte a Árvore Epífita para o formato .dot
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "dot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------------------------------------------------------
// Retorna uma string com o nome da variável e o seu multi-intervalo.
// OBS: retira do nome os símbolos @, # ou $.
//--------------------------------------------------------------------------
char *var_dot(Variavel X) {
    int idx = 0;
    char *name = nome(X);
    if (name[0] == '@' || name[0] == '#' || name[0] == '$') idx = 1;

    char *nome = (char *) malloc(sizeof(char)*128);
    strcpy(nome, &(name[idx]));
    int j = strlen(nome);

    nome[j++] = '\n';

    if (M_degenerado(multi_intervalo(X))) {
      sprintf(&(nome[j]), "[%f]", inf(objeto(primeiro_no(multi_intervalo(X)))));
      j = strlen(nome);
    }
    else {
      nome[j++] = '{';
      for (no p = primeiro_no(multi_intervalo(X)); p; p = proximo_no(p)) {
        intervalo I = objeto(p);

        if (eh_vazio(I))
          sprintf(&(nome[j]), "0");
        else
          sprintf(&(nome[j]), "%c%f, %f%c", Te(I)?'[':'(', inf(I), sup(I), Td(I)?']':')');

        j = strlen(nome);
        nome[j++] = ' ';
      }
      nome[j++] = '}';
    }
    nome[j] = '\0';
    return nome;
}

//--------------------------------------------------------------------------
// Retorna uma string com o nome da variável.
// OBS: retira do nome os símbolos @, # ou $.
//--------------------------------------------------------------------------
char *nome_dot(Variavel X) {
  char *name = nome(X);
  if (name[0] == '@' || name[0] == '#' || name[0] == '$')
    return &(name[1]);
  return &(name[0]);
}

//--------------------------------------------------------------------------
// Recebe a epifita e uma lista de labels de cada variavel na epifita e gera
// um arquivo graph_out.dot com a representação .dot da decomposição.
// Para isso, chama a função arvore_dot() para construir cada árvore a partir
// da raiz. A lista de labels pode ser vazia.
//--------------------------------------------------------------------------
void gera_dot(epifita C, lista labels) {

  int counter_tree = 0;
  int counter_op = 0;
  no q = labels ? primeiro_no(labels) : NULL;

  FILE *F = fopen("graph_out.dot","w");
  fprintf(F, "digraph {\n");

  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
    fprintf(F, "    subgraph cluster_%d {\n", counter_tree);
    fprintf(F, "    label=%d;\n", counter_tree++);
    q = arvore_dot(raiz(objeto(p)), &counter_op, q, TRUE, "black", F);
    fprintf(F, "    }\n");

    if (arco(objeto(p)))
      q = arvore_dot(arco(objeto(p)), &counter_op, q, FALSE, "blue", F);
  }
  fprintf(F, "}\n");
  fclose(F);
}

//--------------------------------------------------------------------------
// Gera o .dot de uma sub-árvore, a partir da restrição R, no arquivo F.
// Se q != NULL então usa os labels que estão em objeto(q).
// Retorna o nó da lista de labels que deverá ser usado na geração do restante
// da decomposição.
//--------------------------------------------------------------------------
no arvore_dot(restricao R, int *counter_op, no q, int recursivo, char *color, FILE *F) {

  fprintf(F, "        %s_%d [label=%s];\n", opcode2str(op(R)), *counter_op, opcode2str(op(R)));
  if (q) {
    restricao_labels_dot L = objeto(q);
    q = proximo_no(q);
    fprintf(F, "    %s [label=\"%s\"];\n", nome_dot(cabeca(R)), L->label_cabeca);
    fprintf(F, "    %s [label=\"%s\"];\n", nome_dot(cauda_E(R)), L->label_cauda_E);
    fprintf(F, "    %s [label=\"%s\"];\n", nome_dot(cauda_D(R)), L->label_cauda_D);
  }
  fprintf(F, "    %s -> %s_%d [color=%s];\n", nome_dot(cabeca(R)), opcode2str(op(R)), *counter_op, color);
  fprintf(F, "    %s_%d -> %s [color=%s];\n", opcode2str(op(R)), *counter_op, nome_dot(cauda_E(R)), color);
  fprintf(F, "    %s_%d -> %s [color=%s];\n", opcode2str(op(R)), *counter_op, nome_dot(cauda_D(R)), color);

  (*counter_op)++;

  if (recursivo)
    for (no p = primeiro_no(filhos(R)); p; p = proximo_no(p))
      q = arvore_dot(objeto(p), counter_op, q, TRUE, color, F);

  return q;
}

//--------------------------------------------------------------------------
// Cria e retorna a lista de labels da decomposição. Para isso, chama a função
// labels_arvore_dot() para processar cada árvore a partir da raiz.
//--------------------------------------------------------------------------
lista labels_dot(epifita C) {

  lista labels = cria_lista();

  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
    labels_arvore_dot(raiz(objeto(p)), labels, TRUE);

    if (arco(objeto(p)))
      labels_arvore_dot(arco(objeto(p)), labels, FALSE);
  }

  return labels;
}

//--------------------------------------------------------------------------
// Cria os labels para cada restrição da sub-árvore, armazenando na lista de labels.
// Lembrando que cada label nada mais é do que o nome+multi-intervalo de cada variável
//--------------------------------------------------------------------------
void labels_arvore_dot(restricao R, lista labels, int recursivo) {
    
  restricao_labels_dot L = (restricao_labels_dot) malloc(sizeof(t_restricao_labels_dot));
  L->label_cabeca = var_dot(cabeca(R));
  L->label_cauda_E = var_dot(cauda_E(R));
  L->label_cauda_D = var_dot(cauda_D(R));

  enfileira(L, labels);

  if (recursivo)
    for (no p = primeiro_no(filhos(R)); p; p = proximo_no(p))
      labels_arvore_dot(objeto(p), labels, TRUE);
}
//--------------------------------------------------------------------------
// Função que deve ser chamada para atualizar os labels. Basicamente, vai adicionar
// aos labels o multi-intervalo de cada variável. Chama a função
// update_labels_arvore_dot() para processar cada árvore a partir da raiz.
//--------------------------------------------------------------------------
void update_labels_dot(epifita C, lista labels) {

  no q = primeiro_no(labels);
  for (no p = primeiro_no(arvores(C)); p; p = proximo_no(p)) {
    q = update_labels_arvore_dot(raiz(objeto(p)), q, TRUE);
    if (arco(objeto(p)))
      q = update_labels_arvore_dot(arco(objeto(p)), q, FALSE);
  }
}

//--------------------------------------------------------------------------
// Função que atualiza os labels das restrição de uma sub-árvore, a partir da raiz.
// Para isso, chama a função update_label_var_dot para cada variável da raiz
//--------------------------------------------------------------------------
no update_labels_arvore_dot(restricao R, no q, int recursivo) {

  restricao_labels_dot L = objeto(q);
  q = proximo_no(q);

  update_label_var_dot(L->label_cabeca, cabeca(R));
  update_label_var_dot(L->label_cauda_E, cauda_E(R));
  update_label_var_dot(L->label_cauda_D, cauda_D(R));

  if (recursivo)
    for (no p = primeiro_no(filhos(R)); p; p = proximo_no(p))
      q = update_labels_arvore_dot(objeto(p), q, TRUE);

  return q;

}

//--------------------------------------------------------------------------
// Adiona ao label l da variável X o seu multi-intervalo
//--------------------------------------------------------------------------
void update_label_var_dot(char *l, Variavel X) {

  int j = strlen(l);

  l[j++] = '\n';

  if (M_degenerado(multi_intervalo(X))) {
    sprintf(&(l[j]), "[%f]", inf(objeto(primeiro_no(multi_intervalo(X)))));
    j = strlen(l);
  }
  else {
    l[j++] = '{';
    for (no p = primeiro_no(multi_intervalo(X)); p; p = proximo_no(p)) {
      intervalo I = objeto(p);

      if (eh_vazio(I))
        sprintf(&(l[j]), "0");
      else
        sprintf(&(l[j]), "%c%f, %f%c", Te(I)?'[':'(', inf(I), sup(I), Td(I)?']':')');

      j = strlen(l);
      l[j++] = ' ';
    }
    l[j++] = '}';
  }
  l[j] = '\0';
}

//--------------------------------------------------------------------------


