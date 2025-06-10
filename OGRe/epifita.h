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

#ifndef __EPIFITA__
#define __EPIFITA__

#include "arvore_restricoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELTA_FACTOR 100

extern unsigned long long int MAX_STEPS_CONSISTENCY;    // Numero máximo de passos da consistência global
extern unsigned long long int DEBUG_MAX_STEPS_REACHED;  // Quantidade de ramos nos quais a consistência atingiu
                                                        // o MAX_STEPS_CONSISTENCY
extern unsigned long long int MAX_STEPS_USED;           // Maior número de passos da consistência global que foi
                                                        // usado

// OBS: um passo da consistência global é (depois de aplicar a consistência direcionada inicial) aplicar uma rodada
// de consistência de arco para TODAS as restrições que envolvem uma dada variável.

extern unsigned long long int STEPS_USED;

typedef struct t_epifita *epifita;

// uma Árvore Epífita eh um conjunto de arvores interligadas
// por arcos e tem uma variável objetivo.
typedef struct t_epifita {

  lista arvores;
  lista complete_odv;

  Variavel var_objetivo;

} t_epifita;

//--------------------------------------------------------------------------

epifita cria_epifita(void);

void destroi_epifita(epifita C);

lista arvores(epifita C);

lista complete_odv(epifita C);

Variavel var_objetivo(epifita C);

void calcula_complete_odv(epifita C);

void seta_variavel_objetivo(epifita C);

void imprime_epifita(epifita C);

void imprime_variaveis(epifita C);

void imprime_variavel(char *s, epifita C);

// Aplica a consistência na Árvore Epífita inteira retornando, caso houver, o
// conflito. A lista de variáveis no argumento serve como ponto inicial
// do algoritmo. Se a lista for NULL, basta aplicar em todas as restrições.
int global_consistency(epifita C, lista vars);

// Aplica a consistência direcionada na Árvore Epífita inteira retornando, caso houver,
// o conflito. Se direcao >= 0, aplicada DAC, caso contrário, DAC reversa.
int directional_consistency(epifita C, no inicial, int direcao);

// devolve o maior erro que ocorre nos arcos caso
// seja aplicada uma valoração minimizadora
double max_erro_RAC(epifita C);

#endif
