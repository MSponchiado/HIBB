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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Como usar:                                                                                                                            //
//                                                                                                                                       //
// 1 - chamar "gera_dot(C, NULL);" após criar a epífita para gerar o grafo apenas com os nomes das variáveis.                            //
// 2 - se quiser incluir no grafo também os multi-intervalos antes e depois de uma tentativa de valoração:                               //
//     2.a - "lista labels = labels_dot(C);" antes de começar a valoração, para obter os labels com multi-intervalos consistentes.       //
//     2.b - "update_labels_dot(C, labels);"                                                                                             //
//           "gera_dot(C, labels);"                                                                                                      //
//           "exit(1);" após efetuar a valoração, para incluir nos labels os intervalos degenerados obtidos e gerar novamente o grafo.   //
//                                                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DOT__
#define __DOT__

#include "epifita.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_restricao_labels_dot *restricao_labels_dot;

// estrutura que guarda labels (nome + multi-intervalo) das variáveis de uma restrição
typedef struct t_restricao_labels_dot {

  char *label_cabeca;
  char *label_cauda_E;
  char *label_cauda_D;

} t_restricao_labels_dot;

//--------------------------------------------------------------------------
// Retorna uma string com o nome da variável e o seu multi-intervalo.
// OBS: retira do nome os símbolos @, # ou $.
//--------------------------------------------------------------------------
char *var_dot(Variavel X);

//--------------------------------------------------------------------------
// Retorna uma string com o nome da variável.
// OBS: retira do nome os símbolos @, # ou $.
//--------------------------------------------------------------------------
char *nome_dot(Variavel X);

//--------------------------------------------------------------------------
// Recebe a epifita e uma lista de labels de cada variavel na epifita e gera
// um arquivo graph_out.dot com a representação .dot da decomposição.
// Para isso, chama a função arvore_dot() para construir cada árvore a partir
// da raiz. A lista de labels pode ser vazia.
//--------------------------------------------------------------------------
void gera_dot(epifita C, lista labels);

//--------------------------------------------------------------------------
// Gera o .dot de uma sub-árvore, a partir da restrição R, no arquivo F.
// Se q != NULL então usa os labels que estão em objeto(q).
// Retorna o nó da lista de labels que deverá ser usado na geração do restante
// da decomposição.
//--------------------------------------------------------------------------
no arvore_dot(restricao R, int *counter_op, no q, int recursivo, char *color, FILE *F);

//--------------------------------------------------------------------------
// Cria e retorna a lista de labels da decomposição. Para isso, chama a função
// labels_arvore_dot() para processar cada árvore a partir da raiz.
//--------------------------------------------------------------------------
lista labels_dot(epifita C);

//--------------------------------------------------------------------------
// Cria os labels para cada restrição da sub-árvore, armazenando na lista de labels.
// Lembrando que cada label nada mais é do que o nome+multi-intervalo de cada variável
//--------------------------------------------------------------------------
void labels_arvore_dot(restricao R, lista labels, int recursivo);

//--------------------------------------------------------------------------
// Função que deve ser chamada para atualizar os labels. Basicamente, vai adicionar
// aos labels o multi-intervalo de cada variável. Chama a função
// update_labels_arvore_dot() para processar cada árvore a partir da raiz.
//--------------------------------------------------------------------------
void update_labels_dot(epifita C, lista labels);

//--------------------------------------------------------------------------
// Função que atualiza os labels das restrição de uma sub-árvore, a partir da raiz.
// Para isso, chama a função update_label_var_dot para cada variável da raiz
//--------------------------------------------------------------------------
no update_labels_arvore_dot(restricao R, no q, int recursivo);

//--------------------------------------------------------------------------
// Adiona ao label l da variável X o seu multi-intervalo
//--------------------------------------------------------------------------
void update_label_var_dot(char *l, Variavel X);

#endif

