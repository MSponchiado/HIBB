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
 *  Branch and Bound intervalar (RAC direcionada aproximada).
 *
 *  Autor: Guilherme A. Derenievicz (gaderenievicz@inf.ufpr.br)
 *  Departamente de Informática - Universidade Federal do Paraná
 *
 *  versão: 1.0 27/03/18
 *
 ******************************************************************************/

#ifndef __BRANCH__
#define __BRANCH__

#include "epifita.h"
#include "heuristica.h"
#include "busca_local.h"
#include <stdio.h>
#include <stdlib.h>

extern long int TIMEOUT;                            // timeout em segundos
extern unsigned int DEBUG;                          // flag para ativar algumas informações de DEBUG da execução
extern unsigned int FULL_CONSISTENCY;               // Utiliza Full Arc Consistency ao invés de Directional

typedef struct t_ramo *ramo;

// ramo é um nó do backtracking.
// - valoracao é uma lista de Mintervalos que tem como padrão a ordem
// em que as variáveis aparecem na Árvore Epífita (ver código da função branch_and_bound).
// - var_decidida é um ponteiro para a Variavel que foi escolhida para ramificação e
// seu respectivo valor_decidido.
// - jah_visitado é um flag utilizado na pilha de bactracking
// - pai é um ponteiro pro pai na árvore de decisão
typedef struct t_ramo {

  lista valoracao;

  Variavel var_decidida;
  Mintervalo valor_decidido;

  int jah_visitado;
  int conflito;
  
  ramo pai;
  
  int nivel;

} t_ramo;

//--------------------------------------------------------------------------

ramo cria_ramo(Variavel V, Mintervalo M, ramo p, int conflit, epifita C);

void destroi_ramo(void *r);

lista valoracao(ramo r);

Variavel var_decidida(ramo r);

Mintervalo valor_decidido(ramo r);

int jah_visitado(ramo r);

ramo ramo_pai(ramo r);

int conflito(ramo r);

int nivel(ramo r);

//--------------------------------------------------------------------------
// atualiza a decomposição epífita C com os multi-intervalos da lista val:
// aplica a valoração na Árvore Epífita (seguindo a ordem das variáveis)
// *copiando* os multi-intervalos da lista val para a epifita C.
void aplica(epifita C, lista val);

// salva os multi-intervalos da decomposição epífita C no ramo r, *copiando* os multi-intervalos.
// nota-se que a lista de valoracao do ramo r é *limpada*, sendo possível reaproveitar
// um ramo.
void salva(epifita C, ramo r);

//--------------------------------------------------------------------------

// Cria um ramo filho do ramo atual no qual X foi particionado e todas as inferências feitas.
// O lado pode ser 'a', 'b' ou 'n', onde o última indica que não se deve
// particionar X.
ramo ramifica(Variavel X, restricao R, char lado, ramo atual, epifita C);

// Casca para a busca local. Aqui eu faço esse "upper-bound" que (ATENÇÃO) não valora as variáveis!
// monta a pilha de insatisfativeis com restrições não RAC-consistentes
int tem_solucao(epifita C, lista insatisfativeis);

// aplica o valor_objetivo como upper_bound na valoração do ramo_atual. 
void upper_bound(double valor_objetivo, ramo ramo_atual, epifita C);

// Essa função vai percorrer a pilha de restrições insatisfatíveis e escolher uma ramificação para continuar a busca.
// Para cada restrição insatisfatível ELEGÍVEL é testado um conjunto de 4 opções: cada cauda e cada lado, todas são empilhadas no backtracking.
// Para cada conjunto escolhe-se heuristicamente a melhor opção e armazena o seu nó do backtracking em n (mas apenas se não há conflito)
// Retorna o n da primeira ramificação encontrada não conflitante.
// A primeira heurística é respeitar a pilha, portanto, a primeira ramificação que der certo será retornada.
// A segunda heurística é definidA por "aplica_heuristica_erro_minimo_nos_arcos()".copel grupo b
// Parâmetro verifica_cabeca_degenerada: se for TRUE, a restrição é elegível apenas se não tiver a cabeça degenerada
no escolhe_variavel_e_ramifica(lista insatisfativeis, ramo ramo_atual, lista backtracking, int verifica_cabeca_degenerada, epifita C);

//--------------------------------------------------------------------------

// para DEBUG apenas.
void imprime_pilha_de_backtracking(lista backtracking);

//--------------------------------------------------------------------------

// InDE: o B&B serve para criar a populacao inicial. Assim, faz o B&B até atingir a MAX_PROFUNDIDADE,
// usando os subproblemas encontrados (e consistentes) para cria a populacao. Retorna uma lista de valorações.
lista branch_and_bound(epifita C, unsigned int MAX_PROFUNDIDADE, double *tempo_de_execucao);

//--------------------------------------------------------------------------

#endif
