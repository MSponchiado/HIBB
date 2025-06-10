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
 *  Biblioteca para o pré-processador que rotaciona a Árvore Epífita.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#include "real.h"
#include "lista.h"
#include "intervalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PERMITE_CONS_ARCOS 1            // heuristica de escolha de Árvore Epífita 1
#define MAX_CABECAS_NAO_AUXILIARES 500  // heuristica de escolha de Árvore Epífita 2
#define MAX_ARCOS_UNSAT INFINITY        // heuristica de escolha de Árvore Epífita 3

#define MAX_FORMULAS_POR_DIR 1000       // número máximo de Árvores Epífitas "distintas" geradas =
#define MAX_DIR_QTD 1000                // MAX_FORMULAS_POR_DIR * MAX_DIR_QTD

typedef struct t_Var *Var;

typedef struct t_Rest *Rest;

typedef struct t_PointerRest *PointerRest;

// Var é uma estrutura que contém
// o nome (da variável), o nível que
// ela foi escolhida e uma lista de restrições
// Também contém flags indicando se eh cabeca de arco
// e/ou não auxiliar.
// Por fim, a variavel tem um intervalo com o seu
// valor consistente (podemos considerá-lo na escolha
// de uma ou outra rotação) - heurística de escolha de Árvore Epífita 4
typedef struct t_Var {

  char *nome;
  int nivel;
  lista rest;

  int cabeca_de_arco;
  int cabeca_de_arco_nao_auxiliar;

  intervalo I;

} t_Var;

// Rest é uma estrutura que contém 3 variáveis
// X, Y e Z, um char representando o operador,
// um inteiro representando quantas de suas variáveis
// já foram processadas e estão no conjunto CAP (grau de interseção da restrição)
// e outro representando se ele já foi percorrido (ok)
typedef struct t_Rest {

  Var X, Y, Z;
  int op;
  int cap;
  int ok;

} t_Rest;

// PointerRest é um ponteiro pra restrição
// especial, pois contém informações adicionais.
// A saber, a restrição apontada e dois inteiros
// ja_buscada e rotacao, representando, respectivamente,
// se a restrição já foi buscada e qual a sua rotação
// (qual variável virou a cabeça da restrição).
// Obs.: a cabeça de uma restrição é a variável isolada das demais na equação.
// Ex.: X é a cabeça de X = Y + Z
typedef struct t_PointerRest {

  Rest rest;
  int ja_buscada;
  int rotacao;

  int tipo;

} t_PointerRest;

//--------------------------------------------------------------------------
// Função que destroi um PointerRest

void destroi_PointerRest(void *Pr);

//--------------------------------------------------------------------------
// Função que compara o nome de uma variável com
// uma string, utilizada na busca de variáveis.

char *nome(Var v);

int compara_Var(void *v, void *s);

//--------------------------------------------------------------------------
// Função que aplica várias podas num dado ramo

int poda(Rest R, int arcos_insatisfativeis, int cabecas_nao_auxiliares, int otimo_cabecas_nao_auxiliares);

//--------------------------------------------------------------------------
// Função que atualiza o conjunto cap dependendo do char no parâmetro

void atualiza_conjunto_cap(lista r, char s);


//--------------------------------------------------------------------------
// Função que lê variáveis do arquivo dado

char *le_variaveis(char *arq, lista variaveis, FILE *f);

//--------------------------------------------------------------------------
// Função que lê restrições do arquivo dado

void le_restricoes(char *arq, lista variaveis, lista restricoes);

//--------------------------------------------------------------------------

lista opera(intervalo A, intervalo B, int op, int rot);

int igualdade_no_arco(Rest R, int rot);

//--------------------------------------------------------------------------
// Imprime Restrições Rotacionadas

void imprime_rotacao(lista backtrack, int gera_grafo, FILE *f);
