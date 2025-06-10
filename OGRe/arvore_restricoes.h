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
 *  Implementação da estrutura de dados para árvores.
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __ARVORE__
#define __ARVORE__

#include "variavel.h"
#include "operacoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERRO_ABS_REL 2
#define ERRO_RELATIVO 1
#define ERRO_ABSOLUTO 0

extern double ERRO_RAC;               // ERRO (ou tolerância) permitido nas restrições RAC (vulgo "epsilon do conjunto \Omega")
extern unsigned int TIPO_ERRO_RAC;    // Tipo do ERRO, que pode ser ERRO_ABSOLUTO, ERRO_RELATIVO ou ambos (ERRO_ABS_REL)

typedef struct t_restricao *restricao;

// Uma restrição é da forma X = Y op Z
// Além disso, aponta para o pai e tem uma lista
// de apontadores para os filhos
typedef struct t_restricao {

  restricao pai;
  lista filhos;

  int op;
  Variavel X, Y, Z;
  
  lista (*f_op)(intervalo A, intervalo B);
  lista (*f_iop)(intervalo A, intervalo B);
  lista (*f_cop)(intervalo A, intervalo B);

} t_restricao;

typedef struct t_arvore *arvore;

// contém lista de folhas e variáveis das árvores
// e aponta para a raiz. Também contém o arco que "liga"
// a cabeça da raiz em outras árvores da Árvore Epífita
typedef struct t_arvore {

  restricao raiz;
  restricao arco;
  lista variaveis;
  lista odv;

} t_arvore;

//--------------------------------------------------------------------------

restricao cria_restricao(Variavel X, Variavel Y, Variavel Z, int operador);

void destroi_restricao(void *R);

restricao pai(restricao R);

lista filhos(restricao R);

Variavel cabeca(restricao R);

Variavel cauda_E(restricao R);

Variavel cauda_D(restricao R);

int op(restricao R);

// verifica se X é a cabeça da restrição R
int compara_cabeca_restricao(void *X, void *R);

// verifica se as restrições R e S são iguais
int compara_restricao(void *R, void *S);

// retorna 1 se R é RAC
int eh_RAC(restricao R);

//--------------------------------------------------------------------------

arvore cria_arvore(void);

void destroi_arvore(void *T);

restricao raiz(arvore T);

restricao arco(arvore T);

lista variaveis(arvore T);

lista odv(arvore T);

arvore adiciona_arco(restricao R, arvore T);

arvore adiciona_restricao(restricao R, arvore T, no p);

void imprime_restricoes(restricao R, int recursivamente);

//--------------------------------------------------------------------------

// Directional Arc Consistency, ordenado da raiz para as folhas (DAC) ou o
// contrario (DAC_reverse).
// Obs.: pode ser usado tanto para uma unica arvore quanto para um conjunto,
// isto eh, toda a Árvore Epífita.
int DAC(no inicial, int *conflito);

int DAC_reverse(no inicial, int *conflito);

// consistencia de arco na arvore T, linear no numero de restrições.
// retorna 1 se alterou algum ou 0 caso a árvore já fosse consistente
int arc_consistency(arvore T, int *conflito);

// aplica arc consistency na restrição R
// retorna um inteiro de 3 digitos que representa um número binário cujos
// dígitos significam: d1 d2 d3 --> d1 é se alterou a cauda_D(R)
// d2 é se alterou a cauda_E(R) e d3 é se alterou a cabeca(R).
int local_consistency(restricao R, int *conflito);

//--------------------------------------------------------------------------
// transforma o erro absoluto em relativo, se for o caso
double normaliza_erro_RAC(double EA, double ref);

// Função auxiliar da erro_de_valoracao(R):
// Recebe 2 multi-intervalos M e tmp, M \subseteq tmp, e retorna a maior
// distância entre dois pontos quaisquer tomados destes multi-intervalos.
// Essa distância pode ser absoluta ou relativa, dependendo do tipo de erro
// utilizado para calcular RAC.
double maior_distancia(Mintervalo M, Mintervalo tmp);

// devolve o erro maximo que pode ocorrer na restricao R
// Aqui tornei essa função um pouco estranha: ela retorna o menor erro dos máximos das rotações, isto é,
// a função auxiliar maior_distancia() retorna o maior erro de uma rotação, mas essa função vai retornar o menor
// desses valores para todas as 3 rotações.
double erro_RAC(restricao R);

#endif
